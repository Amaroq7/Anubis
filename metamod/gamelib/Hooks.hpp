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

#include <gamelib/IHooks.hpp>
#include <HookChains.hpp>

namespace Metamod::GameLib
{
    using GameInitHook = Hook<void>;
    using GameInitHookRegistry = HookRegistry<void>;

    using ClientConnectHook = Hook<bool, Engine::IEdict *, std::string_view, std::string_view, std::string &>;
    using ClientConnectHookRegistry = HookRegistry<bool, Engine::IEdict *, std::string_view, std::string_view, std::string &>;

    using ClientPutinServerHook = Hook<void, Engine::IEdict *>;
    using ClientPutinServerHookRegistry = HookRegistry<void, Engine::IEdict *>;

    using ClientCmdHook = Hook<void, Engine::IEdict *>;
    using ClientCmdHookRegistry = HookRegistry<void, Engine::IEdict *>;

    using ClientInfoChangedHook = Hook<void, Engine::IEdict *, char *>;
    using ClientInfoChangedHookRegistry = HookRegistry<void, Engine::IEdict *, char *>;

    using ServerActivateHook = Hook<void, std::uint32_t, std::uint32_t>;
    using ServerActivateHookRegistry = HookRegistry<void, std::uint32_t, std::uint32_t>;

    using ServerDeactivateHook = Hook<void>;
    using ServerDeactivateHookRegistry = HookRegistry<void>;

    using StartFrameHook = Hook<void>;
    using StartFrameHookRegistry = HookRegistry<void>;

    using GameShutdownHook = Hook<void>;
    using GameShutdownHookRegistry = HookRegistry<void>;

    class Hooks : public IHooks
    {
    public:
        Hooks() = default;
        ~Hooks() override = default;

        GameInitHookRegistry *gameInit() override;
        ClientConnectHookRegistry *clientConnect() override;
        ClientPutinServerHookRegistry *clientPutinServer() override;
        ClientCmdHookRegistry *clientCmd() override;
        ClientInfoChangedHookRegistry *clientInfoChanged() override;
        ServerActivateHookRegistry *serverActivate() override;
        ServerDeactivateHookRegistry *serverDeactivate() override;
        StartFrameHookRegistry *startFrame() override;
        GameShutdownHookRegistry *gameShutdown() override;

    private:
        GameInitHookRegistry m_gameInitRegistry;
        ClientConnectHookRegistry m_clientConnectRegistry;
        ClientPutinServerHookRegistry m_clientPutinServerRegistry;
        ClientCmdHookRegistry m_clientCmdRegistry;
        ClientInfoChangedHookRegistry m_clientInfoChangedRegistry;
        ServerActivateHookRegistry m_serverActivateRegistry;
        ServerDeactivateHookRegistry m_serverDeactivateRegistry;
        StartFrameHookRegistry m_startFrameRegistry;
        GameShutdownHookRegistry m_gameShutdownRegistry;
    };
}
