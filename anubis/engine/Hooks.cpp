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

#include <osconfig.h>
#include <extdll.h>
#include "ReHooks.hpp"

namespace Anubis::Engine
{
    Hooks::Hooks(nstd::observer_ptr<IRehldsHookchains> rehldsHooks)
        : m_precacheModelRegistry(std::make_unique<PrecacheModelHookRegistry>()),
          m_precacheSoundRegistry(std::make_unique<PrecacheSoundHookRegistry>()),
          m_precacheGenericRegistry(std::make_unique<PrecacheGenericHookRegistry>()),
          m_changeLevelRegistry(std::make_unique<ChangeLevelHookRegistry>()),
          m_srvCmdRegistry(std::make_unique<SrvCmdHookRegistry>()),
          m_srvExecRegistry(std::make_unique<SrvExecHookRegistry>()),
          m_regSrvCmdRegistry(std::make_unique<RegSrvCmdHookRegistry>()),
          m_messageBeginRegistry(std::make_unique<MessageBeginHookRegistry>()),
          m_messageEndRegistry(std::make_unique<MessageEndHookRegistry>()),
          m_writeByteRegistry(std::make_unique<WriteByteHookRegistry>()),
          m_writeCharRegistry(std::make_unique<WriteCharHookRegistry>()),
          m_writeShortRegistry(std::make_unique<WriteShortHookRegistry>()),
          m_writeLongRegistry(std::make_unique<WriteLongHookRegistry>()),
          m_writeEntityRegistry(std::make_unique<WriteEntityHookRegistry>()),
          m_writeAngleRegistry(std::make_unique<WriteAngleHookRegistry>()),
          m_writeCoordRegistry(std::make_unique<WriteCoordHookRegistry>()),
          m_writeStringRegistry(std::make_unique<WriteStringHookRegistry>()),
          m_regUserMsgRegistry(std::make_unique<RegUserMsgHookRegistry>()),
          m_getPlayerAuthIDRegistry(std::make_unique<GetPlayerAuthIDHookRegistry>()),
          m_getPlayerUserIDRegistry(std::make_unique<GetPlayerUserIDHookRegistry>()),
          m_svDropClientRegistry(std::make_unique<SVDropClientHookRegistry>(
              [&rehldsHooks]()
              {
                  rehldsHooks->SV_DropClient()->registerHook(ReHooks::SV_DropClientHook);
              },
              [&rehldsHooks]()
              {
                  rehldsHooks->SV_DropClient()->unregisterHook(ReHooks::SV_DropClientHook);
              })),
          m_cvarDirectSetReRegistry(std::make_unique<CvarDirectSetHookRegistry>(
              [&rehldsHooks]()
              {
                  rehldsHooks->Cvar_DirectSet()->registerHook(ReHooks::Cvar_DirectSetHook);
              },
              [&rehldsHooks]()
              {
                  rehldsHooks->Cvar_DirectSet()->unregisterHook(ReHooks::Cvar_DirectSetHook);
              })),
          m_infoKeyValueRegistry(std::make_unique<InfoKeyValueHookRegistry>()),
          m_cmdArgvRegistry(std::make_unique<CmdArgvHookRegistry>()),
          m_cmdArgsRegistry(std::make_unique<CmdArgsHookRegistry>()),
          m_cmdArgcRegistry(std::make_unique<CmdArgcHookRegistry>()),
          m_registerCvarRegistry(std::make_unique<RegisterCvarHookRegistry>()),
          m_getCvarRegistry(std::make_unique<GetCvarHookRegistry>()),
          m_setModelRegistry(std::make_unique<SetModelHookRegistry>()),
          m_createEntityRegistry(std::make_unique<CreateEntityHookRegistry>()),
          m_removeEntityRegistry(std::make_unique<RemoveEntityHookRegistry>()),
          m_alertRegistry(std::make_unique<AlertHookRegistry>()),
          m_serverPrintRegistry(std::make_unique<ServerPrintHookRegistry>()),
          m_isDedicatedRegistry(std::make_unique<IsDedicatedHookRegistry>()),
          m_checkEngParmRegistry(std::make_unique<CheckEngParmHookRegistry>()),
          m_queryClientCvarValueRegistry(std::make_unique<QueryClientCvarValueHookRegistry>()),
          m_queryClientCvarValue2Registry(std::make_unique<QueryClientCvarValue2HookRegistry>()),
          m_cvarDirectSetRegistry(std::make_unique<CvarDirectSetHookRegistry>()),
          m_indexOfEdictRegistry(std::make_unique<IndexOfEdictHookRegistry>()),
          m_gameDirRegistry(std::make_unique<GetGameDirHookRegistry>()),
          m_getCvarValueRegistry(std::make_unique<GetCvarValueHookRegistry>()),
          m_getCvarStringRegistry(std::make_unique<GetCvarStringHookRegistry>()),
          m_setCvarValueRegistry(std::make_unique<SetCvarValueHookRegistry>()),
          m_setCvarStringRegistry(std::make_unique<SetCvarStringHookRegistry>()),
          m_getEntOffsetRegistry(std::make_unique<GetEntityOffsetHookRegistry>()),
          m_getEntityOfEntOffsetRegistry(std::make_unique<GetEntityOfEntityOffsetHookRegistry>()),
          m_getEntityOfEntIdRegistry(std::make_unique<GetEntityOfEntityIdHookRegistry>()),
          m_allocEntPrivDataRegistry(std::make_unique<AllocEntPrivateDataHookRegistry>()),
          m_edAllocRegistry(std::make_unique<EdAllocHookRegistry>()),
          m_stringFromOffsetRegistry(std::make_unique<StringFromOffsetHookRegistry>()),
          m_strAllocRegistry(std::make_unique<AllocStringHookRegistry>()),
          m_modelIndexHookRegistry(std::make_unique<ModelIndexHookRegistry>()),
          m_randomLongHookRegistry(std::make_unique<RandomLongHookRegistry>()),
          m_randomFloatHookRegistry(std::make_unique<RandomFloatHookRegistry>()),
          m_clientPrintHookRegistry(std::make_unique<ClientPrintHookRegistry>()),
          m_entIsOnFloorHookRegistry(std::make_unique<EntIsOnFloorHookRegistry>()),
          m_dropToFloorHookRegistry(std::make_unique<DropToFloorHookRegistry>()),
          m_emitSoundHookRegistry(std::make_unique<EmitSoundHookRegistry>()),
          m_emitAmbientSoundRegistry(std::make_unique<EmitAmbientSoundRegistry>()),
          m_traceLineHookRegistry(std::make_unique<TraceLineHookRegistry>()),
          m_traceTossHookRegistry(std::make_unique<TraceTossHookRegistry>()),
          m_traceMonsterHullHookRegistry(std::make_unique<TraceMonsterHullHookRegistry>()),
          m_traceHullHookRegistry(std::make_unique<TraceHullHookRegistry>()),
          m_traceModelHookRegistry(std::make_unique<TraceModelHookRegistry>()),
          m_traceTextureHookRegistry(std::make_unique<TraceTextureHookRegistry>()),
          m_traceSphereHookRegistry(std::make_unique<TraceSphereHookRegistry>()),
          m_setOriginHookRegistry(std::make_unique<SetOriginHookRegistry>()),
          m_setSizeHookRegistry(std::make_unique<SetSizeHookRegistry>()),
          m_createNamedEntityHookRegistry(std::make_unique<CreateNamedEntityHookRegistry>())
    {
    }

