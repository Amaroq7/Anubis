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

#include <engine/Common.hpp>
#include <engine/IHooks.hpp>
#include "HookChains.hpp"

class IRehldsHookchains;

namespace Anubis::Engine
{
    using PrecacheModelHook = Hook<PrecacheId, std::string_view>;
    using PrecacheModelHookRegistry = HookRegistry<PrecacheId, std::string_view>;

    using PrecacheSoundHook = Hook<PrecacheId, std::string_view>;
    using PrecacheSoundHookRegistry = HookRegistry<PrecacheId, std::string_view>;

    using PrecacheGenericHook = Hook<PrecacheId, std::string_view>;
    using PrecacheGenericHookRegistry = HookRegistry<PrecacheId, std::string_view>;

    using ChangeLevelHook = Hook<void, std::string_view, std::string_view>;
    using ChangeLevelHookRegistry = HookRegistry<void, std::string_view, std::string_view>;

    using SrvCmdHook = Hook<void, std::string_view>;
    using SrvCmdHookRegistry = HookRegistry<void, std::string_view>;

    using SrvExecHook = Hook<void>;
    using SrvExecHookRegistry = HookRegistry<void>;

    using RegSrvCmdHook = Hook<void, std::string_view, ServerCmdCallback>;
    using RegSrvCmdHookRegistry = HookRegistry<void, std::string_view, ServerCmdCallback>;

    using MessageBeginHook =
        Hook<void, MsgDest, MsgType, std::optional<std::array<float, 3>>, nstd::observer_ptr<IEdict>>;
    using MessageBeginHookRegistry =
        HookRegistry<void, MsgDest, MsgType, std::optional<std::array<float, 3>>, nstd::observer_ptr<IEdict>>;

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

    using RegUserMsgHook = Hook<MsgType, std::string_view, MsgSize>;
    using RegUserMsgHookRegistry = HookRegistry<MsgType, std::string_view, MsgSize>;

    using GetPlayerAuthIDHook = Hook<std::string_view, nstd::observer_ptr<IEdict>>;
    using GetPlayerAuthIDHookRegistry = HookRegistry<std::string_view, nstd::observer_ptr<IEdict>>;

    using GetPlayerUserIDHook = Hook<UserID, nstd::observer_ptr<IEdict>>;
    using GetPlayerUserIDHookRegistry = HookRegistry<UserID, nstd::observer_ptr<IEdict>>;

    using SVDropClientHook = Hook<void, nstd::observer_ptr<IGameClient>, bool, std::string_view>;
    using SVDropClientHookRegistry = HookRegistry<void, nstd::observer_ptr<IGameClient>, bool, std::string_view>;

    using CvarDirectSetHook = Hook<void, nstd::observer_ptr<ICvar>, std::string_view>;
    using CvarDirectSetHookRegistry = HookRegistry<void, nstd::observer_ptr<ICvar>, std::string_view>;

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

    using GetCvarHook = Hook<nstd::observer_ptr<ICvar>, std::string_view>;
    using GetCvarHookRegistry = HookRegistry<nstd::observer_ptr<ICvar>, std::string_view>;

    using SetModelHook = Hook<void, nstd::observer_ptr<IEdict>, std::string_view>;
    using SetModelHookRegistry = HookRegistry<void, nstd::observer_ptr<IEdict>, std::string_view>;

    using CreateEntityHook = Hook<nstd::observer_ptr<IEdict>>;
    using CreateEntityHookRegistry = HookRegistry<nstd::observer_ptr<IEdict>>;

    using RemoveEntityHook = Hook<void, nstd::observer_ptr<IEdict>>;
    using RemoveEntityHookRegistry = HookRegistry<void, nstd::observer_ptr<IEdict>>;

    using AlertHook = Hook<void, AlertType, std::string_view>;
    using AlertHookRegistry = HookRegistry<void, AlertType, std::string_view>;

