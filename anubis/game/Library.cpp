/*
 *  Copyright (C) 2020-2021 Anubis Development Team
 *
 *  This file is part of Anubis.
 *
 *  Anubis is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Anubis is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Anubis.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <extdll.h>

#include <EngineExports.hpp>

#include "Library.hpp"
#include "engine/Callbacks.hpp"

#include <Anubis.hpp>
#include <DllExports.hpp>
#include <engine/ILibrary.hpp>

namespace Anubis::Game
{
    Library::Library(nstd::observer_ptr<Engine::ILibrary> engine,
                     std::string_view gameDir,
                     const std::unique_ptr<Logger> &logger)
        : m_hooks(std::make_unique<Hooks>()),
          m_engine(engine),
          m_logger(logger),
          m_dllFunctions(std::make_unique<DLL_FUNCTIONS>()),
          m_newDllFunctions(std::make_unique<NEW_DLL_FUNCTIONS>()),
          m_gameLibDllFunctions(std::make_unique<DLL_FUNCTIONS>()),
          m_gameLibNewDllFunctions(std::make_unique<NEW_DLL_FUNCTIONS>())
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
        else if (gameDir == "dod")
        {
            m_modType = Mod::DoD;
        }
        else if (gameDir == "tfc")
        {
            m_modType = Mod::TFC;
        }
        else
        {
            throw std::runtime_error("Game mod not recognized");
        }

        Callbacks::Engine::getGame(this);
        Callbacks::Engine::getEngine(m_engine);
        Engine::Callbacks::GameDLL::getGame(this);

        auto modId = static_cast<std::underlying_type_t<Mod>>(m_modType);

        m_gameDir = std::filesystem::current_path() / gameDir;
        m_pathName = std::filesystem::current_path() / gameDir / "dlls" /
#if defined __linux__
                     knownGames[modId].libNameLinux;
#elif defined _WIN32
                     knownGames[modId].libNameWin32;
#endif

        _loadGameDLL();
    }

    std::string_view Library::getName() const
    {
        return knownGames[static_cast<std::underlying_type_t<Mod>>(m_modType)].name;
    }

    std::string_view Library::getDesc() const
    {
        return m_gameLibDllFunctions->pfnGetGameDescription();
    }

    const std::filesystem::path &Library::getGameDir() const
    {
        return m_gameDir;
    }

    const std::filesystem::path &Library::getPathname() const
    {
        return m_pathName;
    }

    void Library::_loadGameDLL()
    {
        using namespace std::string_literals;

        try
        {
            m_gameLibrary = std::make_unique<Module>(m_pathName);
        }
        catch (const std::runtime_error &e)
        {
            throw std::runtime_error("Error while loading game dll: "s + e.what());
        }

        auto fnGiveEngFuncs = m_gameLibrary->getSymbol<Engine::fnGiveFuncs>(engineEntryFuncName);
        if (!fnGiveEngFuncs)
        {
            throw std::runtime_error("Cannot find GiveFnptrsToDll in the game library");
        }
        fnGiveEngFuncs(m_engine->getEngineFuncs().get(), m_engine->getGlobals().get());

        auto fnGetNewDLLFuncs = m_gameLibrary->getSymbol<fnNewDllFunctions>(newDllApiFuncName);
        if (fnGetNewDLLFuncs)
        {
            std::int32_t version = newDLLInterfaceVersion;
            if (!fnGetNewDLLFuncs(m_gameLibNewDllFunctions.get(), &version))
            {
                if (version > newDLLInterfaceVersion)
                {
                    throw std::runtime_error("New DLL API functions not compatible. Anubis outdated.");
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
            if (!fnGetEntityAPI2(m_gameLibDllFunctions.get(), &version))
            {
                if (version > entityAPIInterfaceVersion)
                {
                    throw std::runtime_error("EntityAPI2 functions not compatible. Anubis outdated.");
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
                if (!fnGetEntityAPI(m_gameLibDllFunctions.get(), entityAPIInterfaceVersion))
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
        *m_dllFunctions = *m_gameLibDllFunctions;
        *m_newDllFunctions = *m_gameLibNewDllFunctions;

        // Replace only funcs we want to have hooked
#define ASSIGN_ENT_FUNC(func) ((*m_dllFunctions).func = Callbacks::Engine::func)
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
#define ASSIGN_NEW_DLL_FUNC(func) ((*m_newDllFunctions).func = Callbacks::Engine::func)
        ASSIGN_NEW_DLL_FUNC(pfnGameShutdown);
        ASSIGN_NEW_DLL_FUNC(pfnCvarValue);
        ASSIGN_NEW_DLL_FUNC(pfnCvarValue2);
#undef ASSIGN_NEW_DLL_FUNC
    }

    const std::unique_ptr<DLL_FUNCTIONS> &Library::getDllFuncs()
    {
        return m_dllFunctions;
    }

    const std::unique_ptr<NEW_DLL_FUNCTIONS> &Library::getNewDllFuncs()
    {
        return m_newDllFunctions;
    }

    Mod Library::getMod() const
    {
        return m_modType;
    }

    nstd::observer_ptr<IHooks> Library::getHooks() const
    {
        return m_hooks;
    }

    bool Library::callGameEntity(std::string_view name, nstd::observer_ptr<Engine::IEdict> pEntity)
    {
        using EntityFn = void (*)(entvars_t *);

        auto pfnEntity = m_gameLibrary->getSymbol<EntityFn>(name.data());
        if (!pfnEntity)
        {
            return false;
        }

        std::invoke(pfnEntity, static_cast<entvars_t *>(*pEntity));
        return true;
    }

    void Library::pfnGameInit(FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_gameLibDllFunctions->pfnGameInit();
        }

        static auto hookChain = m_hooks->gameInit();
        hookChain->callChain(
            [this]()
            {
                m_gameLibDllFunctions->pfnGameInit();
            });
    }

    std::int32_t Library::pfnSpawn(nstd::observer_ptr<Engine::IEdict> pEntity, FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_gameLibDllFunctions->pfnSpawn(static_cast<edict_t *>(*pEntity));
        }

        static auto hookChain = m_hooks->spawn();

        return hookChain->callChain(
            [this](nstd::observer_ptr<Engine::IEdict> edict)
            {
                return m_gameLibDllFunctions->pfnSpawn(static_cast<edict_t *>(*edict));
            },
            pEntity);
    }

    bool Library::pfnClientConnect(nstd::observer_ptr<Engine::IEdict> pEntity,
                                   std::string_view pszName,
                                   std::string_view pszAddress,
                                   std::string &szRejectReason,
                                   FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_gameLibDllFunctions->pfnClientConnect(static_cast<edict_t *>(*pEntity), pszName.data(),
                                                           pszAddress.data(), szRejectReason.data());
        }

        static auto hookChain = m_hooks->clientConnect();

        return hookChain->callChain(
            [this](nstd::observer_ptr<Engine::IEdict> pEntity, std::string_view pszName, std::string_view pszAddress,
                   std::string &szRejectReason)
            {
                constexpr const std::size_t REASON_REJECT_MAX_LEN = 128;
                if (szRejectReason.capacity() < REASON_REJECT_MAX_LEN)
                {
                    szRejectReason.resize(REASON_REJECT_MAX_LEN);
                }
                return m_gameLibDllFunctions->pfnClientConnect(static_cast<edict_t *>(*pEntity), pszName.data(),
                                                               pszAddress.data(), szRejectReason.data()) == TRUE;
            },
            pEntity, pszName, pszAddress, szRejectReason);
    }

    void Library::pfnClientPutInServer(nstd::observer_ptr<Engine::IEdict> pEntity, FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_gameLibDllFunctions->pfnClientPutInServer(static_cast<edict_t *>(*pEntity));
        }

        static auto hookChain = m_hooks->clientPutinServer();

        hookChain->callChain(
            [this](nstd::observer_ptr<Engine::IEdict> pEntity)
            {
                m_gameLibDllFunctions->pfnClientPutInServer(static_cast<edict_t *>(*pEntity));
            },
            pEntity);
    }

    void Library::pfnClientCommand(nstd::observer_ptr<Engine::IEdict> pEntity, FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_gameLibDllFunctions->pfnClientCommand(static_cast<edict_t *>(*pEntity));
        }

        static auto hookChain = m_hooks->clientCmd();

        hookChain->callChain(
            [this](nstd::observer_ptr<Engine::IEdict> pEntity)
            {
                m_gameLibDllFunctions->pfnClientCommand(static_cast<edict_t *>(*pEntity));
            },
            pEntity);
    }

    void Library::pfnClientUserInfoChanged(nstd::observer_ptr<Engine::IEdict> pEntity,
                                           Engine::InfoBuffer infobuffer,
                                           FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_gameLibDllFunctions->pfnClientUserInfoChanged(static_cast<edict_t *>(*pEntity), infobuffer);
        }

        static auto hookChain = m_hooks->clientInfoChanged();

        hookChain->callChain(
            [this](nstd::observer_ptr<Engine::IEdict> pEntity, Engine::InfoBuffer infobuffer)
            {
                m_gameLibDllFunctions->pfnClientUserInfoChanged(static_cast<edict_t *>(*pEntity), infobuffer);
            },
            pEntity, infobuffer);
    }

    void Library::pfnServerActivate(std::uint32_t edictCount, std::uint32_t clientMax, FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_gameLibDllFunctions->pfnServerActivate(m_edictList, static_cast<int>(edictCount),
                                                            static_cast<int>(clientMax));
        }

        static auto hookChain = m_hooks->serverActivate();

        hookChain->callChain(
            [this](std::uint32_t edictCount, std::uint32_t clientMax)
            {
                m_gameLibDllFunctions->pfnServerActivate(m_edictList, static_cast<int>(edictCount),
                                                         static_cast<int>(clientMax));
            },
            edictCount, clientMax);
    }

    void Library::pfnServerDeactivate(FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_gameLibDllFunctions->pfnServerDeactivate();
        }

        static auto hookChain = m_hooks->serverDeactivate();

        hookChain->callChain(
            [this]()
            {
                m_gameLibDllFunctions->pfnServerDeactivate();
            });
    }

    void Library::pfnStartFrame(FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_gameLibDllFunctions->pfnStartFrame();
        }

        static auto hookChain = m_hooks->startFrame();

        hookChain->callChain(
            [this]()
            {
                m_gameLibDllFunctions->pfnStartFrame();
            });
    }

    void Library::pfnGameShutdown(FuncCallType callType)
    {
        auto gameDLLFn = [this]()
        {
            if (m_gameLibNewDllFunctions->pfnGameShutdown)
            {
                m_gameLibNewDllFunctions->pfnGameShutdown();
            }
        };

        if (callType == FuncCallType::Direct)
        {
            return gameDLLFn();
        }

        static auto hookChain = m_hooks->gameShutdown();

        hookChain->callChain(
            [gameDLLFn]()
            {
                return gameDLLFn();
            });
    }

    void Library::pfnCvarValue(nstd::observer_ptr<Engine::IEdict> player, std::string_view value, FuncCallType callType)
    {
        auto gameDLLFn = [this](nstd::observer_ptr<Engine::IEdict> player, std::string_view value)
        {
            if (m_gameLibNewDllFunctions->pfnCvarValue)
            {
                m_gameLibNewDllFunctions->pfnCvarValue(static_cast<edict_t *>(*player), value.data());
            }
        };

        if (callType == FuncCallType::Direct)
        {
            return gameDLLFn(player, value);
        }

        static auto hookChain = m_hooks->cvarValue();

        hookChain->callChain(
            [gameDLLFn](nstd::observer_ptr<Engine::IEdict> player, std::string_view value)
            {
                return gameDLLFn(player, value);
            },
            player, value);
    }

    void Library::pfnCvarValue2(nstd::observer_ptr<Engine::IEdict> player,
                                std::uint32_t requestID,
                                std::string_view cvarName,
                                std::string_view value,
                                FuncCallType callType)
    {
        auto gameDLLFn = [this](nstd::observer_ptr<Engine::IEdict> player, std::uint32_t requestID,
                                std::string_view cvarName, std::string_view value)
        {
            if (m_gameLibNewDllFunctions->pfnCvarValue2)
            {
                m_gameLibNewDllFunctions->pfnCvarValue2(static_cast<edict_t *>(*player), static_cast<int>(requestID),
                                                        cvarName.data(), value.data());
            }
        };

        if (callType == FuncCallType::Direct)
        {
            return gameDLLFn(player, requestID, cvarName, value);
        }

        static auto hookChain = m_hooks->cvarValue2();

        hookChain->callChain(
            [gameDLLFn](nstd::observer_ptr<Engine::IEdict> player, std::uint32_t requestID, std::string_view cvarName,
                        std::string_view value)
            {
                return gameDLLFn(player, requestID, cvarName, value);
            },
            player, requestID, cvarName, value);
    }

    Module::SystemHandle Library::getSystemHandle() const
    {
        return static_cast<Module::SystemHandle>(*m_gameLibrary);
    }

    nstd::observer_ptr<IBaseEntity> Library::getBaseEntity(nstd::observer_ptr<Engine::IEdict> edict)
    {
        return m_entityHolder->getBaseEntity(edict);
    }

    nstd::observer_ptr<IBasePlayer> Library::getBasePlayer(nstd::observer_ptr<Engine::IEdict> edict)
    {
        std::uint32_t idx = edict->getIndex();
        if (idx > m_maxClients || !idx)
        {
            return {};
        }

        return m_entityHolder->getBasePlayer(edict);
    }

    void Library::setMaxClients(std::uint32_t maxPlayers)
    {
        m_maxClients = maxPlayers;
    }

    nstd::observer_ptr<IBasePlayerHooks> Library::getCBasePlayerHooks()
    {
        return m_basePlayerHooks;
    }

    void Library::initVFuncHooks()
    {
        std::filesystem::path mmDllPath = gAnubisApi->getPath(PathType::Anubis) / "dlls";

#if defined __linux__
        mmDllPath /= knownGames[static_cast<std::size_t>(m_modType)].libEntityNameLinux;
#elif defined _WIN32
        mmDllPath /= knownGames[static_cast<std::size_t>(m_modType)].libEntityNameWin32;
#endif

        _initGameEntityDLL(std::move(mmDllPath));
    }

    void Library::_initGameEntityDLL(std::filesystem::path &&path)
    {
        if (path.empty())
        {
            m_logger->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Cannot find entity library for {}",
                             m_gameDir.filename().string());
            m_logger->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Virtual func hooking is unavailable");
            m_logger->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "GameRules are unavailable");
            return;
        }

        try
        {
            auto entityLib = std::make_unique<Module>(IPlugin::Type::EntityDLL, std::move(path));

            if (!entityLib->initPlugin(gAnubisApi))
            {
                throw std::runtime_error("Cannot initialize entity library");
            }

            entityLib->setupHook(SetupHookType::EntityHolder,
                                 [this](std::any entHolder)
                                 {
                                     m_entityHolder = std::any_cast<nstd::observer_ptr<IEntityHolder>>(entHolder);
                                 });

            entityLib->setupHook(SetupHookType::BasePlayerHooks,
                                 [this](std::any basePlrHooks)
                                 {
                                     m_basePlayerHooks =
                                         std::any_cast<nstd::observer_ptr<IBasePlayerHooks>>(basePlrHooks);
                                 });

            entityLib->setupHook(SetupHookType::GameRules,
                                 [this](std::any gameRules)
                                 {
                                     if (auto rules = std::any_cast<nstd::observer_ptr<IRules>>(gameRules); rules)
                                     {
                                         m_rules = rules;
                                     }
                                 });

            entityLib->installVFHooks();

            m_entityLibrary = std::move(entityLib);

            gAnubisApi->installVFHooksForPlugins();
        }
        catch (const std::runtime_error &e)
        {
            m_logger->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Cannot load entity library. Reason: {}",
                             e.what());
            m_logger->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "Virtual func hooking is unavailable");
            m_logger->logMsg(LogLevel::Warning, LogDest::ConsoleFile, "GameRules are unavailable");
        }
    }

    void Library::freeEntitiesDLL()
    {
        if (m_entityLibrary)
        {
            m_entityLibrary->deinitPlugin();
        }
    }

    void Library::setEdictList(edict_t *edictList)
    {
        m_edictList = edictList;
    }

    nstd::observer_ptr<IBaseEntity> Library::allocEntity(nstd::observer_ptr<Engine::IEdict> edict) const
    {
        if (m_entityHolder)
            return m_entityHolder->allocEntity(edict);

        return {};
    }

    nstd::observer_ptr<IBaseEntity> Library::getBaseEntity(edict_t *entity) const
    {
        if (m_entityHolder)
            return m_entityHolder->getBaseEntity(entity);

        return {};
    }

    nstd::observer_ptr<IRules> Library::getRules() const
    {
        return m_rules;
    }
} // namespace Anubis::Game