    nstd::observer_ptr<IPrecacheModelHookRegistry> Hooks::precacheModel()
    {
        return m_precacheModelRegistry;
    }

    nstd::observer_ptr<IPrecacheSoundHookRegistry> Hooks::precacheSound()
    {
        return m_precacheSoundRegistry;
    }

    nstd::observer_ptr<IPrecacheGenericHookRegistry> Hooks::precacheGeneric()
    {
        return m_precacheGenericRegistry;
    }

    nstd::observer_ptr<IChangeLevelHookRegistry> Hooks::changeLevel()
    {
        return m_changeLevelRegistry;
    }

    nstd::observer_ptr<ISrvCmdHookRegistry> Hooks::srvCmd()
    {
        return m_srvCmdRegistry;
    }

    nstd::observer_ptr<ISrvExecHookRegistry> Hooks::srvExec()
    {
        return m_srvExecRegistry;
    }

    nstd::observer_ptr<IRegSrvCmdHookRegistry> Hooks::regSrvCmd()
    {
        return m_regSrvCmdRegistry;
    }

    nstd::observer_ptr<IMessageBeginHookRegistry> Hooks::messageBegin()
    {
        return m_messageBeginRegistry;
    }

    nstd::observer_ptr<IMessageEndHookRegistry> Hooks::messageEnd()
    {
        return m_messageEndRegistry;
    }

    nstd::observer_ptr<IWriteByteHookRegistry> Hooks::writeByte()
    {
        return m_writeByteRegistry;
    }

    nstd::observer_ptr<IWriteCharHookRegistry> Hooks::writeChar()
    {
        return m_writeCharRegistry;
    }

