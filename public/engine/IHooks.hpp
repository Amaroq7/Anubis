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
#include "Common.hpp"
#include "../IHookChains.hpp"

#include <string_view>
#include <memory>
#include <array>
#include <optional>

namespace Anubis::Game
{
    class IBaseEntity;
}

namespace Anubis::Engine
{
    class IEdict;
    class ICvar;
    class IGameClient;
    class ITraceResult;

    using IPrecacheModelHook = IHook<PrecacheId, std::string_view>;
    using IPrecacheModelHookRegistry = IHookRegistry<PrecacheId, std::string_view>;

    using IPrecacheSoundHook = IHook<PrecacheId, std::string_view>;
    using IPrecacheSoundHookRegistry = IHookRegistry<PrecacheId, std::string_view>;

    using IPrecacheGenericHook = IHook<PrecacheId, std::string_view>;
    using IPrecacheGenericHookRegistry = IHookRegistry<PrecacheId, std::string_view>;

    using IChangeLevelHook = IHook<void, std::string_view, std::string_view>;
    using IChangeLevelHookRegistry = IHookRegistry<void, std::string_view, std::string_view>;

    using ISrvCmdHook = IHook<void, std::string_view>;
    using ISrvCmdHookRegistry = IHookRegistry<void, std::string_view>;

    using ISrvExecHook = IHook<void>;
    using ISrvExecHookRegistry = IHookRegistry<void>;

    using IRegSrvCmdHook = IHook<void, std::string_view, ServerCmdCallback>;
    using IRegSrvCmdHookRegistry = IHookRegistry<void, std::string_view, ServerCmdCallback>;

    using IMessageBeginHook =
        IHook<void, MsgDest, MsgType, std::optional<std::array<float, 3>>, nstd::observer_ptr<IEdict>>;
    using IMessageBeginHookRegistry =
        IHookRegistry<void, MsgDest, MsgType, std::optional<std::array<float, 3>>, nstd::observer_ptr<IEdict>>;

    using IMessageEndHook = IHook<void>;
    using IMessageEndHookRegistry = IHookRegistry<void>;

    using IWriteByteHook = IHook<void, std::byte>;
    using IWriteByteHookRegistry = IHookRegistry<void, std::byte>;

    using IWriteCharHook = IHook<void, char>;
    using IWriteCharHookRegistry = IHookRegistry<void, char>;

    using IWriteShortHook = IHook<void, std::int16_t>;
    using IWriteShortHookRegistry = IHookRegistry<void, std::int16_t>;

    using IWriteLongHook = IHook<void, std::int32_t>;
    using IWriteLongHookRegistry = IHookRegistry<void, std::int32_t>;

    using IWriteEntityHook = IHook<void, MsgEntity>;
    using IWriteEntityHookRegistry = IHookRegistry<void, MsgEntity>;

    using IWriteAngleHook = IHook<void, MsgAngle>;
    using IWriteAngleHookRegistry = IHookRegistry<void, MsgAngle>;

    using IWriteCoordHook = IHook<void, MsgCoord>;
    using IWriteCoordHookRegistry = IHookRegistry<void, MsgCoord>;

    using IWriteStringHook = IHook<void, std::string_view>;
    using IWriteStringHookRegistry = IHookRegistry<void, std::string_view>;

    using IRegUserMsgHook = IHook<MsgType, std::string_view, MsgSize>;
    using IRegUserMsgHookRegistry = IHookRegistry<MsgType, std::string_view, MsgSize>;

    using IGetPlayerAuthIDHook = IHook<std::string_view, nstd::observer_ptr<IEdict>>;
    using IGetPlayerAuthIDHookRegistry = IHookRegistry<std::string_view, nstd::observer_ptr<IEdict>>;

    using IGetPlayerUserIDHook = IHook<UserID, nstd::observer_ptr<IEdict>>;
    using IGetPlayerUserIDHookRegistry = IHookRegistry<UserID, nstd::observer_ptr<IEdict>>;

    using ISVDropClientHook = IHook<void, nstd::observer_ptr<IGameClient>, bool, std::string_view>;
    using ISVDropClientHookRegistry = IHookRegistry<void, nstd::observer_ptr<IGameClient>, bool, std::string_view>;

