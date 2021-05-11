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

#include <engine/Common.hpp>
#include <engine/IHooks.hpp>
#include "HookChains.hpp"

namespace Metamod::Engine
{
    using PrecacheModelHook = Hook<PrecacheId, std::string_view>;
    using PrecacheModelHookRegistry = HookRegistry<PrecacheId, std::string_view>;

    using PrecacheSoundHook = Hook<PrecacheId, std::string_view>;
    using PrecacheSoundHookRegistry = HookRegistry<PrecacheId, std::string_view>;

    using ChangeLevelHook = Hook<void, std::string_view, std::string_view>;
    using ChangeLevelHookRegistry = HookRegistry<void, std::string_view, std::string_view>;

    using SrvCmdHook = Hook<void, std::string_view>;
    using SrvCmdHookRegistry = HookRegistry<void, std::string_view>;

    using SrvExecHook = Hook<void>;
    using SrvExecHookRegistry = HookRegistry<void>;

    using RegSrvCmdHook = Hook<void, std::string_view, ServerCmdCallback>;
    using RegSrvCmdHookRegistry = HookRegistry<void, std::string_view, ServerCmdCallback>;

    using MessageBeginHook = Hook<void, MsgDest, MsgType, const float *, IEdict *>;
    using MessageBeginHookRegistry = HookRegistry<void, MsgDest, MsgType, const float *, IEdict *>;

    using MessageEndHook = Hook<void>;
    using MessageEndHookRegistry = HookRegistry<void>;

    using WriteByteHook = Hook<void, std::byte>;
    using WriteByteHookRegistry = HookRegistry<void, std::byte>;

    using WriteCharHook = Hook<void, char>;
    using WriteCharHookRegistry = HookRegistry<void, char>;

    using WriteShortHook = Hook<void, std::int16_t>;
    using WriteShortHookRegistry = HookRegistry<void, std::int16_t>;

    using WriteLongHook = Hook<void, std::int32_t>;
    using WriteLongHookRegistry = HookRegistry<void, std::int32_t>;

    using WriteEntityHook = Hook<void, MsgEntity>;
    using WriteEntityHookRegistry = HookRegistry<void, MsgEntity>;

    using WriteAngleHook = Hook<void, MsgAngle>;
    using WriteAngleHookRegistry = HookRegistry<void, MsgAngle>;

    using WriteCoordHook = Hook<void, MsgCoord>;
    using WriteCoordHookRegistry = HookRegistry<void, MsgCoord>;

    using WriteStringHook = Hook<void, std::string_view>;
    using WriteStringHookRegistry = HookRegistry<void, std::string_view>;

    using RegUserMsgHook = Hook<MsgType, std::string_view, std::int16_t>;
    using RegUserMsgHookRegistry = HookRegistry<MsgType, std::string_view, std::int16_t>;

    using GetPlayerAuthIDHook = Hook<std::string_view, IEdict *>;
    using GetPlayerAuthIDHookRegistry = HookRegistry<std::string_view, IEdict *>;
    
    using GetPlayerUserIDHook = Hook<UserID, IEdict *>;
    using GetPlayerUserIDHookRegistry = HookRegistry<UserID, IEdict *>;

    using SVDropClientHook = Hook<void, IGameClient *, bool, std::string_view>;
    using SVDropClientHookRegistry = HookRegistry<void, IGameClient *, bool, std::string_view>;

    using CvarDirectSetHook = Hook<void, ICvar *, std::string_view>;
    using CvarDirectSetHookRegistry = HookRegistry<void, ICvar *, std::string_view>;
    
    using InfoKeyValueHook = Hook<std::string_view, InfoBuffer, std::string_view>;
    using InfoKeyValueHookRegistry = HookRegistry<std::string_view, InfoBuffer, std::string_view>;
    
    using CmdArgvHook = Hook<std::string_view, std::uint8_t>;
    using CmdArgvHookRegistry = HookRegistry<std::string_view, std::uint8_t>;
    
    using CmdArgsHook = Hook<std::string_view>;
    using CmdArgsHookRegistry = HookRegistry<std::string_view>;
    
    using CmdArgcHook = Hook<std::uint8_t>;
    using CmdArgcHookRegistry = HookRegistry<std::uint8_t>;

    using RegisterCvarHook = Hook<void, std::string_view, std::string_view>;
    using RegisterCvarHookRegistry = HookRegistry<void, std::string_view, std::string_view>;