    nstd::observer_ptr<IWriteShortHookRegistry> Hooks::writeShort()
    {
        return m_writeShortRegistry;
    }

    nstd::observer_ptr<IWriteLongHookRegistry> Hooks::writeLong()
    {
        return m_writeLongRegistry;
    }

    nstd::observer_ptr<IWriteEntityHookRegistry> Hooks::writeEntity()
    {
        return m_writeEntityRegistry;
    }

    nstd::observer_ptr<IWriteAngleHookRegistry> Hooks::writeAngle()
    {
        return m_writeAngleRegistry;
    }

    nstd::observer_ptr<IWriteCoordHookRegistry> Hooks::writeCoord()
    {
        return m_writeCoordRegistry;
    }

    nstd::observer_ptr<IWriteStringHookRegistry> Hooks::writeString()
    {
        return m_writeStringRegistry;
    }

    nstd::observer_ptr<IRegUserMsgHookRegistry> Hooks::regUserMsg()
    {
        return m_regUserMsgRegistry;
    }

    nstd::observer_ptr<IGetPlayerAuthIDHookRegistry> Hooks::getPlayerAuthID()
    {
        return m_getPlayerAuthIDRegistry;
    }

    nstd::observer_ptr<IGetPlayerUserIDHookRegistry> Hooks::getPlayerUserID()
    {
        return m_getPlayerUserIDRegistry;
    }

    nstd::observer_ptr<ISVDropClientHookRegistry> Hooks::svDropClient()
    {
        return m_svDropClientRegistry;
    }

    nstd::observer_ptr<ICvarDirectSetHookRegistry> Hooks::cvarDirectSet()
    {
        return m_cvarDirectSetRegistry;
    }

    nstd::observer_ptr<IInfoKeyValueHookRegistry> Hooks::infoKeyValue()
    {
        return m_infoKeyValueRegistry;
    }

    nstd::observer_ptr<ICmdArgvHookRegistry> Hooks::cmdArgv()
    {
        return m_cmdArgvRegistry;
    }

    nstd::observer_ptr<ICmdArgsHookRegistry> Hooks::cmdArgs()
    {
        return m_cmdArgsRegistry;
    }

    nstd::observer_ptr<ICmdArgcHookRegistry> Hooks::cmdArgc()
    {
        return m_cmdArgcRegistry;
    }

    nstd::observer_ptr<IRegisterCvarHookRegistry> Hooks::registerCvar()
    {
        return m_registerCvarRegistry;
    }

    nstd::observer_ptr<IGetCvarHookRegistry> Hooks::getCvar()
    {
        return m_getCvarRegistry;
    }

    nstd::observer_ptr<ISetModelHookRegistry> Hooks::setModel()
    {
        return m_setModelRegistry;
    }

    nstd::observer_ptr<ICreateEntityHookRegistry> Hooks::createEntity()
    {
        return m_createEntityRegistry;
    }

    nstd::observer_ptr<IRemoveEntityHookRegistry> Hooks::removeEntity()
    {
        return m_removeEntityRegistry;
    }

    nstd::observer_ptr<IAlertHookRegistry> Hooks::alert()
    {
        return m_alertRegistry;
    }

    nstd::observer_ptr<IServerPrintHookRegistry> Hooks::serverPrint()
    {
        return m_serverPrintRegistry;
    }

    nstd::observer_ptr<IIsDedicatedHookRegistry> Hooks::isDedicated()
    {
        return m_isDedicatedRegistry;
    }

    nstd::observer_ptr<ICheckEngParmHookRegistry> Hooks::checkEngParm()
    {
        return m_checkEngParmRegistry;
    }

    nstd::observer_ptr<IQueryClientCvarValueHookRegistry> Hooks::queryClientCvarValue()
    {
        return m_queryClientCvarValueRegistry;
    }

    nstd::observer_ptr<IQueryClientCvarValue2HookRegistry> Hooks::queryClientCvarValue2()
    {
        return m_queryClientCvarValue2Registry;
    }

    nstd::observer_ptr<ICvarDirectSetHookRegistry> Hooks::cvarDirectSetRe()
    {
        return m_cvarDirectSetReRegistry;
    }

    nstd::observer_ptr<IIndexOfEdictHookRegistry> Hooks::indexOfEdict()
    {
        return m_indexOfEdictRegistry;
    }

    nstd::observer_ptr<IGetGameDirHookRegistry> Hooks::gameDir()
    {
        return m_gameDirRegistry;
    }