    using ICvarDirectSetHook = IHook<void, nstd::observer_ptr<ICvar>, std::string_view>;
    using ICvarDirectSetHookRegistry = IHookRegistry<void, nstd::observer_ptr<ICvar>, std::string_view>;

    using IInfoKeyValueHook = IHook<std::string_view, InfoBuffer, std::string_view>;
    using IInfoKeyValueHookRegistry = IHookRegistry<std::string_view, InfoBuffer, std::string_view>;

    using ICmdArgvHook = IHook<std::string_view, std::uint8_t>;
    using ICmdArgvHookRegistry = IHookRegistry<std::string_view, std::uint8_t>;

    using ICmdArgsHook = IHook<std::string_view>;
    using ICmdArgsHookRegistry = IHookRegistry<std::string_view>;

    using ICmdArgcHook = IHook<std::uint8_t>;
    using ICmdArgcHookRegistry = IHookRegistry<std::uint8_t>;

    using IRegisterCvarHook = IHook<void, std::string_view, std::string_view>;
    using IRegisterCvarHookRegistry = IHookRegistry<void, std::string_view, std::string_view>;

    using IGetCvarHook = IHook<nstd::observer_ptr<ICvar>, std::string_view>;
    using IGetCvarHookRegistry = IHookRegistry<nstd::observer_ptr<ICvar>, std::string_view>;

    using ISetModelHook = IHook<void, nstd::observer_ptr<IEdict>, std::string_view>;
    using ISetModelHookRegistry = IHookRegistry<void, nstd::observer_ptr<IEdict>, std::string_view>;

    using ICreateEntityHook = IHook<nstd::observer_ptr<IEdict>>;
    using ICreateEntityHookRegistry = IHookRegistry<nstd::observer_ptr<IEdict>>;

    using IRemoveEntityHook = IHook<void, nstd::observer_ptr<IEdict>>;
    using IRemoveEntityHookRegistry = IHookRegistry<void, nstd::observer_ptr<IEdict>>;

    using IAlertHook = IHook<void, AlertType, std::string_view>;
    using IAlertHookRegistry = IHookRegistry<void, AlertType, std::string_view>;

    using IServerPrintHook = IHook<void, std::string_view>;
    using IServerPrintHookRegistry = IHookRegistry<void, std::string_view>;

    using IIsDedicatedHook = IHook<bool>;
    using IIsDedicatedHookRegistry = IHookRegistry<bool>;

    using ICheckEngParmHook = IHook<std::pair<std::size_t, std::string_view>, std::string_view>;
    using ICheckEngParmHookRegistry = IHookRegistry<std::pair<std::size_t, std::string_view>, std::string_view>;

    using IQueryClientCvarValueHook = IHook<void, nstd::observer_ptr<IEdict>, std::string_view>;
    using IQueryClientCvarValueHookRegistry = IHookRegistry<void, nstd::observer_ptr<IEdict>, std::string_view>;

    using IQueryClientCvarValue2Hook = IHook<void, nstd::observer_ptr<IEdict>, std::string_view, std::uint32_t>;
    using IQueryClientCvarValue2HookRegistry =
        IHookRegistry<void, nstd::observer_ptr<IEdict>, std::string_view, std::uint32_t>;

    using IIndexOfEdictHook = IHook<std::uint32_t, nstd::observer_ptr<IEdict>>;
    using IIndexOfEdictHookRegistry = IHookRegistry<std::uint32_t, nstd::observer_ptr<IEdict>>;

    using IGetGameDirHook = IHook<std::string>;
    using IGetGameDirHookRegistry = IHookRegistry<std::string>;

    using IGetCvarValueHook = IHook<float, std::string_view>;
    using IGetCvarValueHookRegistry = IHookRegistry<float, std::string_view>;

    using IGetCvarStringHook = IHook<std::string_view, std::string_view>;
    using IGetCvarStringHookRegistry = IHookRegistry<std::string_view, std::string_view>;

