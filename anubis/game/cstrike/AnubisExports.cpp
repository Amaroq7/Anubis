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

#include "ReGameHeaders.hpp"

nstd::observer_ptr<Anubis::Game::ILibrary> gGameLib;
nstd::observer_ptr<Anubis::Engine::ILibrary> gEngineLib;
nstd::observer_ptr<Anubis::IAnubis> gAnubisAPI;
std::unique_ptr<Anubis::ILogger> gLogger;
nstd::observer_ptr<IReGameApi> gReGameAPI;
nstd::observer_ptr<CSysModule> gGameModule;

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

    gReGameAPI = reGameAPI;
    return true;
}

namespace Anubis
{
    nstd::observer_ptr<IPlugin> Query()
    {
        static std::unique_ptr<IPlugin> pluginInfo = std::make_unique<EntityLib::CStrike::Plugin>();
        return pluginInfo;
    }

    bool Init(nstd::observer_ptr<IAnubis> api)
    {
        gAnubisAPI = api;
        gGameLib = api->getGame(Game::ILibrary::VERSION);
        gEngineLib = api->getEngine(Engine::ILibrary::VERSION);
        gLogger = api->getLogger(ILogger::VERSION);

        if (!initReGameDLL_API())
        {
            return false;
        }

        return true;
    }

    void InstallVHooks()
    {
        // init vfunc stuff
        Game::CStrike::getBasePlayerHooks();
        Game::CStrike::getEntityHolder();
    }

    void Shutdown()
    {
        gLogger.reset();
        gGameModule.reset();
    }

    namespace Game
    {
        nstd::observer_ptr<IBasePlayerHooks> BasePlayerHooks()
        {
            return CStrike::getBasePlayerHooks();
        }

        nstd::observer_ptr<IEntityHolder> EntityHolder()
        {
            return CStrike::getEntityHolder();
        }
    } // namespace Game
} // namespace Anubis