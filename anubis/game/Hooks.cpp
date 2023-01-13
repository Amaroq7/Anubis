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

#include "Hooks.hpp"

namespace Anubis::Game
{
    Hooks::Hooks()
        : m_gameInitRegistry(std::make_unique<GameInitHookRegistry>()),
          m_spawnRegistry(std::make_unique<SpawnHookRegistry>()),
          m_clientConnectRegistry(std::make_unique<ClientConnectHookRegistry>()),
          m_clientPutinServerRegistry(std::make_unique<ClientPutinServerHookRegistry>()),
          m_clientCmdRegistry(std::make_unique<ClientCmdHookRegistry>()),
          m_clientInfoChangedRegistry(std::make_unique<ClientInfoChangedHookRegistry>()),
          m_serverActivateRegistry(std::make_unique<ServerActivateHookRegistry>()),
          m_serverDeactivateRegistry(std::make_unique<ServerDeactivateHookRegistry>()),
          m_startFrameRegistry(std::make_unique<StartFrameHookRegistry>()),
          m_gameShutdownRegistry(std::make_unique<GameShutdownHookRegistry>()),
          m_cvarValueRegistry(std::make_unique<CvarValueHookRegistry>()),
          m_cvarValue2Registry(std::make_unique<CvarValue2HookRegistry>()),
          m_clientDisconnectHookRegistry(std::make_unique<ClientDisconnectHookRegistry>())
    {
    }

    nstd::observer_ptr<IGameInitHookRegistry> Hooks::gameInit()
    {
        return m_gameInitRegistry;
    }

    nstd::observer_ptr<ISpawnHookRegistry> Hooks::spawn()
    {
        return m_spawnRegistry;
    }

    nstd::observer_ptr<IClientConnectHookRegistry> Hooks::clientConnect()
    {
        return m_clientConnectRegistry;
    }

    nstd::observer_ptr<IClientPutinServerHookRegistry> Hooks::clientPutinServer()
    {
        return m_clientPutinServerRegistry;
    }

    nstd::observer_ptr<IClientCmdHookRegistry> Hooks::clientCmd()
    {
        return m_clientCmdRegistry;
    }

    nstd::observer_ptr<IClientInfoChangedHookRegistry> Hooks::clientInfoChanged()
    {
        return m_clientInfoChangedRegistry;
    }

    nstd::observer_ptr<IServerActivateHookRegistry> Hooks::serverActivate()
    {
        return m_serverActivateRegistry;
    }

    nstd::observer_ptr<IServerDeactivateHookRegistry> Hooks::serverDeactivate()
    {
        return m_serverDeactivateRegistry;
    }

    nstd::observer_ptr<IStartFrameHookRegistry> Hooks::startFrame()
    {
        return m_startFrameRegistry;
    }

    nstd::observer_ptr<IGameShutdownHookRegistry> Hooks::gameShutdown()
    {
        return m_gameShutdownRegistry;
    }

    nstd::observer_ptr<ICvarValueHookRegistry> Hooks::cvarValue()
    {
        return m_cvarValueRegistry;
    }

    nstd::observer_ptr<ICvarValue2HookRegistry> Hooks::cvarValue2()
    {
        return m_cvarValue2Registry;
    }

    nstd::observer_ptr<IClientDisconnectHookRegistry> Hooks::clientDisconnect()
    {
        return m_clientDisconnectHookRegistry;
    }
} // namespace Anubis::Game
