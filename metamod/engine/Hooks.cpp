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

#include "Hooks.hpp"

#include <osconfig.h>
#include <extdll.h>
#include <util.h>
#include <usercmd.h>
#include "ReHooks.hpp"

namespace Metamod::Engine
{
    Hooks::Hooks(IRehldsHookchains *rehldsHooks) :
          m_svDropClientRegistry([rehldsHooks]() {
              rehldsHooks->SV_DropClient()->registerHook(ReHooks::SV_DropClientHook);
          }, [rehldsHooks]() {
              rehldsHooks->SV_DropClient()->unregisterHook(ReHooks::SV_DropClientHook);
          }),
          m_cvarDirectSet([rehldsHooks]() {
              rehldsHooks->Cvar_DirectSet()->registerHook(ReHooks::Cvar_DirectSetHook);
          }, [rehldsHooks]() {
              rehldsHooks->Cvar_DirectSet()->unregisterHook(ReHooks::Cvar_DirectSetHook);
          })
    {}

    PrecacheModelHookRegistry *Hooks::precacheModel()
    {
        return &m_precacheModelRegistry;
    }

    PrecacheSoundHookRegistry *Hooks::precacheSound()
    {
        return &m_precacheSoundRegistry;
    }

    PrecacheGenericHookRegistry *Hooks::precacheGeneric()
    {
        return &m_precacheGeneric;
    }

    ChangeLevelHookRegistry *Hooks::changeLevel()
    {
        return &m_changeLevelRegistry;
    }

    SrvCmdHookRegistry *Hooks::srvCmd()
    {
        return &m_srvCmdRegistry;
    }

    SrvExecHookRegistry *Hooks::srvExec()
    {
        return &m_srvExecRegistry;
    }

    RegSrvCmdHookRegistry *Hooks::regSrvCmd()
    {
        return &m_regSrvCmdRegistry;
    }

    MessageBeginHookRegistry *Hooks::messageBegin()
    {
        return &m_messageBeginRegistry;
    }

    MessageEndHookRegistry *Hooks::messageEnd()
    {
        return &m_messageEndRegistry;
    }

    WriteByteHookRegistry *Hooks::writeByte()
    {
        return &m_writeByteRegistry;
    }

    WriteCharHookRegistry *Hooks::writeChar()
    {
        return &m_writeCharRegistry;
    }

    WriteShortHookRegistry *Hooks::writeShort()
    {
        return &m_writeShortRegistry;
    }

    WriteLongHookRegistry *Hooks::writeLong()
    {
        return &m_writeLongRegistry;
    }

    WriteEntityHookRegistry *Hooks::writeEntity()
    {
        return &m_writeEntityRegistry;
    }

    WriteAngleHookRegistry *Hooks::writeAngle()
    {
        return &m_writeAngleRegistry;
    }

    WriteCoordHookRegistry *Hooks::writeCoord()
    {
        return &m_writeCoordRegistry;
    }

    WriteStringHookRegistry *Hooks::writeString()
    {
        return &m_writeStringRegistry;
    }

    RegUserMsgHookRegistry *Hooks::regUserMsg()
    {
        return &m_regUserMsgRegistry;
    }
    
    GetPlayerAuthIDHookRegistry *Hooks::getPlayerAuthID()
    {
        return &m_getPlayerAuthID;
    }
    
    GetPlayerUserIDHookRegistry *Hooks::getPlayerUserID()
    {
        return &m_getPlayerUserID;
    }

    SVDropClientHookRegistry *Hooks::svDropClient()
    {
        return &m_svDropClientRegistry;
    }

    CvarDirectSetHookRegistry *Hooks::cvarDirectSet()
    {
        return &m_cvarDirectSet;
    }
    
    InfoKeyValueHookRegistry *Hooks::infoKeyValue()
    {
        return &m_infoKeyValue;
    }
    
    CmdArgvHookRegistry *Hooks::cmdArgv()
    {
        return &m_cmdArgv;
    }
    
    CmdArgsHookRegistry *Hooks::cmdArgs()
    {
        return &m_cmdArgs;
    }
    
    CmdArgcHookRegistry *Hooks::cmdArgc()
    {
        return &m_cmdArgc;
    }

    RegisterCvarHookRegistry *Hooks::registerCvar()
    {
        return &m_registerCvar;
    }

    GetCvarHookRegistry *Hooks::getCvar()
    {
        return &m_getCvar;
    }

    SetModelHookRegistry *Hooks::setModel()
    {
        return &m_setModel;
    }

    CreateEntityHookRegistry *Hooks::createEntity()
    {
        return &m_createEntity;
    }

    RemoveEntityHookRegistry *Hooks::removeEntity()
    {
        return &m_removeEntity;
    }

    AlertHookRegistry *Hooks::alert()
    {
        return &m_alert;
    }

    ServerPrintHookRegistry *Hooks::serverPrint()
    {
        return &m_serverPrint;
    }

    IsDedicatedHookRegistry *Hooks::isDedicated()
    {
        return &m_isDedicated;
    }

    CheckEngParmHookRegistry *Hooks::checkEngParm()
    {
        return &m_checkEngParm;
    }

    QueryClientCvarValueHookRegistry *Hooks::queryClientCvarValue()
    {
        return &m_queryClientCvarValue;
    }

    QueryClientCvarValue2HookRegistry *Hooks::queryClientCvarValue2()
    {
        return &m_queryClientCvarValue2;
    }
}