    using GetCvarHook = Hook<ICvar *, std::string_view>;
    using GetCvarHookRegistry = HookRegistry<ICvar *, std::string_view>;

    using SetModelHook = Hook<void, IEdict *, std::string_view>;
    using SetModelHookRegistry = HookRegistry<void, IEdict *, std::string_view>;

    using CreateEntityHook = Hook<IEdict *>;
    using CreateEntityHookRegistry = HookRegistry<IEdict*>;

    using RemoveEntityHook = Hook<void, IEdict *>;
    using RemoveEntityHookRegistry = HookRegistry<void, IEdict *>;

    class Hooks final : public IHooks
    {
    public:
        ~Hooks() override = default;

        PrecacheModelHookRegistry *precacheModel() override;
        PrecacheSoundHookRegistry *precacheSound() override;
        ChangeLevelHookRegistry *changeLevel() override;
        SrvCmdHookRegistry *srvCmd() override;
        SrvExecHookRegistry *srvExec() override;
        RegSrvCmdHookRegistry *regSrvCmd() override;
        MessageBeginHookRegistry *messageBegin() override;
        MessageEndHookRegistry *messageEnd() override;
        WriteByteHookRegistry *writeByte() override;
        WriteCharHookRegistry *writeChar() override;
        WriteShortHookRegistry *writeShort() override;
        WriteLongHookRegistry *writeLong() override;
        WriteEntityHookRegistry *writeEntity() override;
        WriteAngleHookRegistry *writeAngle() override;
        WriteCoordHookRegistry *writeCoord() override;
        WriteStringHookRegistry *writeString() override;
        RegUserMsgHookRegistry *regUserMsg() override;
        GetPlayerAuthIDHookRegistry *getPlayerAuthID() override;
        GetPlayerUserIDHookRegistry *getPlayerUserID() override;
        SVDropClientHookRegistry *svDropClient() override;
        CvarDirectSetHookRegistry *cvarDirectSet() override;
        InfoKeyValueHookRegistry *infoKeyValue() override;
        CmdArgvHookRegistry *cmdArgv() override;
        CmdArgsHookRegistry *cmdArgs() override;
        CmdArgcHookRegistry *cmdArgc() override;
        RegisterCvarHookRegistry *registerCvar() override;
        GetCvarHookRegistry *getCvar() override;
        SetModelHookRegistry *setModel() override;
        CreateEntityHookRegistry *createEntity() override;
        RemoveEntityHookRegistry *removeEntity() override;

    private:
        PrecacheModelHookRegistry m_precacheModelRegistry;
        PrecacheSoundHookRegistry m_precacheSoundRegistry;
        ChangeLevelHookRegistry m_changeLevelRegistry;
        SrvCmdHookRegistry m_srvCmdRegistry;
        SrvExecHookRegistry m_srvExecRegistry;
        RegSrvCmdHookRegistry m_regSrvCmdRegistry;
        MessageBeginHookRegistry m_messageBeginRegistry;
        MessageEndHookRegistry m_messageEndRegistry;
        WriteByteHookRegistry m_writeByteRegistry;
        WriteCharHookRegistry m_writeCharRegistry;
        WriteShortHookRegistry m_writeShortRegistry;
        WriteLongHookRegistry m_writeLongRegistry;
        WriteEntityHookRegistry m_writeEntityRegistry;
        WriteAngleHookRegistry m_writeAngleRegistry;
        WriteCoordHookRegistry m_writeCoordRegistry;
        WriteStringHookRegistry m_writeStringRegistry;
        RegUserMsgHookRegistry m_regUserMsgRegistry;
        GetPlayerAuthIDHookRegistry m_getPlayerAuthID;
        GetPlayerUserIDHookRegistry m_getPlayerUserID;
        SVDropClientHookRegistry m_svDropClientRegistry;
        CvarDirectSetHookRegistry m_cvarDirectSet;
        InfoKeyValueHookRegistry m_infoKeyValue;
        CmdArgvHookRegistry m_cmdArgv;
        CmdArgsHookRegistry m_cmdArgs;
        CmdArgcHookRegistry m_cmdArgc;
        RegisterCvarHookRegistry m_registerCvar;
        GetCvarHookRegistry m_getCvar;
        SetModelHookRegistry m_setModel;
        CreateEntityHookRegistry m_createEntity;
        RemoveEntityHookRegistry m_removeEntity;
    };
}