    using ServerPrintHook = Hook<void, std::string_view>;
    using ServerPrintHookRegistry = HookRegistry<void, std::string_view>;

    using IsDedicatedHook = Hook<bool>;
    using IsDedicatedHookRegistry = HookRegistry<bool>;

    using CheckEngParmHook = Hook<std::pair<std::size_t, std::string_view>, std::string_view>;
    using CheckEngParmHookRegistry = HookRegistry<std::pair<std::size_t, std::string_view>, std::string_view>;

    using QueryClientCvarValueHook = Hook<void, nstd::observer_ptr<IEdict>, std::string_view>;
    using QueryClientCvarValueHookRegistry = HookRegistry<void, nstd::observer_ptr<IEdict>, std::string_view>;

    using QueryClientCvarValue2Hook = Hook<void, nstd::observer_ptr<IEdict>, std::string_view, std::uint32_t>;
    using QueryClientCvarValue2HookRegistry =
        HookRegistry<void, nstd::observer_ptr<IEdict>, std::string_view, std::uint32_t>;

    using IndexOfEdictHook = Hook<std::uint32_t, nstd::observer_ptr<IEdict>>;
    using IndexOfEdictHookRegistry = HookRegistry<std::uint32_t, nstd::observer_ptr<IEdict>>;

    using GetGameDirHook = Hook<std::string>;
    using GetGameDirHookRegistry = HookRegistry<std::string>;

    using GetCvarValueHook = Hook<float, std::string_view>;
    using GetCvarValueHookRegistry = HookRegistry<float, std::string_view>;

    using GetCvarStringHook = Hook<std::string_view, std::string_view>;
    using GetCvarStringHookRegistry = HookRegistry<std::string_view, std::string_view>;

    using SetCvarValueHook = Hook<void, std::string_view, float>;
    using SetCvarValueHookRegistry = HookRegistry<void, std::string_view, float>;

    using SetCvarStringHook = Hook<void, std::string_view, std::string_view>;
    using SetCvarStringHookRegistry = HookRegistry<void, std::string_view, std::string_view>;

    using GetEntityOffsetHook = Hook<EntityOffset, nstd::observer_ptr<IEdict>>;
    using GetEntityOffsetHookRegistry = HookRegistry<EntityOffset, nstd::observer_ptr<IEdict>>;

    using GetEntityOfEntityOffsetHook = Hook<nstd::observer_ptr<IEdict>, EntityOffset>;
    using GetEntityOfEntityOffsetHookRegistry = HookRegistry<nstd::observer_ptr<IEdict>, EntityOffset>;

    using GetEntityOfEntityIdHook = Hook<nstd::observer_ptr<IEdict>, std::uint32_t>;
    using GetEntityOfEntityIdHookRegistry = HookRegistry<nstd::observer_ptr<IEdict>, std::uint32_t>;

    using AllocEntPrivateDataHook =
        Hook<nstd::observer_ptr<Game::IBaseEntity>, nstd::observer_ptr<IEdict>, std::int32_t>;
    using AllocEntPrivateDataHookRegistry =
        HookRegistry<nstd::observer_ptr<Game::IBaseEntity>, nstd::observer_ptr<IEdict>, std::int32_t>;

    using EdAllocHook = Hook<nstd::observer_ptr<IEdict>>;
    using EdAllocHookRegistry = HookRegistry<nstd::observer_ptr<IEdict>>;

    using StringFromOffsetHook = Hook<std::string_view, StringOffset>;
    using StringFromOffsetHookRegistry = HookRegistry<std::string_view, StringOffset>;

    using AllocStringHook = Hook<StringOffset, std::string_view>;
    using AllocStringHookRegistry = HookRegistry<StringOffset, std::string_view>;

    using ModelIndexHook = Hook<ModelIndex, std::string_view>;
    using ModelIndexHookRegistry = HookRegistry<ModelIndex, std::string_view>;

