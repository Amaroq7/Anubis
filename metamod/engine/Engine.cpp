/*
 *  Copyright (C) 2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <Metamod.hpp>

#include "Engine.hpp"
#include "Callbacks.hpp"
#include "ReHooks.hpp"
#include <MetaCvars.hpp>

#include <memory>

using namespace std::string_literals;

namespace Metamod::Engine
{
    Engine::Engine()
        : m_hooks(std::make_unique<Hooks>()),
          m_reHLDSFuncs(_initReHLDSAPI())
    {
        _replaceFuncs();
        _installHooks();

        m_reHLDSFuncs->AddCvarListener(gMetaLogLevelCvar.name, CvarListener::metaLogLevel);
    }

    Edict *Engine::getEdict(std::uint32_t index)
    {
        const auto &edictImpl = m_edicts.at(index);

        if (edictImpl)
        {
            return edictImpl.get();
        }

        // check if edict is valid and register it
        if (edict_t *edict = INDEXENT(index); !FNullEnt(edict) || !index)
        {
            return (m_edicts.at(index) = std::make_unique<Edict>(edict)).get();
        }
        return nullptr;
    }

    Edict *Engine::getEdict(edict_t *edict)
    {
        const auto &edictImpl = m_edicts.at(ENTINDEX(edict));

        if (edictImpl)
        {
            return edictImpl.get();
        }

        // check if edict is valid and register it
        if (!FNullEnt(edict) || !ENTINDEX(edict))
        {
            return (m_edicts.at(ENTINDEX(edict)) = std::make_unique<Edict>(edict)).get();
        }
        return nullptr;
    }

    TraceResult *Engine::createTraceResult()
    {
        return m_traceResults.emplace_front().get();
    }

    EntVars *Engine::getEntVars(entvars_t *vars)
    {
        const auto &edictImpl = m_edicts.at(ENTINDEX(ENT(vars)));

        if (edictImpl)
        {
            return edictImpl->getEntVars();
        }

        // check if edict is valid and register it
        if (!FNullEnt(vars) || !OFFSET(vars))
        {
            edict_t *edict = ENT(vars);
            return (m_edicts.at(ENTINDEX(edict)) = std::make_unique<Edict>(edict))->getEntVars();
        }
        return nullptr;
    }

    TraceResult *Engine::createTraceResult(::TraceResult *tr)
    {
        return m_traceResults.emplace_front(std::make_unique<TraceResult>(tr)).get();
    }

    void Engine::freeTraceResult(ITraceResult *tr)
    {
        m_traceResults.remove_if([tr](const std::unique_ptr<TraceResult> &ptr) {
            return ptr.get() == tr;
        });
    }

    float Engine::getTime() const
    {
        return gpGlobals->time;
    }

    std::string_view Engine::getMapName() const
    {
        return STRING(gpGlobals->mapname);
    }

    void Engine::clear(bool uninstallHooks)
    {
        m_traceResults.clear();

        if (uninstallHooks)
        {
            _uninstallHooks();
        }
    }

    void Engine::_installHooks()
    {
        m_reHookchains->SV_DropClient()->registerHook(ReHooks::SV_DropClientHook);
        m_reHookchains->Cvar_DirectSet()->registerHook(ReHooks::Cvar_DirectSetHook);
    }

    void Engine::_uninstallHooks()
    {
        m_reHookchains->SV_DropClient()->unregisterHook(ReHooks::SV_DropClientHook);
        m_reHookchains->Cvar_DirectSet()->unregisterHook(ReHooks::Cvar_DirectSetHook);
    }

    const RehldsFuncs_t *Engine::_initReHLDSAPI()
    {
        CSysModule *engineModule = Sys_LoadModule(ENGINE_LIB);
        if (!engineModule)
        {
            throw std::runtime_error("Failed to locate engine module");
        }

        CreateInterfaceFn ifaceFactory = Sys_GetFactory(engineModule);
        if (!ifaceFactory)
        {
            throw std::runtime_error("Failed to locate interface factory in engine module");
        }

        auto reHLDSApi = reinterpret_cast<IRehldsApi *>(ifaceFactory(VREHLDS_HLDS_API_VERSION, nullptr));
        if (!reHLDSApi)
        {
            throw std::runtime_error("Failed to retrieve ReHLDS API interface");
        }

        std::uint32_t majorVersion = reHLDSApi->GetMajorVersion();
        std::uint32_t minorVersion = reHLDSApi->GetMinorVersion();

        m_rehldsVersion = {majorVersion, minorVersion};

        if (majorVersion != REHLDS_API_VERSION_MAJOR)
        {
            using namespace std::string_literals;
            throw std::runtime_error(
                "ReHLDS API major version mismatch. "s +
                "Expected " + std::to_string(REHLDS_API_VERSION_MAJOR) +
                " got " + std::to_string(majorVersion));
        }

        if (minorVersion < REHLDS_API_VERSION_MINOR)
        {
            using namespace std::string_literals;
            throw std::runtime_error(
                "ReHLDS API minor version mismatch. "s +
                "Expected at least " + std::to_string(REHLDS_API_VERSION_MINOR) +
                " got " + std::to_string(minorVersion));
        }

        m_reHookchains = reHLDSApi->GetHookchains();
        return reHLDSApi->GetFuncs();
    }

    Hooks *Engine::getHooks()
    {
        return m_hooks.get();
    }

    PrecacheId Engine::precacheModel(std::string_view model, FuncCallType callType) const
    {
        static PrecacheModelHookRegistry *hookchain = m_hooks->precacheModel();
        return _execEngineFunc(hookchain, [](std::string_view model) {
            return PrecacheId(PRECACHE_MODEL(model.data()));
        }, callType, model);
    }

    PrecacheId Engine::precacheSound(std::string_view sound, FuncCallType callType) const
    {
        static PrecacheModelHookRegistry *hookchain = m_hooks->precacheSound();
        return _execEngineFunc(hookchain, [](std::string_view sound) {
            return PrecacheId(PRECACHE_SOUND(sound.data()));
        }, callType, sound);
    }

    void Engine::changeLevel(std::string_view level1, std::string_view level2, FuncCallType callType) const
    {
        static ChangeLevelHookRegistry *hookchain = m_hooks->changeLevel();
        _execEngineFunc(hookchain, [](std::string_view level1, std::string_view level2) {
            CHANGE_LEVEL(level1.data(), level2.data());
        }, callType, level1, level2);
    }

    void Engine::serverCommand(std::string_view cmd, FuncCallType callType) const
    {
        static SrvCmdHookRegistry *hookchain = m_hooks->srvCmd();
        _execEngineFunc(hookchain, [](std::string_view cmd) {
            SERVER_COMMAND(cmd.data());
        }, callType, cmd);
    }

    void Engine::serverExecute(FuncCallType callType) const
    {
        static SrvExecHookRegistry *hookchain = m_hooks->srvExec();
        _execEngineFunc(hookchain, []() {
            SERVER_EXECUTE();
        }, callType);
    }

    void Engine::registerSrvCommand(std::string_view cmd, ServerCmdCallback cb, FuncCallType callType) const
    {
        static RegSrvCmdHookRegistry *hookchain = m_hooks->regSrvCmd();
        _execEngineFunc(hookchain, [](std::string_view cmd, ServerCmdCallback cb) {
            std::invoke(g_engfuncs.pfnAddServerCommand, cmd.data(), *cb.target<void(*)()>());
        }, callType, cmd, cb);
    }

    void Engine::messageBegin(MsgDest msgDest, MsgType msgType, const float *pOrigin, IEdict *pEdict, FuncCallType callType) const
    {
        static MessageBeginHookRegistry *hookchain = m_hooks->messageBegin();
        _execEngineFunc(hookchain, [](MsgDest msgDest, std::uint8_t msgType, const float *pOrigin, IEdict *pEdict) {
            MESSAGE_BEGIN(static_cast<int>(msgDest),
                          static_cast<int>(msgType),
                          pOrigin,
                          (pEdict) ? static_cast<edict_t *>(*dynamic_cast<Edict *>(pEdict)) : nullptr);
        }, callType, msgDest, msgType, pOrigin, pEdict);
    }

    void Engine::messageEnd(FuncCallType callType) const
    {
        static MessageEndHookRegistry *hookchain = m_hooks->messageEnd();
        _execEngineFunc(hookchain, []() {
            MESSAGE_END();
        }, callType);
    }

    void Engine::writeByte(std::byte byteArg, FuncCallType callType) const
    {
        static WriteByteHookRegistry *hookchain = m_hooks->writeByte();
        _execEngineFunc(hookchain, [](std::byte byteArg) {
            WRITE_BYTE(std::to_integer<int>(byteArg));
        }, callType, byteArg);
    }

    void Engine::writeChar(char charArg, FuncCallType callType) const
    {
        static WriteCharHookRegistry *hookchain = m_hooks->writeChar();
        _execEngineFunc(hookchain, [](char charArg) {
            WRITE_CHAR(charArg);
        }, callType, charArg);
    }

    void Engine::writeShort(std::int16_t shortArg, FuncCallType callType) const
    {
        static WriteShortHookRegistry *hookchain = m_hooks->writeShort();
        _execEngineFunc(hookchain, [](std::int16_t shortArg) {
            WRITE_SHORT(shortArg);
        }, callType, shortArg);
    }

    void Engine::writeLong(std::int32_t longArg, FuncCallType callType) const
    {
        static WriteLongHookRegistry *hookchain = m_hooks->writeLong();
        _execEngineFunc(hookchain, [](std::int32_t longArg) {
            WRITE_LONG(longArg);
        }, callType, longArg);
    }

    void Engine::writeEntity(MsgEntity entArg, FuncCallType callType) const
    {
        static WriteEntityHookRegistry *hookchain = m_hooks->writeEntity();
        _execEngineFunc(hookchain, [](MsgEntity entArg) {
            WRITE_ENTITY(static_cast<int16_t>(entArg));
        }, callType, entArg);
    }

    void Engine::writeAngle(MsgAngle angleArg, FuncCallType callType) const
    {
        static WriteAngleHookRegistry *hookchain = m_hooks->writeAngle();
        _execEngineFunc(hookchain, [](MsgAngle angleArg) {
            WRITE_ANGLE(angleArg);
        }, callType, angleArg);
    }

    void Engine::writeCoord(MsgCoord coordArg, FuncCallType callType) const
    {
        static WriteCoordHookRegistry *hookchain = m_hooks->writeCoord();
        _execEngineFunc(hookchain, [](MsgCoord coordArg) {
            WRITE_COORD(coordArg);
        }, callType, coordArg);
    }

    void Engine::writeString(std::string_view strArg, FuncCallType callType) const
    {
        static WriteStringHookRegistry *hookchain = m_hooks->writeString();
        _execEngineFunc(hookchain, [](std::string_view strArg) {
            WRITE_STRING(strArg.data());
        }, callType, strArg);
    }

    MsgType Engine::regUserMsg(std::string_view name, std::int16_t size, FuncCallType callType) const
    {
        static RegUserMsgHookRegistry *hookchain = m_hooks->regUserMsg();
        return _execEngineFunc(hookchain, [](std::string_view name, std::int16_t size) {
            MsgType msgType(REG_USER_MSG(name.data(), size));
            gMetaGlobal->addNewMsg(msgType, name, size);
            return msgType;
        }, callType, name, size);
    }
    
    std::string_view Engine::getPlayerAuthID(IEdict *pEdict, FuncCallType callType) const
    {
        static GetPlayerAuthIDHookRegistry *hookchain = m_hooks->getPlayerAuthID();
        return _execEngineFunc(hookchain, [](IEdict *pEdict) -> std::string_view {
            return GETPLAYERAUTHID(*dynamic_cast<Edict *>(pEdict));
        }, callType, pEdict);
    }
    
    UserID Engine::getPlayerUserID(IEdict *pEdict, FuncCallType callType) const
    {
        static GetPlayerUserIDHookRegistry *hookchain = m_hooks->getPlayerUserID();
        return _execEngineFunc(hookchain, [](IEdict *pEdict) {
            return UserID(GETPLAYERUSERID(*dynamic_cast<Edict *>(pEdict)));
        }, callType, pEdict);
    }
    
    std::string_view Engine::infoKeyValue(InfoBuffer infobuffer, std::string_view key, FuncCallType callType) const
    {
        static InfoKeyValueHookRegistry *hookchain = m_hooks->infoKeyValue();
        return _execEngineFunc(hookchain, [](InfoBuffer infobuffer, std::string_view key) {
            return std::string_view(std::invoke(g_engfuncs.pfnInfoKeyValue, infobuffer, key.data()));
        }, callType, infobuffer, key);
    }
    
    std::string_view Engine::cmdArgs(FuncCallType callType) const
    {
        static CmdArgsHookRegistry *hookchain = m_hooks->cmdArgs();
        return _execEngineFunc(hookchain, []() {
            return std::string_view(CMD_ARGS());
        }, callType);
    }
    
    std::string_view Engine::cmdArgv(std::uint8_t argc, FuncCallType callType) const
    {
        static CmdArgvHookRegistry *hookchain = m_hooks->cmdArgv();
        return _execEngineFunc(hookchain, [](std::uint8_t argc) {
            return std::string_view(CMD_ARGV(static_cast<int>(argc)));
        }, callType, argc);
    }
    
    std::uint8_t Engine::cmdArgc(FuncCallType callType) const
    {
        static CmdArgcHookRegistry *hookchain = m_hooks->cmdArgc();
        return _execEngineFunc(hookchain, []() {
            return static_cast<std::uint8_t>(CMD_ARGC());
        }, callType);
    }
    
    Cvar *Engine::getCvar(std::string_view name, FuncCallType callType)
    {
        static GetCvarHookRegistry *hookchain = m_hooks->getCvar();
        return static_cast<Cvar *>(_execEngineFunc(hookchain, [this](std::string_view name) -> ICvar * {
            if (auto it = m_cvars.find(name.data()); it != m_cvars.end())
            {
                return it->second.get();
            }

            if (cvar_t *cvar = CVAR_GET_POINTER(name.data()); cvar)
            {
                const auto &[iter, inserted] = m_cvars.emplace(name, std::make_unique<Cvar>(cvar));
                return iter->second.get();
            }

            return nullptr;
        }, callType, name));
    }

    void Engine::registerCvar(std::string_view name, std::string_view value, FuncCallType callType)
    {
        static RegisterCvarHookRegistry *hookchain = m_hooks->registerCvar();
        return _execEngineFunc(hookchain, [this](std::string_view name, std::string_view value) {
            auto cvar = std::make_unique<Cvar>(name, value);
            CVAR_REGISTER(*cvar.get());

            if (CVAR_GET_POINTER(name.data()))
            {
                m_cvars.emplace(name, std::move(cvar));
            }
        }, callType, name, value);
    }

    void Engine::setModel(IEdict *pEdict, std::string_view model, FuncCallType callType)
    {
        static SetModelHookRegistry *hookchain = m_hooks->setModel();
        return _execEngineFunc(hookchain, [](IEdict *edict, std::string_view model) {
            SET_MODEL(*dynamic_cast<Edict *>(edict), model.data());
        }, callType, pEdict, model);
    }

    Edict *Engine::createEntity(FuncCallType callType)
    {
        static CreateEntityHookRegistry *hookchain = m_hooks->createEntity();
        return static_cast<Edict *>(_execEngineFunc(hookchain, [this]() -> IEdict * {
            return getEdict(CREATE_ENTITY());
        }, callType));
    }

    void Engine::removeEntity(IEdict *pEdict, FuncCallType callType)
    {
        static RemoveEntityHookRegistry *hookchain = m_hooks->removeEntity();
        return _execEngineFunc(hookchain, [](IEdict *edict) {
            REMOVE_ENTITY(*dynamic_cast<Edict *>(edict));
        }, callType, pEdict);
    }

    void Engine::_replaceFuncs()
    {
        m_engineFuncs = g_engfuncs;

        // Replace only funcs we want to have hooked
#define ASSIGN_ENG_FUNCS(func) (m_engineFuncs.func = Callbacks::GameDLL::func)
        ASSIGN_ENG_FUNCS(pfnPrecacheModel);
        ASSIGN_ENG_FUNCS(pfnPrecacheSound);
        ASSIGN_ENG_FUNCS(pfnChangeLevel);
        ASSIGN_ENG_FUNCS(pfnServerCommand);
        ASSIGN_ENG_FUNCS(pfnServerExecute);
        ASSIGN_ENG_FUNCS(pfnAddServerCommand);
        ASSIGN_ENG_FUNCS(pfnMessageBegin);
        ASSIGN_ENG_FUNCS(pfnMessageEnd);
        ASSIGN_ENG_FUNCS(pfnWriteByte);
        ASSIGN_ENG_FUNCS(pfnWriteChar);
        ASSIGN_ENG_FUNCS(pfnWriteShort);
        ASSIGN_ENG_FUNCS(pfnWriteLong);
        ASSIGN_ENG_FUNCS(pfnWriteAngle);
        ASSIGN_ENG_FUNCS(pfnWriteCoord);
        ASSIGN_ENG_FUNCS(pfnWriteString);
        ASSIGN_ENG_FUNCS(pfnWriteEntity);
        ASSIGN_ENG_FUNCS(pfnRegUserMsg);
        ASSIGN_ENG_FUNCS(pfnGetPlayerUserId);
        ASSIGN_ENG_FUNCS(pfnGetPlayerAuthId);
        ASSIGN_ENG_FUNCS(pfnInfoKeyValue);
        ASSIGN_ENG_FUNCS(pfnCmd_Args);
        ASSIGN_ENG_FUNCS(pfnCmd_Argv);
        ASSIGN_ENG_FUNCS(pfnCmd_Argc);
        ASSIGN_ENG_FUNCS(pfnCVarRegister);
        ASSIGN_ENG_FUNCS(pfnCVarGetPointer);
        ASSIGN_ENG_FUNCS(pfnSetModel);
        ASSIGN_ENG_FUNCS(pfnCreateEntity);
        ASSIGN_ENG_FUNCS(pfnRemoveEntity);
#undef ASSIGN_ENG_FUNCS
    }

    const enginefuncs_t *Engine::getEngineFuncs() const
    {
        return &m_engineFuncs;
    }

    void Engine::initializeEdicts(edict_t *pEdictList, std::uint32_t edictsNum, std::uint32_t clientMax [[maybe_unused]])
    {
        m_edictList = pEdictList;
        for (std::size_t i = 0; i < edictsNum; i++)
        {
            edict_t *edict = pEdictList + i;

            if (edict->free || !edict->pvPrivateData)
            {
                continue;
            }

            m_edicts.at(i) = std::make_unique<Edict>(edict);
        }
    }

    edict_t *Engine::getEngineEdictList() const
    {
        return m_edictList;
    }

    const std::array<std::uint32_t, 2> &Engine::getReHLDSVersion() const
    {
        return m_rehldsVersion;
    }

    bool Engine::addExtDll(void *hModule) const
    {
        return m_reHLDSFuncs->AddExtDll(hModule);
    }

    void Engine::removeExtDll(void *hModule) const
    {
       m_reHLDSFuncs->RemoveExtDll(hModule);
    }

    Engine::~Engine()
    {
        _uninstallHooks();
    }

    Cvar *Engine::registerCvar(cvar_t *cvar)
    {
        const auto &[iter, inserted] = m_cvars.emplace(cvar->name, std::make_unique<Cvar>(cvar));

        return iter->second.get();
    }
} // namespace Metamod::Engine
