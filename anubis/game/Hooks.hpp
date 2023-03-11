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

#pragma once

#include <game/IHooks.hpp>
#include <HookChains.hpp>

namespace Anubis::Game
{
    using GameInitHook = Hook<void>;
    using GameInitHookRegistry = HookRegistry<void>;

    using SpawnHook = Hook<std::int32_t, nstd::observer_ptr<Engine::IEdict>>;
    using SpawnHookRegistry = HookRegistry<std::int32_t, nstd::observer_ptr<Engine::IEdict>>;

    using ClientConnectHook = Hook<bool,
                                   nstd::observer_ptr<Engine::IEdict>,
                                   std::string_view,
                                   std::string_view,
                                   nstd::observer_ptr<std::string>>;
    using ClientConnectHookRegistry = HookRegistry<bool,
                                                   nstd::observer_ptr<Engine::IEdict>,
                                                   std::string_view,
                                                   std::string_view,
                                                   nstd::observer_ptr<std::string>>;

    using ClientPutinServerHook = Hook<void, nstd::observer_ptr<Engine::IEdict>>;
    using ClientPutinServerHookRegistry = HookRegistry<void, nstd::observer_ptr<Engine::IEdict>>;

    using ClientCmdHook = Hook<void, nstd::observer_ptr<Engine::IEdict>>;
    using ClientCmdHookRegistry = HookRegistry<void, nstd::observer_ptr<Engine::IEdict>>;

    using ClientInfoChangedHook = Hook<void, nstd::observer_ptr<Engine::IEdict>, Engine::InfoBuffer>;
    using ClientInfoChangedHookRegistry = HookRegistry<void, nstd::observer_ptr<Engine::IEdict>, Engine::InfoBuffer>;

    using ServerActivateHook = Hook<void, std::uint32_t, std::uint32_t>;
    using ServerActivateHookRegistry = HookRegistry<void, std::uint32_t, std::uint32_t>;

    using ServerDeactivateHook = Hook<void>;
    using ServerDeactivateHookRegistry = HookRegistry<void>;

    using StartFrameHook = Hook<void>;
    using StartFrameHookRegistry = HookRegistry<void>;

    using GameShutdownHook = Hook<void>;
    using GameShutdownHookRegistry = HookRegistry<void>;

    using CvarValueHook = Hook<void, nstd::observer_ptr<Engine::IEdict>, std::string_view>;
    using CvarValueHookRegistry = HookRegistry<void, nstd::observer_ptr<Engine::IEdict>, std::string_view>;

    using CvarValue2Hook =
        Hook<void, nstd::observer_ptr<Engine::IEdict>, std::uint32_t, std::string_view, std::string_view>;
    using CvarValue2HookRegistry =
        HookRegistry<void, nstd::observer_ptr<Engine::IEdict>, std::uint32_t, std::string_view, std::string_view>;

    using ClientDisconnectHook = Hook<void, nstd::observer_ptr<Engine::IEdict>>;
    using ClientDisconnectHookRegistry = HookRegistry<void, nstd::observer_ptr<Engine::IEdict>>;

    class Hooks final : public IHooks
    {
    public:
        Hooks();
        ~Hooks() final = default;

        nstd::observer_ptr<CStrike::IHooks> CSHooks() final;

        nstd::observer_ptr<IGameInitHookRegistry> gameInit() final;
        nstd::observer_ptr<ISpawnHookRegistry> spawn() final;
        nstd::observer_ptr<IClientConnectHookRegistry> clientConnect() final;
        nstd::observer_ptr<IClientPutinServerHookRegistry> clientPutinServer() final;
        nstd::observer_ptr<IClientCmdHookRegistry> clientCmd() final;
        nstd::observer_ptr<IClientInfoChangedHookRegistry> clientInfoChanged() final;
        nstd::observer_ptr<IServerActivateHookRegistry> serverActivate() final;
        nstd::observer_ptr<IServerDeactivateHookRegistry> serverDeactivate() final;
        nstd::observer_ptr<IStartFrameHookRegistry> startFrame() final;
        nstd::observer_ptr<IGameShutdownHookRegistry> gameShutdown() final;
        nstd::observer_ptr<ICvarValueHookRegistry> cvarValue() final;
        nstd::observer_ptr<ICvarValue2HookRegistry> cvarValue2() final;
        nstd::observer_ptr<IClientDisconnectHookRegistry> clientDisconnect() final;

        void initCSHooks(nstd::observer_ptr<CStrike::IHooks> hooks);

    private:
        std::unique_ptr<GameInitHookRegistry> m_gameInitRegistry;
        std::unique_ptr<SpawnHookRegistry> m_spawnRegistry;
        std::unique_ptr<ClientConnectHookRegistry> m_clientConnectRegistry;
        std::unique_ptr<ClientPutinServerHookRegistry> m_clientPutinServerRegistry;
        std::unique_ptr<ClientCmdHookRegistry> m_clientCmdRegistry;
        std::unique_ptr<ClientInfoChangedHookRegistry> m_clientInfoChangedRegistry;
        std::unique_ptr<ServerActivateHookRegistry> m_serverActivateRegistry;
        std::unique_ptr<ServerDeactivateHookRegistry> m_serverDeactivateRegistry;
        std::unique_ptr<StartFrameHookRegistry> m_startFrameRegistry;
        std::unique_ptr<GameShutdownHookRegistry> m_gameShutdownRegistry;
        std::unique_ptr<CvarValueHookRegistry> m_cvarValueRegistry;
        std::unique_ptr<CvarValue2HookRegistry> m_cvarValue2Registry;
        std::unique_ptr<ClientDisconnectHookRegistry> m_clientDisconnectHookRegistry;

    private:
        nstd::observer_ptr<CStrike::IHooks> m_CSHooks;
    };
} // namespace Anubis::Game
