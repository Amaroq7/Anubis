/*
 *  Copyright (C) 2020-2021 Metamod++ Development Team
 *
 *  This file is part of Metamod++.
 *
 *  Metamod++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Metamod++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Metamod++.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <extdll.h>
#include <enginecallback.h>
#include <util.h>

#include "../EngineExports.hpp"

#include "Library.hpp"
#include <Metamod.hpp>
#include <DllExports.hpp>
#include <game/IEntityHolder.hpp>
#include <engine/IEntVars.hpp>
#include <engine/ILibrary.hpp>

#include <yaml-cpp/yaml.h>

namespace Metamod::Game
{
    Library::Library(Engine::ILibrary *engine,
                     std::string_view gameDir,
                     const fs::path &metaConfigsDir) : m_hooks(std::make_unique<Hooks>()), m_engine(engine)
    {
        if (gameDir == "valve")
        {
            m_modType = Mod::Valve;
        }
        else if (gameDir == "cstrike")
        {
            m_modType = Mod::CStrike;
        }
        else if (gameDir == "czero")
        {
            m_modType = Mod::CZero;
        }
        else
        {
            throw std::runtime_error("Game mod not recognized");
        }

        Callbacks::Engine::init(this, engine);

        auto modId = static_cast<std::underlying_type_t<Mod>>(m_modType);

        m_gameDir = fs::current_path() / gameDir;
        m_pathName = fs::current_path() / gameDir / "dlls" /
#if defined __linux__
        knownGames[modId].libNameLinux;
#elif defined _WIN32
        knownGames[modId].libNameWin32;
#endif

        _loadGameDLL();
        // Dont load voffsets for CStrike and CZero for now
        if (m_modType == Mod::Valve)
        {
            _loadVOffsets(metaConfigsDir);
        }
    }

    std::string_view Library::getName() const
    {
        return knownGames[static_cast<std::underlying_type_t<Mod>>(m_modType)].name;
    }

    std::string_view Library::getDesc() const
    {
        return gEntityInterface.pfnGetGameDescription();
    }

    const fs::path &Library::getGameDir() const
    {
        return m_gameDir;
    }

    const fs::path &Library::getPathname() const
    {
        return m_pathName;
    }

    void Library::_loadGameDLL()
    {
        using namespace std::string_literals;

        m_gameLibrary = std::make_unique<Module>(m_pathName);

        if (!m_gameLibrary->isLoaded())
        {
            throw std::runtime_error("Error while loading game dll: "s + Module::getError().data());
        }

        auto fnGiveEngFuncs = m_gameLibrary->getSymbol<Engine::fnGiveFuncs>(engineEntryFuncName);
        if (!fnGiveEngFuncs)
        {
            throw std::runtime_error("Cannot find GiveFnptrsToDll in the game library");
        }
        fnGiveEngFuncs(const_cast<enginefuncs_t *>(m_engine->getEngineFuncs()), gpGlobals);

        auto fnGetNewDLLFuncs = m_gameLibrary->getSymbol<fnNewDllFunctions>(newDllApiFuncName);
        if (fnGetNewDLLFuncs)
        {
            std::int32_t version = newDLLInterfaceVersion;
            if (!fnGetNewDLLFuncs(&gNewDLLFunctions, &version))
            {
                if (version > newDLLInterfaceVersion)
                {
                    throw std::runtime_error("New DLL API functions not compatible. Metamod outdated.");
                }
                else
                {
                    throw std::runtime_error("New DLL API functions not compatible. GameDLL outdated.");
                }
            }
        }

        auto fnGetEntityAPI2 = m_gameLibrary->getSymbol<fnDllFunctionsv2>(entityApi2FuncName);
        if (fnGetEntityAPI2)
        {
            std::int32_t version = entityAPIInterfaceVersion;
            if (!fnGetEntityAPI2(&gEntityInterface, &version))
            {
                if (version > entityAPIInterfaceVersion)
                {
                    throw std::runtime_error("EntityAPI2 functions not compatible. Metamod outdated.");
                }
                else
                {
                    throw std::runtime_error("EntityAPI2 functions not compatible. GameDLL outdated.");
                }
            }
        }
        else
        {
            auto fnGetEntityAPI = m_gameLibrary->getSymbol<fnDllFunctions>(entityApiFuncName);
            if (fnGetEntityAPI)
            {
                if (!fnGetEntityAPI(&gEntityInterface, entityAPIInterfaceVersion))
                {
                    throw std::runtime_error("Cannot get retrieve EntityAPI functions from GameDLL.");
                }
            }
            else
            {
                throw std::runtime_error("Cannot get retrieve EntityAPI functions from GameDLL.");
            }
        }

        _replaceFuncs();
    }

    void Library::_replaceFuncs()
    {
        m_dllApiTable = gEntityInterface;
        m_newDllApiTable = gNewDLLFunctions;

        // Replace only funcs we want to have hooked
#define ASSIGN_ENT_FUNC(func) (m_dllApiTable.func = Callbacks::Engine::func)
        ASSIGN_ENT_FUNC(pfnGameInit);
        ASSIGN_ENT_FUNC(pfnSpawn);
        ASSIGN_ENT_FUNC(pfnClientConnect);
        ASSIGN_ENT_FUNC(pfnClientPutInServer);
        ASSIGN_ENT_FUNC(pfnClientCommand);
        ASSIGN_ENT_FUNC(pfnClientUserInfoChanged);
        ASSIGN_ENT_FUNC(pfnServerActivate);
        ASSIGN_ENT_FUNC(pfnServerDeactivate);
        ASSIGN_ENT_FUNC(pfnStartFrame);
#undef ASSIGN_ENT_FUNC
#define ASSIGN_NEW_DLL_FUNC(func) (m_newDllApiTable.func = Callbacks::Engine::func)
        ASSIGN_NEW_DLL_FUNC(pfnGameShutdown);
#undef ASSIGN_NEW_DLL_FUNC
    }

    void *Library::getDllFuncs()
    {
        return &m_dllApiTable;
    }

    void *Library::getNewDllFuncs()
    {
        return &m_newDllApiTable;
    }

    Mod Library::getMod() const
    {
        return m_modType;
    }

    Hooks *Library::getHooks() const
    {
        return m_hooks.get();
    }

    bool Library::callGameEntity(std::string_view name, Engine::IEntVars *pev)
    {
        using ENTITY_FN = void (*)(entvars_t *);

        auto pfnEntity = m_gameLibrary->getSymbol<ENTITY_FN>(name.data());
        
        if (!pfnEntity)
        {
            return false;
        }

        std::invoke(pfnEntity, static_cast<entvars_t *>(*pev));
        return true;
    }

    void Library::pfnGameInit(FuncCallType callType)
    {
        static GameInitHookRegistry *hookchain = m_hooks->gameInit();
        _execGameDLLFunc(hookchain, []() {
            gEntityInterface.pfnGameInit();
        }, callType);
    }

    std::int32_t Library::pfnSpawn(Engine::IEdict *edict, FuncCallType callType)
    {
        static SpawnHookRegistry *hookchain = m_hooks->spawn();
        return _execGameDLLFunc(hookchain, [](Engine::IEdict *edict) {
            return gEntityInterface.pfnSpawn(static_cast<edict_t *>(*edict));
        }, callType, edict);
    }

    bool Library::pfnClientConnect(Engine::IEdict *pEntity,
                                   std::string_view pszName,
                                   std::string_view pszAddress,
                                   std::string &szRejectReason,
                                   FuncCallType callType)
    {
        static ClientConnectHookRegistry *hookchain = m_hooks->clientConnect();
        return _execGameDLLFunc(hookchain, [](::Metamod::Engine::IEdict *pEntity, std::string_view pszName, std::string_view pszAddress, std::string &szRejectReason) {
            constexpr const std::size_t REASON_REJECT_MAX_LEN = 128;
            if (szRejectReason.capacity() < REASON_REJECT_MAX_LEN)
            {
                szRejectReason.reserve(REASON_REJECT_MAX_LEN);
            }
            return static_cast<bool>(
                gEntityInterface.pfnClientConnect(
                    static_cast<edict_t *>(*pEntity), pszName.data(), pszAddress.data(), szRejectReason.data()
                )
            );
        }, callType, pEntity, pszName, pszAddress, std::ref(szRejectReason));
    }

    void Library::pfnClientPutInServer(Engine::IEdict *pEntity, FuncCallType callType)
    {
        static ClientPutinServerHookRegistry *hookchain = m_hooks->clientPutinServer();
        _execGameDLLFunc(hookchain, [](Engine::IEdict *pEntity) {
            gEntityInterface.pfnClientPutInServer(static_cast<edict_t *>(*pEntity));
        }, callType, pEntity);
    }

    void Library::pfnClientCommand(Engine::IEdict *pEntity, FuncCallType callType)
    {
        static ClientCmdHookRegistry *hookchain = m_hooks->clientCmd();
        _execGameDLLFunc(hookchain, [](Engine::IEdict *pEntity) {
            gEntityInterface.pfnClientCommand(static_cast<edict_t *>(*pEntity));
        }, callType, pEntity);
    }

    void Library::pfnClientUserInfoChanged(Engine::IEdict *pEntity, Engine::InfoBuffer infobuffer, FuncCallType callType)
    {
        static ClientInfoChangedHookRegistry *hookchain = m_hooks->clientInfoChanged();
        _execGameDLLFunc(hookchain, [](Engine::IEdict *pEntity, Engine::InfoBuffer infobuffer) {
            gEntityInterface.pfnClientUserInfoChanged(static_cast<edict_t *>(*pEntity), infobuffer);
        }, callType, pEntity, infobuffer);
    }

    void Library::pfnServerActivate(std::uint32_t edictCount, std::uint32_t clientMax, FuncCallType callType)
    {
        static ServerActivateHookRegistry *hookchain = m_hooks->serverActivate();
        _execGameDLLFunc(hookchain, [](std::uint32_t edictCount, std::uint32_t clientMax) {
            gEntityInterface.pfnServerActivate(gMetaGlobal->getEngine()->getEngineEdictList(),
                                               static_cast<int>(edictCount),
                                               static_cast<int>(clientMax));
        }, callType, edictCount, clientMax);
    }

    void Library::pfnServerDeactivate(FuncCallType callType)
    {
        static ServerDeactivateHookRegistry *hookchain = m_hooks->serverDeactivate();
        _execGameDLLFunc(hookchain, []() {
            gEntityInterface.pfnServerDeactivate();
        }, callType);
    }

    void Library::pfnStartFrame(FuncCallType callType)
    {
        static StartFrameHookRegistry *hookchain = m_hooks->startFrame();
        _execGameDLLFunc(hookchain, []() {
            gEntityInterface.pfnStartFrame();
        }, callType);
    }

    void Library::pfnGameShutdown(FuncCallType callType)
    {
        static GameShutdownHookRegistry *hookchain = m_hooks->gameShutdown();
        _execGameDLLFunc(hookchain, []() {
            if (gNewDLLFunctions.pfnGameShutdown)
            {
                gNewDLLFunctions.pfnGameShutdown();
            }
        }, callType);
    }

    Module::SystemHandle Library::getSystemHandle() const
    {
        return static_cast<Module::SystemHandle >(*m_gameLibrary);
    }

    IBaseEntity *Library::getBaseEntity(Engine::IEdict *edict)
    {
        if (!m_entityHolder)
        {
            return nullptr;
        }

        return m_entityHolder->getBaseEntity(edict);
    }

    IBasePlayer *Library::getBasePlayer(Engine::IEdict *edict)
    {
        if (!m_entityHolder)
        {
            return nullptr;
        }

        std::uint32_t idx = edict->getIndex();
        if (idx > m_maxClients || !idx)
        {
            return nullptr;
        }

        return m_entityHolder->getBasePlayer(edict);
    }

    void Library::setMaxClients(std::uint32_t maxPlayers)
    {
        m_maxClients = maxPlayers;
    }

    IBasePlayerHooks *Library::getCBasePlayerHooks()
    {
        return m_basePlayerhooks;
    }

    void Library::_loadVOffsets(const fs::path &metaConfigsDir)
    {
        try
        {
            fs::path virtualOffsetsPath =
                metaConfigsDir / "gamedata" / "virtual" / getName() / "offsets.yml";
#if defined __linux__
            YAML::Node rootNode = YAML::LoadFile(virtualOffsetsPath.c_str());
            std::string_view osName("linux");
#elif defined _WIN32
            YAML::Node rootNode = YAML::LoadFile(virtualOffsetsPath.string().c_str());
            std::string_view osName("windows");
#endif
            for (auto funcIt = rootNode.begin(); funcIt != rootNode.end(); ++funcIt)
            {
                m_virtualOffsets.try_emplace(funcIt->first.as<std::string>(),
                                                funcIt->second[osName.data()].as<std::uint32_t>());
            }
        }
        catch (const YAML::BadFile &e)
        {
            using namespace std::string_literals;
            throw std::runtime_error("Error parsing yaml vtable offsets file: "s + e.what());
        }
    }

    void Library::initVFuncHooks()
    {
        fs::path mmDllPath = gMetaGlobal->getPath(PathType::Metamod) / "dlls";

        if (m_modType == Mod::Valve)
        {
#if defined __linux__
            mmDllPath /= "libvalve_api.so";
#elif defined _WIN32
            mmDllPath /= "valve_api.dll";
#endif
        }
        else if (m_modType == Mod::CStrike || m_modType == Mod::CZero)
        {
#if defined __linux__
            mmDllPath /= "libcstrike_api.so";
#elif defined _WIN32
            mmDllPath /= "cstrike_api.dll";
#endif
        }
        else
        {
            mmDllPath.clear();
        }

        _initGameEntityDLL(std::move(mmDllPath));
    }

    void Library::_initGameEntityDLL(fs::path &&path)
    {
        if (path.empty())
        {
            gMetaGlobal->logMsg(LogLevel::Warning, LogDest::ConsoleFile,
                                "Cannot find entity library for {}", m_gameDir.filename().string());
            gMetaGlobal->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Virtual func hooking is unavailable");
            return;
        }

        auto entityLib = std::make_unique<Module>(path);
        if (!entityLib->isLoaded())
        {
            gMetaGlobal->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Cannot load entity library: {}", path.string());
            gMetaGlobal->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Virtual func hooking is unavailable");
            return;
        }

        auto InitFn = entityLib->getSymbol<fnInitGameEntityDLL>("InitGameEntitiesDLL");
        if (!InitFn)
        {
            gMetaGlobal->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Cannot find func: InitGameEntitiesDLL");
            gMetaGlobal->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Virtual func hooking is unavailable");
            return;
        }

        std::invoke(InitFn, gMetaGlobal.get(), m_virtualOffsets);

        auto GetBasePlayerHooks = entityLib->getSymbol<fnGetBasePlayerHooks>("GetBasePlayerHooks");
        if (!GetBasePlayerHooks)
        {
            gMetaGlobal->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Cannot find: GetBasePlayerHooks");
            gMetaGlobal->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Virtual func hooking is unavailable");
            return;
        }

        auto GetEntityHolder = entityLib->getSymbol<fnGetEntityHolder>("GetEntityHolder");
        if (!GetEntityHolder)
        {
            gMetaGlobal->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Cannot find: GetEntityHolder");
            gMetaGlobal->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Virtual func hooking is unavailable");
            return;
        }

        m_basePlayerhooks = std::invoke(GetBasePlayerHooks);
        m_entityHolder = std::invoke(GetEntityHolder);

        m_entityLibrary = std::move(entityLib);
    }
}
