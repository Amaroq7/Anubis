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

#include "AnubisExports.hpp"
#include "EntitiesHooks.hpp"
#include "EntityHolder.hpp"

#include <engine/IEdict.hpp>
#include <utility>

#include "ReGameHeaders.hpp"

nstd::observer_ptr<Anubis::Game::ILibrary> gGameLib;
nstd::observer_ptr<Anubis::Engine::ILibrary> gEngineLib;
nstd::observer_ptr<Anubis::IAnubis> gAnubisAPI;
std::unique_ptr<Anubis::ILogger> gLogger;
nstd::observer_ptr<IReGameApi> gReGameAPI;
nstd::observer_ptr<CSysModule> gGameModule;
std::unique_ptr<Anubis::Game::CStrike::Plugin> gPluginInfo;

static bool initReGameDLL_API()
{
    std::filesystem::path regameDllPath = gAnubisAPI->getPath(Anubis::PathType::Game) / "dlls";
#if defined __linux__
    regameDllPath /= "cs.so";
#elif defined _WIN32
    regameDllPath /= "mp.dll";
#endif

#if defined __linux__
    gGameModule = Sys_LoadModule(regameDllPath.c_str());
#elif defined _WIN32
    gGameModule = Sys_LoadModule(regameDllPath.string().c_str());
#endif

    CreateInterfaceFn ifaceFactory = Sys_GetFactory(gGameModule.get());
    if (!ifaceFactory)
    {
        gLogger->logMsg(Anubis::LogDest::ConsoleFile, Anubis::LogLevel::Error, "Failed to locate engine module");
        return false;
    }

    auto reGameAPI = reinterpret_cast<IReGameApi *>(ifaceFactory(VRE_GAMEDLL_API_VERSION, nullptr));
    if (!reGameAPI)
    {
        gLogger->logMsg(Anubis::LogDest::ConsoleFile, Anubis::LogLevel::Error,
                        "Failed to locate interface factory in ReGameDLL");
        return false;
    }

    std::uint32_t majorVersion = reGameAPI->GetMajorVersion();
    std::uint32_t minorVersion = reGameAPI->GetMinorVersion();

    if (majorVersion != REGAMEDLL_API_VERSION_MAJOR)
    {
        using namespace std::string_literals;
        gLogger->logMsg(Anubis::LogDest::ConsoleFile, Anubis::LogLevel::Error,
                        "ReGameDLL API major version mismatch. "s + "Expected " +
                            std::to_string(REGAMEDLL_API_VERSION_MAJOR) + " got " + std::to_string(majorVersion));
        return false;
    }

    if (minorVersion < REGAMEDLL_API_VERSION_MINOR)
    {
        using namespace std::string_literals;
        gLogger->logMsg(Anubis::LogDest::ConsoleFile, Anubis::LogLevel::Error,
                        "ReGameDLL API minor version mismatch. "s + "Expected at least " +
                            std::to_string(REGAMEDLL_API_VERSION_MINOR) + " got " + std::to_string(minorVersion));
        return false;
    }

    if (!reGameAPI->BGetICSEntity(CSENTITY_API_INTERFACE_VERSION))
    {
        using namespace std::string_literals;
        gLogger->logMsg(Anubis::LogDest::ConsoleFile, Anubis::LogLevel::Error,
                        "Interface CCSEntity API version '"s + CSENTITY_API_INTERFACE_VERSION + "' not found");
        return false;
    }

    if (!reGameAPI->BGetIGameRules(GAMERULES_API_INTERFACE_VERSION))
    {
        using namespace std::string_literals;
        gLogger->logMsg(Anubis::LogDest::ConsoleFile, Anubis::LogLevel::Error,
                        "Interface CGameRules API version '"s + GAMERULES_API_INTERFACE_VERSION + "' not found");
        return false;
    }

    reGameAPI->GetHookchains()->InstallGameRules()->registerHook(
        [](IReGameHook_InstallGameRules *chain)
        {
            nstd::observer_ptr<CGameRules> gameRules = chain->callNext();
            gPluginInfo->execHook(Anubis::Game::SetupHookType::GameRules, gPluginInfo->createGameRules(gameRules));
            return gameRules.get();
        },
        HookChainPriority::HC_PRIORITY_DEFAULT);

    gReGameAPI = reGameAPI;
    return true;
}

namespace Anubis
{
    nstd::observer_ptr<IPlugin> Query()
    {
        gPluginInfo = std::make_unique<Anubis::Game::CStrike::Plugin>();
        return gPluginInfo;
    }

    bool Init(nstd::observer_ptr<IAnubis> api)
    {
        gAnubisAPI = api;
        gGameLib = api->getGame(Game::ILibrary::VERSION);
        gEngineLib = api->getEngine(Engine::ILibrary::VERSION);
        gLogger = api->getLogger(ILogger::VERSION);
        gLogger->setLogTag("CSTRIKE API");

        if (!initReGameDLL_API())
        {
            return false;
        }

        gPluginInfo->execHook(Game::SetupHookType::EntityHolder,
                              nstd::make_observer<Game::IEntityHolder>(Game::CStrike::getEntityHolder()));

        return true;
    }

    void InstallVHooks()
    {
        // init vfunc stuff
        gPluginInfo->execHook(Game::SetupHookType::BasePlayerHooks,
                              nstd::make_observer<Game::IBasePlayerHooks>(Game::CStrike::getBasePlayerHooks()));
    }

    void Shutdown()
    {
        gLogger.reset();
        gGameModule.reset();
    }

    namespace Game
    {
        void SetupHook(SetupHookType setupHookType, std::function<void(std::any)> hook)
        {
            return gPluginInfo->addHook(setupHookType, std::move(hook));
        }
    } // namespace Game
} // namespace Anubis