    using ISetCvarValueHook = IHook<void, std::string_view, float>;
    using ISetCvarValueHookRegistry = IHookRegistry<void, std::string_view, float>;

    using ISetCvarStringHook = IHook<void, std::string_view, std::string_view>;
    using ISetCvarStringHookRegistry = IHookRegistry<void, std::string_view, std::string_view>;

    using IGetEntityOffsetHook = IHook<EntityOffset, nstd::observer_ptr<IEdict>>;
    using IGetEntityOffsetHookRegistry = IHookRegistry<EntityOffset, nstd::observer_ptr<IEdict>>;

    using IGetEntityOfEntityOffsetHook = IHook<nstd::observer_ptr<IEdict>, EntityOffset>;
    using IGetEntityOfEntityOffsetHookRegistry = IHookRegistry<nstd::observer_ptr<IEdict>, EntityOffset>;

    using IGetEntityOfEntityIdHook = IHook<nstd::observer_ptr<IEdict>, std::uint32_t>;
    using IGetEntityOfEntityIdHookRegistry = IHookRegistry<nstd::observer_ptr<IEdict>, std::uint32_t>;

    using IAllocEntPrivateDataHook =
        IHook<std::unique_ptr<Game::IBaseEntity>, nstd::observer_ptr<IEdict>, std::int32_t>;
    using IAllocEntPrivateDataHookRegistry =
        IHookRegistry<std::unique_ptr<Game::IBaseEntity>, nstd::observer_ptr<IEdict>, std::int32_t>;

    using IEdAllocHook = IHook<nstd::observer_ptr<IEdict>>;
    using IEdAllocHookRegistry = IHookRegistry<nstd::observer_ptr<IEdict>>;

    using IStringFromOffsetHook = IHook<std::string_view, StringOffset>;
    using IStringFromOffsetHookRegistry = IHookRegistry<std::string_view, StringOffset>;

    using IAllocStringHook = IHook<StringOffset, std::string_view>;
    using IAllocStringHookRegistry = IHookRegistry<StringOffset, std::string_view>;

    using IModelIndexHook = IHook<ModelIndex, std::string_view>;
    using IModelIndexHookRegistry = IHookRegistry<ModelIndex, std::string_view>;

    using IRandomLongHook = IHook<std::int32_t, std::int32_t, std::int32_t>;
    using IRandomLongHookRegistry = IHookRegistry<std::int32_t, std::int32_t, std::int32_t>;

    using IRandomFloatHook = IHook<float, float, float>;
    using IRandomFloatHookRegistry = IHookRegistry<float, float, float>;

    using IClientPrintHook = IHook<void, nstd::observer_ptr<IEdict>, PrintType, std::string_view>;
    using IClientPrintHookRegistry = IHookRegistry<void, nstd::observer_ptr<IEdict>, PrintType, std::string_view>;

    using IEntIsOnFloorHook = IHook<bool, nstd::observer_ptr<IEdict>>;
    using IEntIsOnFloorHookRegistry = IHookRegistry<bool, nstd::observer_ptr<IEdict>>;

    using IDropToFloorHook = IHook<std::int8_t, nstd::observer_ptr<IEdict>>;
    using IDropToFloorHookRegistry = IHookRegistry<std::int8_t, nstd::observer_ptr<IEdict>>;

    using IEmitSoundHook = IHook<void, nstd::observer_ptr<IEdict>, Channel, std::string_view, float, float, SoundFlags, Pitch>;
    using IEmitSoundHookRegistry = IHookRegistry<void, nstd::observer_ptr<IEdict>, Channel, std::string_view, float, float, SoundFlags, Pitch>;

    using IEmitAmbientSoundHook = IHook<void, nstd::observer_ptr<IEdict>, float*, std::string_view, float, float, SoundFlags, Pitch>;
    using IEmitAmbientSoundRegistry = IHookRegistry<void, nstd::observer_ptr<IEdict>, float*, std::string_view, float, float, SoundFlags, Pitch>;

