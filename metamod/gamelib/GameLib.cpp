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

#include "GameLib.hpp"
#include <Metamod.hpp>
#include <DllExports.hpp>
#include "EntitiesHooks.hpp"

#include <yaml-cpp/yaml.h>

namespace Metamod::GameLib
{
    GameLib::GameLib(const std::unique_ptr<Engine::Engine> &engine,
                     std::string_view gameDir,
                     const fs::path &metaConfigsDir) : m_hooks(std::make_unique<Hooks>())
    {
        for (const ModInfo& modInfo : knownGames)
        {
            if (modInfo.name == gameDir)
            {
                break;
            }
            m_modPos++;
        }

        if (knownGamesNum == m_modPos)
        {
            throw std::runtime_error("GameDLL not recognized");
        }

        m_gameDir = fs::current_path() / knownGames[m_modPos].name;
        m_pathName = fs::current_path() /
                     knownGames[m_modPos].name /
                     "dlls" /
#if defined __linux__
                     knownGames[m_modPos].libNameLinux;
#elif defined _WIN32
                     m_knowGames[m_modPos].libNameWin32;
#endif

        _loadGameDLL(engine);
        _loadVOffsets(metaConfigsDir);
    }

    std::string_view GameLib::getName() const
    {
        return knownGames[m_modPos].name;
    }

    std::string_view GameLib::getDesc() const
    {
        return knownGames[m_modPos].desc;
    }

    const fs::path &GameLib::getGameDir() const
    {
        return m_gameDir;
    }

    const fs::path &GameLib::getPathname() const
    {
        return m_pathName;
    }

    void GameLib::_loadGameDLL(const std::unique_ptr<Engine::Engine> &engine)
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
        fnGiveEngFuncs(const_cast<enginefuncs_t *>(engine->getEngineFuncs()), gpGlobals);

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

