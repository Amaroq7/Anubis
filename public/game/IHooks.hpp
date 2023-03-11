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

#include "../observer_ptr.hpp"
#include "../IHookChains.hpp"
#include "../engine/Common.hpp"

#include <string>

namespace Anubis::Engine
{
    class IEdict;
}

namespace Anubis::Game
{
    namespace CStrike
    {
        class IHooks;
    }

    using IGameInitHook = IHook<void>;
    using IGameInitHookRegistry = IHookRegistry<void>;

    using ISpawnHook = IHook<std::int32_t, nstd::observer_ptr<Engine::IEdict>>;
    using ISpawnHookRegistry = IHookRegistry<std::int32_t, nstd::observer_ptr<Engine::IEdict>>;

    using IClientConnectHook = IHook<bool,
                                     nstd::observer_ptr<Engine::IEdict>,
                                     std::string_view,
                                     std::string_view,
                                     nstd::observer_ptr<std::string>>;
    using IClientConnectHookRegistry = IHookRegistry<bool,
                                                     nstd::observer_ptr<Engine::IEdict>,
                                                     std::string_view,
                                                     std::string_view,
                                                     nstd::observer_ptr<std::string>>;

    using IClientPutinServerHook = IHook<void, nstd::observer_ptr<Engine::IEdict>>;
    using IClientPutinServerHookRegistry = IHookRegistry<void, nstd::observer_ptr<Engine::IEdict>>;

    using IClientCmdHook = IHook<void, nstd::observer_ptr<Engine::IEdict>>;
    using IClientCmdHookRegistry = IHookRegistry<void, nstd::observer_ptr<Engine::IEdict>>;

    using IClientInfoChangedHook = IHook<void, nstd::observer_ptr<Engine::IEdict>, Engine::InfoBuffer>;
    using IClientInfoChangedHookRegistry = IHookRegistry<void, nstd::observer_ptr<Engine::IEdict>, Engine::InfoBuffer>;

    using IServerActivateHook = IHook<void, std::uint32_t, std::uint32_t>;
    using IServerActivateHookRegistry = IHookRegistry<void, std::uint32_t, std::uint32_t>;

    using IServerDeactivateHook = IHook<void>;
    using IServerDeactivateHookRegistry = IHookRegistry<void>;

    using IStartFrameHook = IHook<void>;
    using IStartFrameHookRegistry = IHookRegistry<void>;

    using IGameShutdownHook = IHook<void>;
    using IGameShutdownHookRegistry = IHookRegistry<void>;

    using ICvarValueHook = IHook<void, nstd::observer_ptr<Engine::IEdict>, std::string_view>;
    using ICvarValueHookRegistry = IHookRegistry<void, nstd::observer_ptr<Engine::IEdict>, std::string_view>;

    using ICvarValue2Hook =
        IHook<void, nstd::observer_ptr<Engine::IEdict>, std::uint32_t, std::string_view, std::string_view>;
    using ICvarValue2HookRegistry =
        IHookRegistry<void, nstd::observer_ptr<Engine::IEdict>, std::uint32_t, std::string_view, std::string_view>;

    using IClientDisconnectHook = IHook<void, nstd::observer_ptr<Engine::IEdict>>;
    using IClientDisconnectHookRegistry = IHookRegistry<void, nstd::observer_ptr<Engine::IEdict>>;

    class IHooks
    {
    public:
        virtual ~IHooks() = default;

        virtual nstd::observer_ptr<CStrike::IHooks> CSHooks() = 0;

        virtual nstd::observer_ptr<IGameInitHookRegistry> gameInit() = 0;
        virtual nstd::observer_ptr<ISpawnHookRegistry> spawn() = 0;
        virtual nstd::observer_ptr<IClientConnectHookRegistry> clientConnect() = 0;
        virtual nstd::observer_ptr<IClientPutinServerHookRegistry> clientPutinServer() = 0;
        virtual nstd::observer_ptr<IClientCmdHookRegistry> clientCmd() = 0;
        virtual nstd::observer_ptr<IClientInfoChangedHookRegistry> clientInfoChanged() = 0;
        virtual nstd::observer_ptr<IServerActivateHookRegistry> serverActivate() = 0;
        virtual nstd::observer_ptr<IServerDeactivateHookRegistry> serverDeactivate() = 0;
        virtual nstd::observer_ptr<IStartFrameHookRegistry> startFrame() = 0;
        virtual nstd::observer_ptr<IGameShutdownHookRegistry> gameShutdown() = 0;
        virtual nstd::observer_ptr<ICvarValueHookRegistry> cvarValue() = 0;
        virtual nstd::observer_ptr<ICvarValue2HookRegistry> cvarValue2() = 0;
        virtual nstd::observer_ptr<IClientDisconnectHookRegistry> clientDisconnect() = 0;
    };
} // namespace Anubis::Game