    using RandomLongHook = Hook<std::int32_t, std::int32_t, std::int32_t>;
    using RandomLongHookRegistry = HookRegistry<std::int32_t, std::int32_t, std::int32_t>;

    using RandomFloatHook = Hook<float, float, float>;
    using RandomFloatHookRegistry = HookRegistry<float, float, float>;

    using ClientPrintHook = Hook<void, nstd::observer_ptr<IEdict>, PrintType, std::string_view>;
    using ClientPrintHookRegistry = HookRegistry<void, nstd::observer_ptr<IEdict>, PrintType, std::string_view>;

    using EntIsOnFloorHook = Hook<bool, nstd::observer_ptr<IEdict>>;
    using EntIsOnFloorHookRegistry = HookRegistry<bool, nstd::observer_ptr<IEdict>>;

    using DropToFloorHook = Hook<std::int8_t, nstd::observer_ptr<IEdict>>;
    using DropToFloorHookRegistry = HookRegistry<std::int8_t, nstd::observer_ptr<IEdict>>;

    using EmitSoundHook = Hook<void, nstd::observer_ptr<IEdict>, Channel, std::string_view, float, float, SoundFlags, Pitch>;
    using EmitSoundHookRegistry = HookRegistry<void, nstd::observer_ptr<IEdict>, Channel, std::string_view, float, float, SoundFlags, Pitch>;

    using EmitAmbientSoundHook = Hook<void, nstd::observer_ptr<IEdict>, float*, std::string_view, float, float, SoundFlags, Pitch>;
    using EmitAmbientSoundRegistry = HookRegistry<void, nstd::observer_ptr<IEdict>, float*, std::string_view, float, float, SoundFlags, Pitch>;

    using TraceLinetHook = Hook<void, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using TraceLineHookRegistry = HookRegistry<void, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    using TraceTossHook = Hook<void, nstd::observer_ptr<IEdict>, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using TraceTossHookRegistry = HookRegistry<void, nstd::observer_ptr<IEdict>, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    using TraceMonsterHullHook = Hook<bool, nstd::observer_ptr<IEdict>, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using TraceMonsterHullHookRegistry = HookRegistry<bool, nstd::observer_ptr<IEdict>, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    using TraceHullHook = Hook<void, const float*, const float*, int, int , nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using TraceHullHookRegistry = HookRegistry<void, const float*, const float*, int, int , nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    using TraceModelHook = Hook<void, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using TraceModelHookRegistry = HookRegistry<void, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    using TraceTextureHook = Hook<std::string_view, nstd::observer_ptr<IEdict>, const float*, const float*>;
    using TraceTextureHookRegistry = HookRegistry<std::string_view, nstd::observer_ptr<IEdict>, const float*, const float*>;

