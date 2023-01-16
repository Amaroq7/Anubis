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
#include "VFuncCallbacks.hpp"
#include "Config.hpp"

#include <engine/IEdict.hpp>
#include <game/valve/BasePlayer.hpp>

#include <fmt/format.h>

#include <utility>

nstd::observer_ptr<Anubis::Game::ILibrary> gGameLib;
nstd::observer_ptr<Anubis::Engine::ILibrary> gEngineLib;
nstd::observer_ptr<Anubis::IAnubis> gAnubisAPI;
std::unique_ptr<Anubis::ILogger> gLogger;
std::unique_ptr<Anubis::Game::Valve::Plugin> gPluginInfo;

namespace
{
    void protect(std::intptr_t region, std::int32_t protection)
    {
#if defined _WIN32
        MEMORY_BASIC_INFORMATION mbi;
        VirtualQuery(reinterpret_cast<void *>(region), &mbi, sizeof(mbi));
        VirtualProtect(mbi.BaseAddress, mbi.RegionSize, protection, &mbi.Protect);
#elif defined __linux__
        static std::int32_t pageSize = sysconf(_SC_PAGE_SIZE);
        mprotect(reinterpret_cast<void *>(region & ~(pageSize - 1)), pageSize, protection);
#endif
    }

    std::int32_t unprotect(std::intptr_t region)
    {
#if defined _WIN32
        MEMORY_BASIC_INFORMATION mbi;
        VirtualQuery(reinterpret_cast<void *>(region), &mbi, sizeof(mbi));
        VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);
        return mbi.Protect;
#elif defined __linux__
        static std::int32_t pageSize = sysconf(_SC_PAGE_SIZE);
        mprotect(reinterpret_cast<void *>(region & ~(pageSize - 1)), pageSize, PROT_READ | PROT_WRITE | PROT_EXEC);
        return PROT_READ | PROT_EXEC;
#endif
    }

} // namespace

namespace Anubis
{
    nstd::observer_ptr<IPlugin> Query()
    {
        gPluginInfo = std::make_unique<Game::Valve::Plugin>();
        return gPluginInfo;
    }

    bool Init(nstd::observer_ptr<IAnubis> api)
    {
        gAnubisAPI = api;
        gGameLib = api->getGame(Game::ILibrary::VERSION);
        gEngineLib = api->getEngine(Engine::ILibrary::VERSION);
        gLogger = api->getLogger(ILogger::VERSION);
        gLogger->setLogTag("VALVE API");

        try
        {
            Game::Valve::gConfig = std::make_unique<Game::Valve::Config>(gAnubisAPI->getPath(PathType::Configs),
                                                                         gGameLib->getSystemHandle());
        }
        catch (const std::exception &e)
        {
            gLogger->logMsg(LogDest::ConsoleFile, LogLevel::Warning,
                            fmt::format("Cannot initialize GameConfig: {}", e.what()));
            return false;
        }

        gPluginInfo->execHook(Game::SetupHookType::EntityHolder,
                              nstd::make_observer<Game::IEntityHolder>(Game::Valve::getEntityHolder()));

        return true;
    }

    void InstallVHooks()
    {
        auto getVTable = [](std::string_view className)
        {
            const auto &edict = gEngineLib->createEntity(Anubis::FuncCallType::Direct);
            if (!gGameLib->callGameEntity(className, edict))
            {
                gEngineLib->removeEntity(edict, Anubis::FuncCallType::Direct);
                return 0;
            }

            if (!edict->getPrivateData())
            {
                gEngineLib->removeEntity(edict, Anubis::FuncCallType::Direct);
                return 0;
            }

            auto vTable = *(reinterpret_cast<std::intptr_t *>(edict->getPrivateData()));
            gEngineLib->removeEntity(edict, Anubis::FuncCallType::Direct);

            if (!reinterpret_cast<void *>(vTable))
            {
                return 0;
            }

            return vTable;
        };

        try
        {
            CWorldVTable = getVTable("worldspawn");

            // Hook CWorld::Spawn to get CGameRules
            if (CWorldVTable)
            {
                auto vFunc = CWorldVTable + static_cast<std::intptr_t>(sizeof(std::intptr_t) *
                                                                       Game::Valve::gConfig->getVirtualOffset("spawn"));

                CWorldSpawnOrigFn = *reinterpret_cast<std::intptr_t *>(vFunc);
                std::int32_t memProtection = unprotect(vFunc);
                *(reinterpret_cast<std::intptr_t *>(vFunc)) = reinterpret_cast<intptr_t>(Game::VFunc::vSpawnHook);
                protect(vFunc, memProtection);
            }
        }
        catch (const std::exception &e)
        {
            gLogger->logMsg(LogDest::ConsoleFile, LogLevel::Error, e.what());
            return;
        }

        Game::IBasePlayer::VTable = getVTable(Game::Valve::BasePlayer::CLASS_NAME);
        if (!Game::IBasePlayer::VTable)
        {
            return;
        }

        Game::VFunc::gPevOffset = Game::Valve::gConfig->getVirtualOffset("pev");

        gPluginInfo->execHook(Game::SetupHookType::BasePlayerHooks,
                              nstd::make_observer<Game::IBasePlayerHooks>(Game::Valve::getBasePlayerHooks()));
    }

    void Shutdown() {}

    namespace Game
    {
        void SetupHook(SetupHookType setupHookType, std::function<void(std::any)> hook)
        {
            return gPluginInfo->addHook(setupHookType, std::move(hook));
        }
    } // namespace Game
} // namespace Anubis