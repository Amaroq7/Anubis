/*
 *  Copyright (C) 2020 Metamod++ Development Team
 *
 *  This file is part of Metamod++.
 *
 *  Metamod++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option)any later version.

 *  Metamod++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Metamod++.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <gamelib/Funcs.hpp>
#include <gamelib/GameLib.hpp>
#include <Metamod.hpp>
#include <gamelib/Callbacks.hpp>

namespace Metamod::GameLib
{
    Funcs::Funcs(bool direct) : m_direct(direct) {}
    void Funcs::pfnGameInit()
    {
        static GameInitHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->gameInit();
        if (m_direct)
        {
            Callbacks::Direct::pfnGameInit();
        }

        hookchain->callChain(Callbacks::Direct::pfnGameInit);
    }

    bool Funcs::pfnClientConnect(Engine::IEdict *pEntity, std::string_view pszName, std::string_view pszAddress, std::string &szRejectReason)
    {
        static ClientConnectHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->clientConnect();
        if (m_direct)
        {
            return Callbacks::Direct::pfnClientConnect(pEntity, pszName, pszAddress, szRejectReason);
        }

        return hookchain->callChain(Callbacks::Direct::pfnClientConnect, pEntity, pszName, pszAddress, szRejectReason);
    }

    void Funcs::pfnClientPutInServer(Engine::IEdict *pEntity)
    {
        static ClientPutinServerHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->clientPutinServer();
        if (m_direct)
        {
            Callbacks::Direct::pfnClientPutInServer(pEntity);
        }

        hookchain->callChain(Callbacks::Direct::pfnClientPutInServer, pEntity);
    }

    void Funcs::pfnClientCommand(Engine::IEdict *pEntity)
    {
        static ClientCmdHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->clientCmd();
        if (m_direct)
        {
            Callbacks::Direct::pfnClientCommand(pEntity);
        }

        hookchain->callChain(Callbacks::Direct::pfnClientCommand, pEntity);
    }

    void Funcs::pfnClientUserInfoChanged(Engine::IEdict *pEntity, char *infobuffer)
    {
        static ClientInfoChangedHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->clientInfoChanged();
        if (m_direct)
        {
            Callbacks::Direct::pfnClientUserInfoChanged(pEntity, infobuffer);
        }

        hookchain->callChain(Callbacks::Direct::pfnClientUserInfoChanged, pEntity, infobuffer);
    }

    void Funcs::pfnServerActivate(std::uint32_t edictCount, std::uint32_t clientMax)
    {
        static ServerActivateHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->serverActivate();
        if (m_direct)
        {
            Callbacks::Direct::pfnServerActivate(edictCount, clientMax);
        }

        hookchain->callChain(Callbacks::Direct::pfnServerActivate, edictCount, clientMax);
    }

    void Funcs::pfnServerDeactivate()
    {
        static ServerDeactivateHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->serverDeactivate();
        if (m_direct)
        {
            Callbacks::Direct::pfnServerDeactivate();
        }

        hookchain->callChain(Callbacks::Direct::pfnServerDeactivate);
    }

    void Funcs::pfnStartFrame()
    {
        static StartFrameHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->startFrame();
        if (m_direct)
        {
            Callbacks::Direct::pfnStartFrame();
        }

        hookchain->callChain(Callbacks::Direct::pfnStartFrame);
    }

    void Funcs::pfnGameShutdown()
    {
        static GameShutdownHookRegistry *hookchain = gMetaGlobal->getGameLib()->getHooks()->gameShutdown();
        if (m_direct)
        {
            Callbacks::Direct::pfnGameShutdown();
        }

        hookchain->callChain(Callbacks::Direct::pfnGameShutdown);
    }
}