/*
 *  Copyright (C) 2020 Metamod++ Development Team
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

#pragma once

#include <game/IHooks.hpp>
#include <HookChains.hpp>

namespace Metamod::Game
{
    using GameInitHook = Hook<void>;
    using GameInitHookRegistry = HookRegistry<void>;

    using SpawnHook = Hook<std::int32_t, Engine::IEdict *>;
    using SpawnHookRegistry = HookRegistry<std::int32_t, Engine::IEdict *>;

    using ClientConnectHook = Hook<bool, Engine::IEdict *, std::string_view, std::string_view, std::string &>;
    using ClientConnectHookRegistry = HookRegistry<bool, Engine::IEdict *, std::string_view, std::string_view, std::string &>;

    using ClientPutinServerHook = Hook<void, Engine::IEdict *>;
    using ClientPutinServerHookRegistry = HookRegistry<void, Engine::IEdict *>;

    using ClientCmdHook = Hook<void, Engine::IEdict *>;
    using ClientCmdHookRegistry = HookRegistry<void, Engine::IEdict *>;

    using ClientInfoChangedHook = Hook<void, Engine::IEdict *, Engine::InfoBuffer>;
    using ClientInfoChangedHookRegistry = HookRegistry<void, Engine::IEdict *, Engine::InfoBuffer>;

    using ServerActivateHook = Hook<void, std::uint32_t, std::uint32_t>;
    using ServerActivateHookRegistry = HookRegistry<void, std::uint32_t, std::uint32_t>;

    using ServerDeactivateHook = Hook<void>;
    using ServerDeactivateHookRegistry = HookRegistry<void>;

    using StartFrameHook = Hook<void>;
    using StartFrameHookRegistry = HookRegistry<void>;

    using GameShutdownHook = Hook<void>;
    using GameShutdownHookRegistry = HookRegistry<void>;

    using CvarValueHook = Hook<void, const Engine::IEdict *, std::uint32_t, std::string_view, std::string_view>;
    using CvarValueHookRegistry = HookRegistry<void, const Engine::IEdict *, std::uint32_t, std::string_view, std::string_view>;

    class Hooks final : public IHooks
    {
    public:
        Hooks() = default;
        ~Hooks() override = default;

        GameInitHookRegistry *gameInit() final;
        SpawnHookRegistry *spawn() final;
        ClientConnectHookRegistry *clientConnect() final;
        ClientPutinServerHookRegistry *clientPutinServer() final;
        ClientCmdHookRegistry *clientCmd() final;
        ClientInfoChangedHookRegistry *clientInfoChanged() final;
        ServerActivateHookRegistry *serverActivate() final;
        ServerDeactivateHookRegistry *serverDeactivate() final;
        StartFrameHookRegistry *startFrame() final;
        GameShutdownHookRegistry *gameShutdown() final;
        CvarValueHookRegistry *cvarValue() final;

    private:
        GameInitHookRegistry m_gameInitRegistry;
        SpawnHookRegistry m_spawn;
        ClientConnectHookRegistry m_clientConnectRegistry;
        ClientPutinServerHookRegistry m_clientPutinServerRegistry;
        ClientCmdHookRegistry m_clientCmdRegistry;
        ClientInfoChangedHookRegistry m_clientInfoChangedRegistry;
        ServerActivateHookRegistry m_serverActivateRegistry;
        ServerDeactivateHookRegistry m_serverDeactivateRegistry;
        StartFrameHookRegistry m_startFrameRegistry;
        GameShutdownHookRegistry m_gameShutdownRegistry;
        CvarValueHookRegistry m_cvarValueRegistry;
    };
}