    using TraceSphereHook = Hook<void, const float*, const float*, int, float, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using TraceSphereHookRegistry = HookRegistry<void, const float*, const float*, int, float, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    class Hooks final : public IHooks
    {
    public:
        explicit Hooks(nstd::observer_ptr<IRehldsHookchains> rehldsHooks);
        ~Hooks() final = default;

        nstd::observer_ptr<IPrecacheModelHookRegistry> precacheModel() final;
        nstd::observer_ptr<IPrecacheSoundHookRegistry> precacheSound() final;
        nstd::observer_ptr<IPrecacheGenericHookRegistry> precacheGeneric() final;
        nstd::observer_ptr<IChangeLevelHookRegistry> changeLevel() final;
        nstd::observer_ptr<ISrvCmdHookRegistry> srvCmd() final;
        nstd::observer_ptr<ISrvExecHookRegistry> srvExec() final;
        nstd::observer_ptr<IRegSrvCmdHookRegistry> regSrvCmd() final;
        nstd::observer_ptr<IMessageBeginHookRegistry> messageBegin() final;
        nstd::observer_ptr<IMessageEndHookRegistry> messageEnd() final;
        nstd::observer_ptr<IWriteByteHookRegistry> writeByte() final;
        nstd::observer_ptr<IWriteCharHookRegistry> writeChar() final;
        nstd::observer_ptr<IWriteShortHookRegistry> writeShort() final;
        nstd::observer_ptr<IWriteLongHookRegistry> writeLong() final;
        nstd::observer_ptr<IWriteEntityHookRegistry> writeEntity() final;
        nstd::observer_ptr<IWriteAngleHookRegistry> writeAngle() final;
        nstd::observer_ptr<IWriteCoordHookRegistry> writeCoord() final;
        nstd::observer_ptr<IWriteStringHookRegistry> writeString() final;
        nstd::observer_ptr<IRegUserMsgHookRegistry> regUserMsg() final;
        nstd::observer_ptr<IGetPlayerAuthIDHookRegistry> getPlayerAuthID() final;
        nstd::observer_ptr<IGetPlayerUserIDHookRegistry> getPlayerUserID() final;
        nstd::observer_ptr<ISVDropClientHookRegistry> svDropClient() final;
        nstd::observer_ptr<ICvarDirectSetHookRegistry> cvarDirectSetRe() final;
        nstd::observer_ptr<IInfoKeyValueHookRegistry> infoKeyValue() final;
        nstd::observer_ptr<ICmdArgvHookRegistry> cmdArgv() final;
        nstd::observer_ptr<ICmdArgsHookRegistry> cmdArgs() final;
        nstd::observer_ptr<ICmdArgcHookRegistry> cmdArgc() final;
        nstd::observer_ptr<IRegisterCvarHookRegistry> registerCvar() final;
        nstd::observer_ptr<IGetCvarHookRegistry> getCvar() final;
        nstd::observer_ptr<ISetModelHookRegistry> setModel() final;
        nstd::observer_ptr<ICreateEntityHookRegistry> createEntity() final;
        nstd::observer_ptr<IRemoveEntityHookRegistry> removeEntity() final;
        nstd::observer_ptr<IAlertHookRegistry> alert() final;
        nstd::observer_ptr<IServerPrintHookRegistry> serverPrint() final;
        nstd::observer_ptr<IIsDedicatedHookRegistry> isDedicated() final;
        nstd::observer_ptr<ICheckEngParmHookRegistry> checkEngParm() final;
        nstd::observer_ptr<IQueryClientCvarValueHookRegistry> queryClientCvarValue() final;
        nstd::observer_ptr<IQueryClientCvarValue2HookRegistry> queryClientCvarValue2() final;
        nstd::observer_ptr<ICvarDirectSetHookRegistry> cvarDirectSet() final;
        nstd::observer_ptr<IIndexOfEdictHookRegistry> indexOfEdict() final;
        nstd::observer_ptr<IGetGameDirHookRegistry> gameDir() final;
        nstd::observer_ptr<IGetCvarValueHookRegistry> getCvarValue() final;
        nstd::observer_ptr<IGetCvarStringHookRegistry> getCvarString() final;
        nstd::observer_ptr<ISetCvarValueHookRegistry> setCvarValue() final;
        nstd::observer_ptr<ISetCvarStringHookRegistry> setCvarString() final;
        nstd::observer_ptr<IGetEntityOffsetHookRegistry> getEntOffset() final;
        nstd::observer_ptr<IGetEntityOfEntityOffsetHookRegistry> getEntityOfEntOffset() final;
        nstd::observer_ptr<IGetEntityOfEntityIdHookRegistry> getEntityOfEntId() final;
        nstd::observer_ptr<IAllocEntPrivateDataHookRegistry> allocEntPrivData() final;
        nstd::observer_ptr<IEdAllocHookRegistry> edAlloc() final;
        nstd::observer_ptr<IStringFromOffsetHookRegistry> stringFromOffset() final;
        nstd::observer_ptr<IAllocStringHookRegistry> strAlloc() final;
        nstd::observer_ptr<IModelIndexHookRegistry> modelIndex() final;
        nstd::observer_ptr<IRandomLongHookRegistry> randomLong() final;
        nstd::observer_ptr<IRandomFloatHookRegistry> randomFloat() final;
        nstd::observer_ptr<IClientPrintHookRegistry> clientPrint() final;
        nstd::observer_ptr<IEntIsOnFloorHookRegistry> entIsOnFloor() final;
        nstd::observer_ptr<IDropToFloorHookRegistry> dropToFloor() final;
        nstd::observer_ptr<IEmitSoundHookRegistry> emitSound() final;
        nstd::observer_ptr<IEmitAmbientSoundRegistry> emitAmbientSound() final;
        nstd::observer_ptr<ITraceLineHookRegistry> traceLine() final;
        nstd::observer_ptr<ITraceTossHookRegistry> traceToss() final;
        nstd::observer_ptr<ITraceMonsterHullHookRegistry> traceMonsterHull() final;
        nstd::observer_ptr<ITraceHullHookRegistry> traceHull() final;
        nstd::observer_ptr<ITraceModelHookRegistry> traceModel() final;
        nstd::observer_ptr<ITraceTextureHookRegistry> traceTexture() final;
        nstd::observer_ptr<ITraceSphereHookRegistry> traceSphere() final;

    private:
        std::unique_ptr<PrecacheModelHookRegistry> m_precacheModelRegistry;
        std::unique_ptr<PrecacheSoundHookRegistry> m_precacheSoundRegistry;
        std::unique_ptr<PrecacheGenericHookRegistry> m_precacheGenericRegistry;
        std::unique_ptr<ChangeLevelHookRegistry> m_changeLevelRegistry;
        std::unique_ptr<SrvCmdHookRegistry> m_srvCmdRegistry;
        std::unique_ptr<SrvExecHookRegistry> m_srvExecRegistry;
        std::unique_ptr<RegSrvCmdHookRegistry> m_regSrvCmdRegistry;
        std::unique_ptr<MessageBeginHookRegistry> m_messageBeginRegistry;
        std::unique_ptr<MessageEndHookRegistry> m_messageEndRegistry;
        std::unique_ptr<WriteByteHookRegistry> m_writeByteRegistry;
        std::unique_ptr<WriteCharHookRegistry> m_writeCharRegistry;
        std::unique_ptr<WriteShortHookRegistry> m_writeShortRegistry;
        std::unique_ptr<WriteLongHookRegistry> m_writeLongRegistry;
        std::unique_ptr<WriteEntityHookRegistry> m_writeEntityRegistry;
        std::unique_ptr<WriteAngleHookRegistry> m_writeAngleRegistry;
        std::unique_ptr<WriteCoordHookRegistry> m_writeCoordRegistry;
        std::unique_ptr<WriteStringHookRegistry> m_writeStringRegistry;
        std::unique_ptr<RegUserMsgHookRegistry> m_regUserMsgRegistry;
        std::unique_ptr<GetPlayerAuthIDHookRegistry> m_getPlayerAuthIDRegistry;
        std::unique_ptr<GetPlayerUserIDHookRegistry> m_getPlayerUserIDRegistry;
        std::unique_ptr<SVDropClientHookRegistry> m_svDropClientRegistry;
        std::unique_ptr<CvarDirectSetHookRegistry> m_cvarDirectSetReRegistry;
        std::unique_ptr<InfoKeyValueHookRegistry> m_infoKeyValueRegistry;
        std::unique_ptr<CmdArgvHookRegistry> m_cmdArgvRegistry;
        std::unique_ptr<CmdArgsHookRegistry> m_cmdArgsRegistry;
        std::unique_ptr<CmdArgcHookRegistry> m_cmdArgcRegistry;
        std::unique_ptr<RegisterCvarHookRegistry> m_registerCvarRegistry;
        std::unique_ptr<GetCvarHookRegistry> m_getCvarRegistry;
        std::unique_ptr<SetModelHookRegistry> m_setModelRegistry;
        std::unique_ptr<CreateEntityHookRegistry> m_createEntityRegistry;
        std::unique_ptr<RemoveEntityHookRegistry> m_removeEntityRegistry;
        std::unique_ptr<AlertHookRegistry> m_alertRegistry;
        std::unique_ptr<ServerPrintHookRegistry> m_serverPrintRegistry;
        std::unique_ptr<IsDedicatedHookRegistry> m_isDedicatedRegistry;
        std::unique_ptr<CheckEngParmHookRegistry> m_checkEngParmRegistry;
        std::unique_ptr<QueryClientCvarValueHookRegistry> m_queryClientCvarValueRegistry;
        std::unique_ptr<QueryClientCvarValue2HookRegistry> m_queryClientCvarValue2Registry;
        std::unique_ptr<CvarDirectSetHookRegistry> m_cvarDirectSetRegistry;
        std::unique_ptr<IndexOfEdictHookRegistry> m_indexOfEdictRegistry;
        std::unique_ptr<GetGameDirHookRegistry> m_gameDirRegistry;
        std::unique_ptr<GetCvarValueHookRegistry> m_getCvarValueRegistry;
        std::unique_ptr<GetCvarStringHookRegistry> m_getCvarStringRegistry;
        std::unique_ptr<SetCvarValueHookRegistry> m_setCvarValueRegistry;
        std::unique_ptr<SetCvarStringHookRegistry> m_setCvarStringRegistry;
        std::unique_ptr<GetEntityOffsetHookRegistry> m_getEntOffsetRegistry;
        std::unique_ptr<GetEntityOfEntityOffsetHookRegistry> m_getEntityOfEntOffsetRegistry;
        std::unique_ptr<GetEntityOfEntityIdHookRegistry> m_getEntityOfEntIdRegistry;
        std::unique_ptr<AllocEntPrivateDataHookRegistry> m_allocEntPrivDataRegistry;
        std::unique_ptr<EdAllocHookRegistry> m_edAllocRegistry;
        std::unique_ptr<StringFromOffsetHookRegistry> m_stringFromOffsetRegistry;
        std::unique_ptr<AllocStringHookRegistry> m_strAllocRegistry;
        std::unique_ptr<ModelIndexHookRegistry> m_modelIndexHookRegistry;
        std::unique_ptr<RandomLongHookRegistry> m_randomLongHookRegistry;
        std::unique_ptr<RandomFloatHookRegistry> m_randomFloatHookRegistry;
        std::unique_ptr<ClientPrintHookRegistry> m_clientPrintHookRegistry;
        std::unique_ptr<EntIsOnFloorHookRegistry> m_entIsOnFloorHookRegistry;
        std::unique_ptr<DropToFloorHookRegistry> m_dropToFloorHookRegistry;
        std::unique_ptr<EmitSoundHookRegistry> m_emitSoundHookRegistry;
        std::unique_ptr<EmitAmbientSoundRegistry> m_emitAmbientSoundRegistry;
        std::unique_ptr<TraceLineHookRegistry> m_traceLineHookRegistry;
        std::unique_ptr<TraceTossHookRegistry> m_traceTossHookRegistry;
        std::unique_ptr<TraceMonsterHullHookRegistry> m_traceMonsterHullHookRegistry;
        std::unique_ptr<TraceHullHookRegistry> m_traceHullHookRegistry;
        std::unique_ptr<TraceModelHookRegistry> m_traceModelHookRegistry;
        std::unique_ptr<TraceTextureHookRegistry> m_traceTextureHookRegistry;
        std::unique_ptr<TraceSphereHookRegistry> m_traceSphereHookRegistry;
    };
} // namespace Anubis::Engine