    using ITraceLinetHook = IHook<void, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using ITraceLineHookRegistry = IHookRegistry<void, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    using ITraceTossHook = IHook<void, nstd::observer_ptr<IEdict>, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using ITraceTossHookRegistry = IHookRegistry<void, nstd::observer_ptr<IEdict>, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    using ITraceMonsterHullHook = IHook<bool, nstd::observer_ptr<IEdict>, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using ITraceMonsterHullHookRegistry = IHookRegistry<bool, nstd::observer_ptr<IEdict>, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    using ITraceHullHook = IHook<void, const float*, const float*, int, int , nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using ITraceHullHookRegistry = IHookRegistry<void, const float*, const float*, int, int , nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    using ITraceModelHook = IHook<void, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using ITraceModelHookRegistry = IHookRegistry<void, const float*, const float*, int, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    using ITraceTextureHook = IHook<std::string_view, nstd::observer_ptr<IEdict>, const float*, const float*>;
    using ITraceTextureHookRegistry = IHookRegistry<std::string_view, nstd::observer_ptr<IEdict>, const float*, const float*>;

    using ITraceSphereHook = IHook<void, const float*, const float*, int, float, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;
    using ITraceSphereHookRegistry = IHookRegistry<void, const float*, const float*, int, float, nstd::observer_ptr<IEdict>, nstd::observer_ptr<ITraceResult>>;

    class IHooks
    {
    public:
        virtual ~IHooks() = default;