    nstd::observer_ptr<IGetCvarValueHookRegistry> Hooks::getCvarValue()
    {
        return m_getCvarValueRegistry;
    }

    nstd::observer_ptr<IGetCvarStringHookRegistry> Hooks::getCvarString()
    {
        return m_getCvarStringRegistry;
    }

    nstd::observer_ptr<ISetCvarValueHookRegistry> Hooks::setCvarValue()
    {
        return m_setCvarValueRegistry;
    }

    nstd::observer_ptr<ISetCvarStringHookRegistry> Hooks::setCvarString()
    {
        return m_setCvarStringRegistry;
    }

    nstd::observer_ptr<IGetEntityOffsetHookRegistry> Hooks::getEntOffset()
    {
        return m_getEntOffsetRegistry;
    }

    nstd::observer_ptr<IGetEntityOfEntityOffsetHookRegistry> Hooks::getEntityOfEntOffset()
    {
        return m_getEntityOfEntOffsetRegistry;
    }

    nstd::observer_ptr<IGetEntityOfEntityIdHookRegistry> Hooks::getEntityOfEntId()
    {
        return m_getEntityOfEntIdRegistry;
    }

    nstd::observer_ptr<IAllocEntPrivateDataHookRegistry> Hooks::allocEntPrivData()
    {
        return m_allocEntPrivDataRegistry;
    }

    nstd::observer_ptr<IEdAllocHookRegistry> Hooks::edAlloc()
    {
        return m_edAllocRegistry;
    }

    nstd::observer_ptr<IStringFromOffsetHookRegistry> Hooks::stringFromOffset()
    {
        return m_stringFromOffsetRegistry;
    }

    nstd::observer_ptr<IAllocStringHookRegistry> Hooks::strAlloc()
    {
        return m_strAllocRegistry;
    }
    nstd::observer_ptr<IModelIndexHookRegistry> Hooks::modelIndex()
    {
        return m_modelIndexHookRegistry;
    }
    nstd::observer_ptr<IRandomLongHookRegistry> Hooks::randomLong()
    {
        return m_randomLongHookRegistry;
    }
    nstd::observer_ptr<IRandomFloatHookRegistry> Hooks::randomFloat()
    {
        return m_randomFloatHookRegistry;
    }
    nstd::observer_ptr<IClientPrintHookRegistry> Hooks::clientPrint()
    {
        return m_clientPrintHookRegistry;
    }
    nstd::observer_ptr<IEntIsOnFloorHookRegistry> Hooks::entIsOnFloor()
    {
        return m_entIsOnFloorHookRegistry;
    }
    nstd::observer_ptr<IDropToFloorHookRegistry> Hooks::dropToFloor()
    {
        return m_dropToFloorHookRegistry;
    }
    nstd::observer_ptr<IEmitSoundHookRegistry> Hooks::emitSound()
    {
        return m_emitSoundHookRegistry;
    }
    nstd::observer_ptr<IEmitAmbientSoundRegistry> Hooks::emitAmbientSound()
    {
        return m_emitAmbientSoundRegistry;
    }
    nstd::observer_ptr<ITraceLineHookRegistry> Hooks::traceLine()
    {
        return m_traceLineHookRegistry;
    }
    nstd::observer_ptr<ITraceTossHookRegistry> Hooks::traceToss()
    {
        return m_traceTossHookRegistry;
    }
    nstd::observer_ptr<ITraceMonsterHullHookRegistry> Hooks::traceMonsterHull()
    {
        return m_traceMonsterHullHookRegistry;
    }
    nstd::observer_ptr<ITraceHullHookRegistry> Hooks::traceHull()
    {
        return m_traceHullHookRegistry;
    }
    nstd::observer_ptr<ITraceModelHookRegistry> Hooks::traceModel()
    {
        return m_traceModelHookRegistry;
    }
    nstd::observer_ptr<ITraceTextureHookRegistry> Hooks::traceTexture()
    {
        return m_traceTextureHookRegistry;
    }
    nstd::observer_ptr<ITraceSphereHookRegistry> Hooks::traceSphere()
    {
        return m_traceSphereHookRegistry;
    }
    nstd::observer_ptr<ISetOriginHookRegistry> Hooks::setOrigin()
    {
        return m_setOriginHookRegistry;
    }
    nstd::observer_ptr<ISetSizeHookRegistry> Hooks::setSize()
    {
        return m_setSizeHookRegistry;
    }
    nstd::observer_ptr<ICreateNamedEntityHookRegistry> Hooks::createNamedEntity()
    {
        return m_createNamedEntityHookRegistry;
    }
} // namespace Anubis::Engine