    void GameLib::_replaceFuncs()
    {
        m_dllApiTable = gEntityInterface;
        m_newDllApiTable = gNewDLLFunctions;

        // Replace only funcs we want to have hooked
#define ASSIGN_ENT_FUNC(func) (m_dllApiTable.func = Callbacks::Engine::func)
        ASSIGN_ENT_FUNC(pfnGameInit);
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

    const DLL_FUNCTIONS *GameLib::getDllFuncs() const
    {
        return &m_dllApiTable;
    }

    const NEW_DLL_FUNCTIONS *GameLib::getNewDllFuncs() const
    {
        return &m_newDllApiTable;
    }

    Hooks *GameLib::getHooks() const
    {
        return m_hooks.get();
    }

    bool GameLib::callGameEntity(std::string_view name, Engine::IEntVars *pev)
    {
        using ENTITY_FN = void (*)(entvars_t *);

        auto pfnEntity = m_gameLibrary->getSymbol<ENTITY_FN>(name.data());
        
        if (!pfnEntity)
        {
            return false;
        }

        std::invoke(pfnEntity, *dynamic_cast<Engine::EntVars *>(pev));

        return true;
    }

    void GameLib::pfnGameInit(FuncCallType callType)
    {
        static GameInitHookRegistry *hookchain = m_hooks->gameInit();
        _execGameDLLFunc(hookchain, []() {
            gEntityInterface.pfnGameInit();
        }, callType);
    }

    bool GameLib::pfnClientConnect(Engine::IEdict *pEntity,
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
                    *dynamic_cast<::Metamod::Engine::Edict *>(pEntity), pszName.data(), pszAddress.data(), szRejectReason.data()
                )
            );
        }, callType, pEntity, pszName, pszAddress, std::ref(szRejectReason));
    }

    void GameLib::pfnClientPutInServer(Engine::IEdict *pEntity, FuncCallType callType)
    {
        static ClientPutinServerHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->clientPutinServer();
        _execGameDLLFunc(hookchain, [](Engine::IEdict *pEntity) {
            gEntityInterface.pfnClientPutInServer(*dynamic_cast<::Metamod::Engine::Edict *>(pEntity));
        }, callType, pEntity);
    }

    void GameLib::pfnClientCommand(Engine::IEdict *pEntity, FuncCallType callType)
    {
        static ClientCmdHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->clientCmd();
        _execGameDLLFunc(hookchain, [](Engine::IEdict *pEntity) {
            gEntityInterface.pfnClientCommand(*dynamic_cast<::Metamod::Engine::Edict *>(pEntity));
        }, callType, pEntity);
    }

    void GameLib::pfnClientUserInfoChanged(Engine::IEdict *pEntity, char *infobuffer, FuncCallType callType)
    {
        static ClientInfoChangedHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->clientInfoChanged();
        _execGameDLLFunc(hookchain, [](Engine::IEdict *pEntity, char *infobuffer) {
            gEntityInterface.pfnClientUserInfoChanged(*dynamic_cast<::Metamod::Engine::Edict *>(pEntity), infobuffer);
        }, callType, pEntity, infobuffer);
    }

    void GameLib::pfnServerActivate(std::uint32_t edictCount, std::uint32_t clientMax, FuncCallType callType)
    {
        static ServerActivateHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->serverActivate();
        _execGameDLLFunc(hookchain, [](std::uint32_t edictCount, std::uint32_t clientMax) {
            gEntityInterface.pfnServerActivate(gMetaGlobal->getEngine()->getEngineEdictList(),
                                               static_cast<int>(edictCount),
                                               static_cast<int>(clientMax));
        }, callType, edictCount, clientMax);
    }

    void GameLib::pfnServerDeactivate(FuncCallType callType)
    {
        static ServerDeactivateHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->serverDeactivate();
        _execGameDLLFunc(hookchain, []() {
            gEntityInterface.pfnServerDeactivate();
        }, callType);
    }

    void GameLib::pfnStartFrame(FuncCallType callType)
    {
        static StartFrameHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->startFrame();
        _execGameDLLFunc(hookchain, []() {
            gEntityInterface.pfnStartFrame();
        }, callType);
    }

    void GameLib::pfnGameShutdown(FuncCallType callType)
    {
        static GameShutdownHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->gameShutdown();
        _execGameDLLFunc(hookchain, [this]() {
            gMetaGlobal->getEngine()->removeExtDll(getSystemHandle());
            if (gNewDLLFunctions.pfnGameShutdown)
            {
                gNewDLLFunctions.pfnGameShutdown();
            }
        }, callType);
    }

    Module::SystemHandle GameLib::getSystemHandle() const
    {
        return *m_gameLibrary;
    }

    Entities::IBaseEntity *GameLib::getBaseEntity(const Engine::IEdict *edict)
    {
        switch (gMetaGlobal->getGameType())
        {
            case GameType::Valve:
                return _getEntity<Entities::Valve::BaseEntity>(edict);
            case GameType::CStrike:
            case GameType::CZero:
                return _getEntity<Entities::CStrike::BaseEntity>(edict);
            default:
                return nullptr;
        }
    }

    Entities::IBasePlayer *GameLib::getBasePlayer(const Engine::IEdict *edict)
    {
        std::uint32_t idx = edict->getIndex();
        if (idx > m_maxClients || !idx)
        {
            return nullptr;
        }

        switch (gMetaGlobal->getGameType())
        {
            case GameType::Valve:
                return _getEntity<Entities::Valve::BasePlayer>(edict);
            case GameType::CStrike:
            case GameType::CZero:
                //return _getEntity<Entities::CStrike::BasePlayer>(edict);
            default:
                return nullptr;
        }
    }

    void GameLib::setMaxClients(std::uint32_t maxPlayers)
    {
        m_maxClients = maxPlayers;
    }

    BasePlayerHooks *GameLib::getCBasePlayerHooks()
    {
        return m_basePlayerhooks.get();
    }

    std::intptr_t GameLib::getOriginalVFunc(std::string_view vname) const
    {
        return m_originalVFuncs.at(vname.data());
    }

    void GameLib::_loadVOffsets(const fs::path &metaConfigsDir)
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
                if (m_pevOffset == std::numeric_limits<std::uint32_t>::max() &&
                    funcIt->first.as<std::string>() == "pev")
                {
                    m_pevOffset = funcIt->second[osName.data()].as<std::uint32_t>();
                    continue;
                }

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

    std::uint32_t GameLib::getPevOffset() const
    {
        return m_pevOffset;
    }
}