        virtual nstd::observer_ptr<IPrecacheModelHookRegistry> precacheModel() = 0;
        virtual nstd::observer_ptr<IPrecacheSoundHookRegistry> precacheSound() = 0;
        virtual nstd::observer_ptr<IPrecacheGenericHookRegistry> precacheGeneric() = 0;
        virtual nstd::observer_ptr<IChangeLevelHookRegistry> changeLevel() = 0;
        virtual nstd::observer_ptr<ISrvCmdHookRegistry> srvCmd() = 0;
        virtual nstd::observer_ptr<ISrvExecHookRegistry> srvExec() = 0;
        virtual nstd::observer_ptr<IRegSrvCmdHookRegistry> regSrvCmd() = 0;
        virtual nstd::observer_ptr<IMessageBeginHookRegistry> messageBegin() = 0;
        virtual nstd::observer_ptr<IMessageEndHookRegistry> messageEnd() = 0;
        virtual nstd::observer_ptr<IWriteByteHookRegistry> writeByte() = 0;
        virtual nstd::observer_ptr<IWriteCharHookRegistry> writeChar() = 0;
        virtual nstd::observer_ptr<IWriteShortHookRegistry> writeShort() = 0;
        virtual nstd::observer_ptr<IWriteLongHookRegistry> writeLong() = 0;
        virtual nstd::observer_ptr<IWriteEntityHookRegistry> writeEntity() = 0;
        virtual nstd::observer_ptr<IWriteAngleHookRegistry> writeAngle() = 0;
        virtual nstd::observer_ptr<IWriteCoordHookRegistry> writeCoord() = 0;
        virtual nstd::observer_ptr<IWriteStringHookRegistry> writeString() = 0;
        virtual nstd::observer_ptr<IRegUserMsgHookRegistry> regUserMsg() = 0;
        virtual nstd::observer_ptr<IGetPlayerAuthIDHookRegistry> getPlayerAuthID() = 0;
        virtual nstd::observer_ptr<IGetPlayerUserIDHookRegistry> getPlayerUserID() = 0;
        virtual nstd::observer_ptr<ISVDropClientHookRegistry> svDropClient() = 0;
        virtual nstd::observer_ptr<ICvarDirectSetHookRegistry> cvarDirectSetRe() = 0;
        virtual nstd::observer_ptr<IInfoKeyValueHookRegistry> infoKeyValue() = 0;
        virtual nstd::observer_ptr<ICmdArgvHookRegistry> cmdArgv() = 0;
        virtual nstd::observer_ptr<ICmdArgsHookRegistry> cmdArgs() = 0;
        virtual nstd::observer_ptr<ICmdArgcHookRegistry> cmdArgc() = 0;
        virtual nstd::observer_ptr<IRegisterCvarHookRegistry> registerCvar() = 0;
        virtual nstd::observer_ptr<IGetCvarHookRegistry> getCvar() = 0;
        virtual nstd::observer_ptr<ISetModelHookRegistry> setModel() = 0;
        virtual nstd::observer_ptr<ICreateEntityHookRegistry> createEntity() = 0;
        virtual nstd::observer_ptr<IRemoveEntityHookRegistry> removeEntity() = 0;
        virtual nstd::observer_ptr<IAlertHookRegistry> alert() = 0;
        virtual nstd::observer_ptr<IServerPrintHookRegistry> serverPrint() = 0;
        virtual nstd::observer_ptr<IIsDedicatedHookRegistry> isDedicated() = 0;
        virtual nstd::observer_ptr<ICheckEngParmHookRegistry> checkEngParm() = 0;
        virtual nstd::observer_ptr<IQueryClientCvarValueHookRegistry> queryClientCvarValue() = 0;
        virtual nstd::observer_ptr<IQueryClientCvarValue2HookRegistry> queryClientCvarValue2() = 0;
        virtual nstd::observer_ptr<ICvarDirectSetHookRegistry> cvarDirectSet() = 0;
        virtual nstd::observer_ptr<IIndexOfEdictHookRegistry> indexOfEdict() = 0;
        virtual nstd::observer_ptr<IGetGameDirHookRegistry> gameDir() = 0;
        virtual nstd::observer_ptr<IGetCvarValueHookRegistry> getCvarValue() = 0;
        virtual nstd::observer_ptr<IGetCvarStringHookRegistry> getCvarString() = 0;
        virtual nstd::observer_ptr<ISetCvarValueHookRegistry> setCvarValue() = 0;
        virtual nstd::observer_ptr<ISetCvarStringHookRegistry> setCvarString() = 0;
        virtual nstd::observer_ptr<IGetEntityOffsetHookRegistry> getEntOffset() = 0;
        virtual nstd::observer_ptr<IGetEntityOfEntityOffsetHookRegistry> getEntityOfEntOffset() = 0;
        virtual nstd::observer_ptr<IGetEntityOfEntityIdHookRegistry> getEntityOfEntId() = 0;
        virtual nstd::observer_ptr<IAllocEntPrivateDataHookRegistry> allocEntPrivData() = 0;
        virtual nstd::observer_ptr<IEdAllocHookRegistry> edAlloc() = 0;
        virtual nstd::observer_ptr<IStringFromOffsetHookRegistry> stringFromOffset() = 0;
        virtual nstd::observer_ptr<IAllocStringHookRegistry> strAlloc() = 0;
        virtual nstd::observer_ptr<IModelIndexHookRegistry> modelIndex() = 0;
        virtual nstd::observer_ptr<IRandomLongHookRegistry> randomLong() = 0;
        virtual nstd::observer_ptr<IRandomFloatHookRegistry> randomFloat() = 0;
        virtual nstd::observer_ptr<IClientPrintHookRegistry> clientPrint() = 0;
        virtual nstd::observer_ptr<IEntIsOnFloorHookRegistry> entIsOnFloor() = 0;
        virtual nstd::observer_ptr<IDropToFloorHookRegistry> dropToFloor() = 0;
        virtual nstd::observer_ptr<IEmitSoundHookRegistry> emitSound() = 0;
        virtual nstd::observer_ptr<IEmitAmbientSoundRegistry> emitAmbientSound() = 0;
        virtual nstd::observer_ptr<ITraceLineHookRegistry> traceLine() = 0;
        virtual nstd::observer_ptr<ITraceTossHookRegistry> traceToss() = 0;
        virtual nstd::observer_ptr<ITraceMonsterHullHookRegistry> traceMonsterHull() = 0;
        virtual nstd::observer_ptr<ITraceHullHookRegistry> traceHull() = 0;
        virtual nstd::observer_ptr<ITraceModelHookRegistry> traceModel() = 0;
        virtual nstd::observer_ptr<ITraceTextureHookRegistry> traceTexture() = 0;
        virtual nstd::observer_ptr<ITraceSphereHookRegistry> traceSphere() = 0;
    };
} // namespace Anubis::Engine
