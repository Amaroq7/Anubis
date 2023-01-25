/*
 *  Copyright (C) 2020-2021 Anubis Development Team
 *
 *  This file is part of Anubis.
 *
 *  Anubis is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Anubis is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Anubis.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <Anubis.hpp>

#include "Hooks.hpp"
#include "Library.hpp"
#include "Callbacks.hpp"
#include "TraceResult.hpp"
#include "Cvar.hpp"
#include "Edict.hpp"
#include "ReHooks.hpp"

#include <AnubisCvars.hpp>

#include <memory>

using namespace std::string_literals;

namespace Anubis::Engine
{
    Library::Library(std::unique_ptr<enginefuncs_t> &&engineFuncs, nstd::observer_ptr<globalvars_t> globals)
        : m_engineFuncs(std::make_unique<enginefuncs_t>()),
          m_origEngineFuncs(std::move(engineFuncs)),
          m_engineGlobals(globals),
          m_reHLDSAPI(nullptr),
          m_engineLibrary(nullptr),
          m_reHLDSFuncs(_initReHLDSAPI())
    {
        m_reHookchains = m_reHLDSAPI->GetHookchains();
        m_reServerData = m_reHLDSAPI->GetServerData();
        m_reServerStatic = m_reHLDSAPI->GetServerStatic();

        m_rehldsVersion = {static_cast<std::uint32_t>(m_reHLDSAPI->GetMajorVersion()),
                           static_cast<std::uint32_t>(m_reHLDSAPI->GetMinorVersion())};

        _initGameClients();

        m_hooks = std::make_unique<Hooks>(m_reHookchains);
        Callbacks::GameDLL::getEngine(this);
        _replaceFuncs();

        m_reHLDSFuncs->AddCvarListener(gAnubisLogLevelCvar.name, CvarListener::anubisLogLevel);
        m_reHookchains->ED_Alloc()->registerHook(ReHooks::ED_Alloc);
    }

    nstd::observer_ptr<IEdict> Library::getEdict(std::uint32_t index, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            if (edict_t *edict = m_origEngineFuncs->pfnPEntityOfEntIndex(static_cast<int>(index)); edict)
            {
                return m_edicts.at(index);
            }
            return {};
        }

        static auto hookChain = m_hooks->getEntityOfEntId();

        return hookChain->callChain(
            [this](std::uint32_t index) -> nstd::observer_ptr<IEdict>
            {
                if (edict_t *edict = m_origEngineFuncs->pfnPEntityOfEntIndex(static_cast<int>(index)); edict)
                {
                    return m_edicts.at(index);
                }
                return {};
            },
            index);
    }

    nstd::observer_ptr<IEdict> Library::getEdict(const edict_t *edict) const
    {
        return m_edicts.at(m_origEngineFuncs->pfnIndexOfEdict(edict));
    }

    std::unique_ptr<ITraceResult> Library::createTraceResult()
    {
        return std::make_unique<TraceResult>(this);
    }

    nstd::observer_ptr<IEdict> Library::getEdict(const entvars_t *vars) const
    {
        return m_edicts.at(m_origEngineFuncs->pfnIndexOfEdict(vars->pContainingEntity));
    }

    std::unique_ptr<ITraceResult> Library::createTraceResult(::TraceResult *tr)
    {
        return std::make_unique<TraceResult>(tr, this);
    }

    float Library::getTime() const
    {
        return m_engineGlobals->time;
    }

    std::string_view Library::getMapName() const
    {
        return getString(StringOffset(m_engineGlobals->mapname), FuncCallType::Direct);
    }

    nstd::observer_ptr<const RehldsFuncs_t> Library::_initReHLDSAPI()
    {
        m_engineLibrary = Sys_LoadModule(ENGINE_LIB);
        if (!m_engineLibrary)
        {
            throw std::runtime_error("Failed to locate engine module");
        }

        CreateInterfaceFn ifaceFactory = Sys_GetFactory(m_engineLibrary.get());
        if (!ifaceFactory)
        {
            throw std::runtime_error("Failed to locate interface factory in engine module");
        }

        m_reHLDSAPI = reinterpret_cast<IRehldsApi *>(ifaceFactory(VREHLDS_HLDS_API_VERSION, nullptr));
        if (!m_reHLDSAPI)
        {
            throw std::runtime_error("Failed to retrieve ReHLDS API interface");
        }

        std::uint32_t majorVersion = m_reHLDSAPI->GetMajorVersion();
        std::uint32_t minorVersion = m_reHLDSAPI->GetMinorVersion();

        if (majorVersion != REHLDS_API_VERSION_MAJOR)
        {
            using namespace std::string_literals;
            throw std::runtime_error("ReHLDS API major version mismatch. "s + "Expected " +
                                     std::to_string(REHLDS_API_VERSION_MAJOR) + " got " + std::to_string(majorVersion));
        }

        if (minorVersion < REHLDS_API_VERSION_MINOR)
        {
            using namespace std::string_literals;
            throw std::runtime_error("ReHLDS API minor version mismatch. "s + "Expected at least " +
                                     std::to_string(REHLDS_API_VERSION_MINOR) + " got " + std::to_string(minorVersion));
        }

        return {m_reHLDSAPI->GetFuncs()};
    }

    nstd::observer_ptr<IHooks> Library::getHooks()
    {
        return m_hooks;
    }

    PrecacheId Library::precacheModel(std::string_view model, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return PrecacheId(m_origEngineFuncs->pfnPrecacheModel(model.data()));
        }

        static auto hookChain = m_hooks->precacheModel();

        return hookChain->callChain(
            [this](std::string_view model)
            {
                return PrecacheId(m_origEngineFuncs->pfnPrecacheModel(model.data()));
            },
            model);
    }

    PrecacheId Library::precacheSound(std::string_view sound, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return PrecacheId(m_origEngineFuncs->pfnPrecacheSound(sound.data()));
        }

        static auto hookChain = m_hooks->precacheSound();

        return hookChain->callChain(
            [this](std::string_view sound)
            {
                return PrecacheId(m_origEngineFuncs->pfnPrecacheSound(sound.data()));
            },
            sound);
    }

    PrecacheId Library::precacheGeneric(std::string_view generic, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return PrecacheId(m_origEngineFuncs->pfnPrecacheGeneric(generic.data()));
        }

        static auto hookChain = m_hooks->precacheGeneric();

        return hookChain->callChain(
            [this](std::string_view generic)
            {
                return PrecacheId(m_origEngineFuncs->pfnPrecacheGeneric(generic.data()));
            },
            generic);
    }

    void Library::changeLevel(std::string_view level1, std::string_view level2, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnChangeLevel(level1.data(), level2.data());
        }

        static auto hookChain = m_hooks->changeLevel();

        hookChain->callChain(
            [this](std::string_view level1, std::string_view level2)
            {
                m_origEngineFuncs->pfnChangeLevel(level1.data(), level2.data());
            },
            level1, level2);
    }

    void Library::serverCommand(std::string_view cmd, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnServerCommand(cmd.data());
        }

        static auto hookChain = m_hooks->srvCmd();

        hookChain->callChain(
            [this](std::string_view cmd)
            {
                m_origEngineFuncs->pfnServerCommand(cmd.data());
            },
            cmd);
    }

    void Library::serverExecute(FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnServerExecute();
        }

        static auto hookChain = m_hooks->srvExec();

        hookChain->callChain(
            [this]()
            {
                m_origEngineFuncs->pfnServerExecute();
            });
    }

    void Library::registerSrvCommand(std::string_view cmd, ServerCmdCallback cb, FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            const auto &[iter, inserted] = m_srvCmds.try_emplace(cmd.data(), cb);
            return m_origEngineFuncs->pfnAddServerCommand(cmd.data(), *(iter->second).target<void (*)()>());
        }

        static auto hookChain = m_hooks->regSrvCmd();

        hookChain->callChain(
            [this](std::string_view cmd, ServerCmdCallback cb)
            {
                const auto &[iter, inserted] = m_srvCmds.try_emplace(cmd.data(), cb);
                m_origEngineFuncs->pfnAddServerCommand(cmd.data(), *(iter->second).target<void (*)()>());
            },
            cmd, cb);
    }

    void Library::messageBegin(MsgDest msgDest,
                               MsgType msgType,
                               std::optional<std::array<float, 3>> pOrigin,
                               nstd::observer_ptr<IEdict> pEdict,
                               FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnMessageBegin(static_cast<int>(msgDest), msgType,
                                                      pOrigin ? (*pOrigin).data() : nullptr,
                                                      static_cast<edict_t *>(*pEdict));
        }

        static auto hookChain = m_hooks->messageBegin();

        hookChain->callChain(
            [this](MsgDest msgDest, MsgType msgType, std::optional<std::array<float, 3>> pOrigin,
                   nstd::observer_ptr<IEdict> pEdict)
            {
                m_origEngineFuncs->pfnMessageBegin(static_cast<int>(msgDest), static_cast<int>(msgType),
                                                   pOrigin ? pOrigin.value().data() : nullptr,
                                                   (pEdict) ? static_cast<edict_t *>(*pEdict) : nullptr);
            },
            msgDest, msgType, pOrigin, pEdict);
    }

    void Library::messageEnd(FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnMessageEnd();
        }

        static auto hookChain = m_hooks->messageEnd();

        hookChain->callChain(
            [this]()
            {
                m_origEngineFuncs->pfnMessageEnd();
            });
    }

    void Library::writeByte(std::byte byteArg, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnWriteByte(std::to_integer<int>(byteArg));
        }

        static auto hookChain = m_hooks->writeByte();

        hookChain->callChain(
            [this](std::byte byteArg)
            {
                m_origEngineFuncs->pfnWriteByte(std::to_integer<int>(byteArg));
            },
            byteArg);
    }

    void Library::writeChar(char charArg, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnWriteChar(charArg);
        }

        static auto hookChain = m_hooks->writeChar();

        hookChain->callChain(
            [this](char charArg)
            {
                m_origEngineFuncs->pfnWriteChar(charArg);
            },
            charArg);
    }

    void Library::writeShort(std::int16_t shortArg, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnWriteShort(shortArg);
        }

        static auto hookChain = m_hooks->writeShort();

        hookChain->callChain(
            [this](std::int16_t shortArg)
            {
                m_origEngineFuncs->pfnWriteShort(shortArg);
            },
            shortArg);
    }

    void Library::writeLong(std::int32_t longArg, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnWriteLong(longArg);
        }

        static auto hookChain = m_hooks->writeLong();

        hookChain->callChain(
            [this](std::int32_t longArg)
            {
                m_origEngineFuncs->pfnWriteLong(longArg);
            },
            longArg);
    }

    void Library::writeEntity(MsgEntity entArg, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnWriteEntity(static_cast<int16_t>(entArg));
        }

        static auto hookChain = m_hooks->writeEntity();

        hookChain->callChain(
            [this](MsgEntity entArg)
            {
                m_origEngineFuncs->pfnWriteEntity(static_cast<int16_t>(entArg));
            },
            entArg);
    }

    void Library::writeAngle(MsgAngle angleArg, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnWriteAngle(angleArg);
        }

        static auto hookChain = m_hooks->writeAngle();

        hookChain->callChain(
            [this](MsgAngle angleArg)
            {
                m_origEngineFuncs->pfnWriteAngle(angleArg);
            },
            angleArg);
    }

    void Library::writeCoord(MsgCoord coordArg, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnWriteCoord(coordArg);
        }

        static auto hookChain = m_hooks->writeCoord();

        hookChain->callChain(
            [this](MsgCoord coordArg)
            {
                m_origEngineFuncs->pfnWriteCoord(coordArg);
            },
            coordArg);
    }

    void Library::writeString(std::string_view strArg, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnWriteString(strArg.data());
        }

        static auto hookChain = m_hooks->writeString();

        hookChain->callChain(
            [this](std::string_view strArg)
            {
                m_origEngineFuncs->pfnWriteString(strArg.data());
            },
            strArg);
    }

    MsgType Library::regUserMsg(std::string_view name, MsgSize size, FuncCallType callType) const
    {
        auto engineFn = [this](std::string_view name, MsgSize size)
        {
            MsgType msgType(m_origEngineFuncs->pfnRegUserMsg(name.data(), size));
            gAnubisApi->addNewMsg(msgType, name, size);
            return msgType;
        };

        if (callType == FuncCallType::Direct)
        {
            return engineFn(name, size);
        }

        static auto hookChain = m_hooks->regUserMsg();

        return hookChain->callChain(
            [engineFn](std::string_view name, MsgSize size)
            {
                return engineFn(name, size);
            },
            name, size);
    }

    std::string_view Library::getPlayerAuthID(nstd::observer_ptr<IEdict> pEdict, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnGetPlayerAuthId(static_cast<edict_t *>(*pEdict));
        }

        static auto hookChain = m_hooks->getPlayerAuthID();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> pEdict)
            {
                return m_origEngineFuncs->pfnGetPlayerAuthId(static_cast<edict_t *>(*pEdict));
            },
            pEdict);
    }

    UserID Library::getPlayerUserID(nstd::observer_ptr<IEdict> pEdict, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return UserID(m_origEngineFuncs->pfnGetPlayerUserId(static_cast<edict_t *>(*pEdict)));
        }

        static auto hookChain = m_hooks->getPlayerUserID();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> pEdict)
            {
                return UserID(m_origEngineFuncs->pfnGetPlayerUserId(static_cast<edict_t *>(*pEdict)));
            },
            pEdict);
    }

    std::string_view Library::infoKeyValue(InfoBuffer infobuffer, std::string_view key, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            char *result = m_origEngineFuncs->pfnInfoKeyValue(infobuffer, key.data());
            return (result) ? std::string_view {result} : std::string_view {};
        }

        static auto hookChain = m_hooks->infoKeyValue();

        return hookChain->callChain(
            [this](InfoBuffer infobuffer, std::string_view key)
            {
                char *result = m_origEngineFuncs->pfnInfoKeyValue(infobuffer, key.data());
                return (result) ? std::string_view {result} : std::string_view {};
            },
            infobuffer, key);
    }

    std::string_view Library::cmdArgs(FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            const char *result = m_origEngineFuncs->pfnCmd_Args();
            return (result) ? std::string_view {result} : std::string_view {};
        }

        static auto hookChain = m_hooks->cmdArgs();

        return hookChain->callChain(
            [this]()
            {
                const char *result = m_origEngineFuncs->pfnCmd_Args();
                return (result) ? std::string_view {result} : std::string_view {};
            });
    }

    std::string_view Library::cmdArgv(std::uint8_t argc, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnCmd_Argv(argc);
        }

        static auto hookChain = m_hooks->cmdArgv();

        return hookChain->callChain(
            [this](std::uint8_t argc)
            {
                return m_origEngineFuncs->pfnCmd_Argv(static_cast<int>(argc));
            },
            std::uint8_t {argc});
    }

    std::uint8_t Library::cmdArgc(FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnCmd_Argc();
        }

        static auto hookChain = m_hooks->cmdArgc();

        return hookChain->callChain(
            [this]()
            {
                return static_cast<std::uint8_t>(m_origEngineFuncs->pfnCmd_Argc());
            });
    }

    nstd::observer_ptr<ICvar> Library::getCvar(std::string_view name, FuncCallType callType)
    {
        auto engineFn = [this](std::string_view name) -> nstd::observer_ptr<ICvar>
        {
            if (auto it = m_cvars.find(name.data()); it != m_cvars.end())
            {
                return it->second;
            }

            if (cvar_t *cvar = m_origEngineFuncs->pfnCVarGetPointer(name.data()); cvar)
            {
                const auto &[iter, inserted] =
                    m_cvars.try_emplace(name.data(), std::make_unique<Cvar>(cvar, nstd::make_observer<ILibrary>(this)));
                return iter->second;
            }

            return {};
        };

        if (callType == FuncCallType::Direct)
        {
            return engineFn(name);
        }

        static auto hookChain = m_hooks->getCvar();

        return hookChain->callChain(
            [engineFn](std::string_view name)
            {
                return engineFn(name);
            },
            name);
    }

    void Library::registerCvar(std::string_view name, std::string_view value, FuncCallType callType)
    {
        auto engineFn = [this](std::string_view name, std::string_view value)
        {
            auto cvar = std::make_unique<Cvar>(name, value, nstd::make_observer<ILibrary>(this));
            m_origEngineFuncs->pfnCVarRegister(static_cast<cvar_t *>(*cvar));

            if (m_origEngineFuncs->pfnCVarGetPointer(name.data()))
            {
                m_cvars.try_emplace(name.data(), std::move(cvar));
            }
        };

        if (callType == FuncCallType::Direct)
        {
            return engineFn(name, value);
        }

        static auto hookChain = m_hooks->registerCvar();

        hookChain->callChain(
            [engineFn](std::string_view name, std::string_view value)
            {
                return engineFn(name, value);
            },
            name, value);
    }

    void Library::setModel(nstd::observer_ptr<IEdict> pEdict, std::string_view model, FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnSetModel(static_cast<edict_t *>(*pEdict), model.data());
        }

        static auto hookChain = m_hooks->setModel();

        hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> edict, std::string_view model)
            {
                m_origEngineFuncs->pfnSetModel(static_cast<edict_t *>(*edict), model.data());
            },
            pEdict, model);
    }

    nstd::observer_ptr<IEdict> Library::createEntity(FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return getEdict(m_origEngineFuncs->pfnCreateEntity());
        }

        static auto hookChain = m_hooks->createEntity();

        return hookChain->callChain(
            [this]()
            {
                return getEdict(m_origEngineFuncs->pfnCreateEntity());
            });
    }

    void Library::removeEntity(nstd::observer_ptr<IEdict> pEdict, FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnRemoveEntity(static_cast<edict_t *>(*pEdict));
        }

        static auto hookChain = m_hooks->removeEntity();

        hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> edict)
            {
                m_origEngineFuncs->pfnRemoveEntity(static_cast<edict_t *>(*edict));
            },
            pEdict);
    }

    void Library::alert(AlertType alertType, std::string_view msg, FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnAlertMessage(static_cast<ALERT_TYPE>(alertType), msg.data());
        }

        static auto hookChain = m_hooks->alert();

        hookChain->callChain(
            [this](AlertType alertType, std::string_view msg)
            {
                m_origEngineFuncs->pfnAlertMessage(static_cast<ALERT_TYPE>(alertType), msg.data());
            },
            alertType, msg);
    }

    void Library::registerCvar(cvar_t *cvar)
    {
        static auto hookChain = m_hooks->registerCvar();

        hookChain->callChain(
            [this, cvar](std::string_view name, std::string_view value)
            {
                auto newCvar = std::make_unique<Cvar>(cvar, nstd::make_observer<ILibrary>(this));

                m_origEngineFuncs->pfnCVarRegister(cvar);

                if (m_origEngineFuncs->pfnCVarGetPointer(cvar->name))
                {
                    m_cvars.try_emplace(cvar->name, std::move(newCvar));
                }
            },
            cvar->name, cvar->string);
    }

    void Library::print(std::string_view szMsg, FuncCallType callType)
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnServerPrint(szMsg.data());
        }

        static auto hookChain = m_hooks->serverPrint();

        hookChain->callChain(
            [this](std::string_view msg)
            {
                m_origEngineFuncs->pfnServerPrint(msg.data());
            },
            szMsg);
    }

    bool Library::isDedicatedServer(FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnIsDedicatedServer() == TRUE;
        }

        static auto hookChain = m_hooks->isDedicated();

        return hookChain->callChain(
            [this]()
            {
                return m_origEngineFuncs->pfnIsDedicatedServer() == TRUE;
            });
    }

    std::pair<std::size_t, std::string_view> Library::checkEngParm(std::string_view parm, FuncCallType callType) const
    {
        auto engineFn = [this](std::string_view parm) -> std::pair<std::size_t, std::string_view>
        {
            char *nextParam;
            auto pos = static_cast<std::size_t>(m_origEngineFuncs->pfnEngCheckParm(parm.data(), &nextParam));

            return {pos, nextParam ? nextParam : std::string_view()};
        };

        if (callType == FuncCallType::Direct)
        {
            return engineFn(parm);
        }

        static auto hookChain = m_hooks->checkEngParm();

        return hookChain->callChain(
            [engineFn](std::string_view parm) -> std::pair<std::size_t, std::string_view>
            {
                return engineFn(parm);
            },
            parm);
    }

    void Library::queryClientCvarValue(nstd::observer_ptr<IEdict> player,
                                       std::string_view cvarName,
                                       FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnQueryClientCvarValue(static_cast<edict_t *>(*player), cvarName.data());
        }

        static auto hookChain = m_hooks->queryClientCvarValue();

        hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> player, std::string_view cvarName)
            {
                m_origEngineFuncs->pfnQueryClientCvarValue(static_cast<edict_t *>(*player), cvarName.data());
            },
            player, cvarName);
    }

    void Library::queryClientCvarValue2(nstd::observer_ptr<IEdict> player,
                                        std::string_view cvarName,
                                        std::uint32_t requestID,
                                        FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnQueryClientCvarValue2(static_cast<edict_t *>(*player), cvarName.data(),
                                                               static_cast<int>(requestID));
        }

        static auto hookChain = m_hooks->queryClientCvarValue2();

        hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> player, std::string_view cvarName, std::uint32_t requestID)
            {
                m_origEngineFuncs->pfnQueryClientCvarValue2(static_cast<edict_t *>(*player), cvarName.data(),
                                                            static_cast<int>(requestID));
            },
            player, cvarName, requestID);
    }

    void Library::cvarDirectSet(nstd::observer_ptr<ICvar> cvar, std::string_view value, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnCvar_DirectSet(static_cast<cvar_t *>(*cvar), value.data());
        }

        static auto hookChain = m_hooks->cvarDirectSet();

        hookChain->callChain(
            [this](nstd::observer_ptr<ICvar> cvar, std::string_view value)
            {
                m_origEngineFuncs->pfnCvar_DirectSet(static_cast<cvar_t *>(*cvar), value.data());
            },
            cvar, value);
    }

    std::uint32_t Library::getIndexOfEdict(nstd::observer_ptr<IEdict> edict, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnIndexOfEdict(static_cast<edict_t *>(*edict));
        }

        static auto hookChain = m_hooks->indexOfEdict();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> edict)
            {
                return static_cast<std::uint32_t>(m_origEngineFuncs->pfnIndexOfEdict(static_cast<edict_t *>(*edict)));
            },
            edict);
    }

    std::string Library::getGameDir(FuncCallType callType) const
    {
        auto engineFn = [this]() -> std::string
        {
            char gameDir[MAX_PATH];
            m_origEngineFuncs->pfnGetGameDir(gameDir);
            return gameDir;
        };

        if (callType == FuncCallType::Direct)
        {
            return engineFn();
        }

        static auto hookChain = m_hooks->gameDir();

        return hookChain->callChain(
            [engineFn]()
            {
                return engineFn();
            });
    }

    float Library::getCvarFloat(std::string_view cvarName, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnCVarGetFloat(cvarName.data());
        }

        static auto hookChain = m_hooks->getCvarValue();

        return hookChain->callChain(
            [this](std::string_view cvarName)
            {
                return m_origEngineFuncs->pfnCVarGetFloat(cvarName.data());
            },
            cvarName);
    }

    std::string_view Library::getCvarString(std::string_view cvarName, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnCVarGetString(cvarName.data());
        }

        static auto hookChain = m_hooks->getCvarString();

        return hookChain->callChain(
            [this](std::string_view cvarName)
            {
                return std::string_view {m_origEngineFuncs->pfnCVarGetString(cvarName.data())};
            },
            cvarName);
    }

    void Library::setCvarFloat(std::string_view cvarName, float value, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnCVarSetFloat(cvarName.data(), value);
        }

        static auto hookChain = m_hooks->setCvarValue();

        return hookChain->callChain(
            [this](std::string_view cvarName, float value)
            {
                m_origEngineFuncs->pfnCVarSetFloat(cvarName.data(), value);
            },
            cvarName, value);
    }

    void Library::setCvarString(std::string_view cvarName, std::string_view value, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnCVarSetString(cvarName.data(), value.data());
        }

        static auto hookChain = m_hooks->setCvarString();

        hookChain->callChain(
            [this](std::string_view cvarName, std::string_view value)
            {
                m_origEngineFuncs->pfnCVarSetString(cvarName.data(), value.data());
            },
            cvarName, value);
    }

    EntityOffset Library::getEntityOffset(nstd::observer_ptr<IEdict> edict, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return EntityOffset(m_origEngineFuncs->pfnEntOffsetOfPEntity(static_cast<edict_t *>(*edict)));
        }

        static auto hookChain = m_hooks->getEntOffset();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> edict)
            {
                return EntityOffset(m_origEngineFuncs->pfnEntOffsetOfPEntity(static_cast<edict_t *>(*edict)));
            },
            edict);
    }

    nstd::observer_ptr<IEdict> Library::getEntityOfEntOffset(EntityOffset entOffset, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return getEdict(m_origEngineFuncs->pfnPEntityOfEntOffset(static_cast<int>(entOffset)));
        }

        static auto hookChain = m_hooks->getEntityOfEntOffset();

        return hookChain->callChain(
            [this](EntityOffset entOffset)
            {
                return getEdict(m_origEngineFuncs->pfnPEntityOfEntOffset(static_cast<int>(entOffset)));
            },
            entOffset);
    }

    std::unique_ptr<Game::IBaseEntity> Library::allocEntPrivateData(nstd::observer_ptr<IEdict> edict,
                                                                    std::int32_t classSize,
                                                                    FuncCallType callType) const
    {
        static nstd::observer_ptr<Game::ILibrary> gameLib = gAnubisApi->getGame();
        if (callType == FuncCallType::Direct)
        {
            void *pvData = m_origEngineFuncs->pfnPvAllocEntPrivateData(static_cast<edict_t *>(*edict), classSize);
            if (pvData)
            {
                return gameLib->getBaseEntity(edict);
            }

            return {};
        }

        static auto hookChain = m_hooks->allocEntPrivData();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> edict, std::int32_t classSize) -> std::unique_ptr<Game::IBaseEntity>
            {
                void *pvData = m_origEngineFuncs->pfnPvAllocEntPrivateData(static_cast<edict_t *>(*edict), classSize);
                if (pvData)
                {
                    return gameLib->getBaseEntity(edict);
                }

                return {};
            },
            edict, classSize);
    }

    std::string_view Library::getString(StringOffset offset, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnSzFromIndex(static_cast<int>(offset.value));
        }

        static auto hookChain = m_hooks->stringFromOffset();

        return hookChain->callChain(
            [this](StringOffset offset)
            {
                return m_origEngineFuncs->pfnSzFromIndex(static_cast<int>(offset.value));
            },
            offset);
    }

    [[nodiscard]] StringOffset Library::allocString(std::string_view str, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return StringOffset(m_origEngineFuncs->pfnAllocString(str.data()));
        }

        static auto hookChain = m_hooks->strAlloc();

        return hookChain->callChain(
            [this](std::string_view str)
            {
                return StringOffset(m_origEngineFuncs->pfnAllocString(str.data()));
            },
            str);
    }

    ModelIndex Library::modelIndex(std::string_view model, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return ModelIndex(m_origEngineFuncs->pfnModelIndex(model.data()));
        }

        static auto hookChain = m_hooks->modelIndex();

        return hookChain->callChain(
            [this](std::string_view model)
            {
                return ModelIndex(m_origEngineFuncs->pfnModelIndex(model.data()));
            },
            model);
    }

    std::int32_t Library::randomLong(std::int32_t lLow, std::int32_t lHigh, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnRandomLong(lLow, lHigh);
        }

        static auto hookChain = m_hooks->randomLong();

        return hookChain->callChain(
            [this](std::int32_t lLow, std::int32_t lHigh)
            {
                return m_origEngineFuncs->pfnRandomLong(lLow, lHigh);
            },
            lLow, lHigh);
    }

    float Library::randomFloat(float flLow, float flHigh, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnRandomFloat(flLow, flHigh);
        }

        static auto hookChain = m_hooks->randomFloat();

        return hookChain->callChain(
            [this](float flLow, float flHigh)
            {
                return m_origEngineFuncs->pfnRandomFloat(flLow, flHigh);
            },
            flLow, flHigh);
    }

    void Library::clientPrint(nstd::observer_ptr<IEdict> pEdict,
                              PrintType ptype,
                              std::string_view szMsg,
                              FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnClientPrintf(static_cast<edict_t *>(*pEdict), static_cast<PRINT_TYPE>(ptype),
                                                      szMsg.data());
        }

        static auto hookChain = m_hooks->clientPrint();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> pEdict, PrintType ptype, std::string_view szMsg)
            {
                return m_origEngineFuncs->pfnClientPrintf(static_cast<edict_t *>(*pEdict),
                                                          static_cast<PRINT_TYPE>(ptype), szMsg.data());
            },
            pEdict, ptype, szMsg);
    }

    bool Library::entIsOnFloor(nstd::observer_ptr<IEdict> e, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnEntIsOnFloor(static_cast<edict_t *>(*e));
        }

        static auto hookChain = m_hooks->entIsOnFloor();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> e)
            {
                return m_origEngineFuncs->pfnEntIsOnFloor(static_cast<edict_t *>(*e));
            },
            e);
    }

    std::int8_t Library::dropToFloor(nstd::observer_ptr<IEdict> e, FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnDropToFloor(static_cast<edict_t *>(*e));
        }

        static auto hookChain = m_hooks->dropToFloor();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> e)
            {
                return m_origEngineFuncs->pfnDropToFloor(static_cast<edict_t *>(*e));
            },
            e);
    }

    void Library::emitSound(nstd::observer_ptr<IEdict> entity,
                            Channel channel,
                            std::string_view sample,
                            float volume,
                            float attenuation,
                            SoundFlags fFlags,
                            Pitch pitch,
                            FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnEmitSound(static_cast<edict_t *>(*entity), static_cast<int>(channel),
                                                   sample.data(), volume, attenuation, static_cast<int>(fFlags),
                                                   static_cast<int>(pitch));
        }

        static auto hookChain = m_hooks->emitSound();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> entity, Channel channel, std::string_view sample, float volume,
                   float attenuation, SoundFlags fFlags, Pitch pitch)
            {
                return m_origEngineFuncs->pfnEmitSound(static_cast<edict_t *>(*entity), static_cast<int>(channel),
                                                       sample.data(), volume, attenuation, static_cast<int>(fFlags),
                                                       static_cast<int>(pitch));
            },
            entity, channel, sample, volume, attenuation, fFlags, pitch);
    }

    void Library::emitAmbientSound(nstd::observer_ptr<IEdict> entity,
                                   float *pos,
                                   std::string_view samp,
                                   float vol,
                                   float attenuation,
                                   SoundFlags fFlags,
                                   Pitch pitch,
                                   FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnEmitAmbientSound(static_cast<edict_t *>(*entity), pos, samp.data(), vol,
                                                          attenuation, static_cast<int>(fFlags),
                                                          static_cast<int>(pitch));
        }

        static auto hookChain = m_hooks->emitAmbientSound();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> entity, float *pos, std::string_view samp, float vol, float attenuation,
                   SoundFlags fFlags, Pitch pitch)
            {
                return m_origEngineFuncs->pfnEmitAmbientSound(static_cast<edict_t *>(*entity), pos, samp.data(), vol,
                                                              attenuation, static_cast<int>(fFlags),
                                                              static_cast<int>(pitch));
            },
            entity, pos, samp, vol, attenuation, fFlags, pitch);
    }

    void Library::traceLine(const float *v1,
                            const float *v2,
                            int fNoMonsters,
                            nstd::observer_ptr<IEdict> pentToSkip,
                            nstd::observer_ptr<ITraceResult> ptr,
                            FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnTraceLine(v1, v2, fNoMonsters, static_cast<edict_t *>(*pentToSkip),
                                                   static_cast<::TraceResult *>(*ptr));
        }

        static auto hookChain = m_hooks->traceLine();

        return hookChain->callChain(
            [this](const float *v1, const float *v2, int fNoMonsters, nstd::observer_ptr<IEdict> pentToSkip,
                   nstd::observer_ptr<ITraceResult> ptr)
            {
                return m_origEngineFuncs->pfnTraceLine(v1, v2, fNoMonsters, static_cast<edict_t *>(*pentToSkip),
                                                       static_cast<::TraceResult *>(*ptr));
            },
            v1, v2, fNoMonsters, pentToSkip, ptr);
    }

    void Library::traceToss(nstd::observer_ptr<IEdict> pent,
                            nstd::observer_ptr<IEdict> pentToIgnore,
                            nstd::observer_ptr<ITraceResult> ptr,
                            FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnTraceToss(static_cast<edict_t *>(*pent), static_cast<edict_t *>(*pentToIgnore),
                                                   static_cast<::TraceResult *>(*ptr));
        }

        static auto hookChain = m_hooks->traceToss();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> pent, nstd::observer_ptr<IEdict> pentToIgnore,
                   nstd::observer_ptr<ITraceResult> ptr)
            {
                return m_origEngineFuncs->pfnTraceToss(static_cast<edict_t *>(*pent),
                                                       static_cast<edict_t *>(*pentToIgnore),
                                                       static_cast<::TraceResult *>(*ptr));
            },
            pent, pentToIgnore, ptr);
    }

    bool Library::traceMonsterHull(nstd::observer_ptr<IEdict> pEdict,
                                   const float *v1,
                                   const float *v2,
                                   int fNoMonsters,
                                   nstd::observer_ptr<IEdict> pentToSkip,
                                   nstd::observer_ptr<ITraceResult> ptr,
                                   FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnTraceMonsterHull(static_cast<edict_t *>(*pEdict), v1, v2, fNoMonsters,
                                                          static_cast<edict_t *>(*pentToSkip),
                                                          static_cast<::TraceResult *>(*ptr));
        }

        static auto hookChain = m_hooks->traceMonsterHull();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> pEdict, const float *v1, const float *v2, int fNoMonsters,
                   nstd::observer_ptr<IEdict> pentToSkip, nstd::observer_ptr<ITraceResult> ptr)
            {
                return m_origEngineFuncs->pfnTraceMonsterHull(static_cast<edict_t *>(*pEdict), v1, v2, fNoMonsters,
                                                              static_cast<edict_t *>(*pentToSkip),
                                                              static_cast<::TraceResult *>(*ptr));
            },
            pEdict, v1, v2, fNoMonsters, pentToSkip, ptr);
    }

    void Library::traceHull(const float *v1,
                            const float *v2,
                            int fNoMonsters,
                            int hullNumber,
                            nstd::observer_ptr<IEdict> pentToSkip,
                            nstd::observer_ptr<ITraceResult> ptr,
                            FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnTraceHull(v1, v2, fNoMonsters, hullNumber, static_cast<edict_t *>(*pentToSkip),
                                                   static_cast<::TraceResult *>(*ptr));
        }

        static auto hookChain = m_hooks->traceHull();

        return hookChain->callChain(
            [this](const float *v1, const float *v2, int fNoMonsters, int hullNumber,
                   nstd::observer_ptr<IEdict> pentToSkip, nstd::observer_ptr<ITraceResult> ptr)
            {
                return m_origEngineFuncs->pfnTraceHull(v1, v2, fNoMonsters, hullNumber,
                                                       static_cast<edict_t *>(*pentToSkip),
                                                       static_cast<::TraceResult *>(*ptr));
            },
            v1, v2, fNoMonsters, hullNumber, pentToSkip, ptr);
    }

    void Library::traceModel(const float *v1,
                             const float *v2,
                             int hullNumber,
                             nstd::observer_ptr<IEdict> pent,
                             nstd::observer_ptr<ITraceResult> ptr,
                             FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnTraceModel(v1, v2, hullNumber, static_cast<edict_t *>(*pent),
                                                    static_cast<::TraceResult *>(*ptr));
        }

        static auto hookChain = m_hooks->traceModel();

        return hookChain->callChain(
            [this](const float *v1, const float *v2, int hullNumber, nstd::observer_ptr<IEdict> pent,
                   nstd::observer_ptr<ITraceResult> ptr)
            {
                return m_origEngineFuncs->pfnTraceModel(v1, v2, hullNumber, static_cast<edict_t *>(*pent),
                                                        static_cast<::TraceResult *>(*ptr));
            },
            v1, v2, hullNumber, pent, ptr);
    }

    std::string_view Library::traceTexture(nstd::observer_ptr<IEdict> pTextureEntity,
                                           const float *v1,
                                           const float *v2,
                                           FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnTraceTexture(static_cast<edict_t *>(*pTextureEntity), v1, v2);
        }

        static auto hookChain = m_hooks->traceTexture();

        return hookChain->callChain(
            [this](nstd::observer_ptr<IEdict> pTextureEntity, const float *v1, const float *v2)
            {
                return m_origEngineFuncs->pfnTraceTexture(static_cast<edict_t *>(*pTextureEntity), v1, v2);
            },
            pTextureEntity, v1, v2);
    }

    void Library::traceSphere(const float *v1,
                              const float *v2,
                              int fNoMonsters,
                              float radius,
                              nstd::observer_ptr<IEdict> pentToSkip,
                              nstd::observer_ptr<ITraceResult> ptr,
                              FuncCallType callType) const
    {
        if (callType == FuncCallType::Direct)
        {
            return m_origEngineFuncs->pfnTraceSphere(v1, v2, fNoMonsters, radius, static_cast<edict_t *>(*pentToSkip),
                                                     static_cast<::TraceResult *>(*ptr));
        }

        static auto hookChain = m_hooks->traceSphere();

        return hookChain->callChain(
            [this](const float *v1, const float *v2, int fNoMonsters, float radius,
                   nstd::observer_ptr<IEdict> pentToSkip, nstd::observer_ptr<ITraceResult> ptr)
            {
                return m_origEngineFuncs->pfnTraceSphere(v1, v2, fNoMonsters, radius,
                                                         static_cast<edict_t *>(*pentToSkip),
                                                         static_cast<::TraceResult *>(*ptr));
            },
            v1, v2, fNoMonsters, radius, pentToSkip, ptr);
    }

    void Library::_replaceFuncs()
    {
        *m_engineFuncs = *m_origEngineFuncs;

        // Replace only funcs we want to have hooked
#define ASSIGN_ENG_FUNCS(func) ((*m_engineFuncs).func = Callbacks::GameDLL::func)
        ASSIGN_ENG_FUNCS(pfnPrecacheModel);
        ASSIGN_ENG_FUNCS(pfnPrecacheSound);
        ASSIGN_ENG_FUNCS(pfnPrecacheGeneric);
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
        ASSIGN_ENG_FUNCS(pfnIsDedicatedServer);
        ASSIGN_ENG_FUNCS(pfnEngCheckParm);
        ASSIGN_ENG_FUNCS(pfnQueryClientCvarValue);
        ASSIGN_ENG_FUNCS(pfnQueryClientCvarValue2);
        ASSIGN_ENG_FUNCS(pfnCvar_DirectSet);
        ASSIGN_ENG_FUNCS(pfnIndexOfEdict);
        ASSIGN_ENG_FUNCS(pfnGetGameDir);
        ASSIGN_ENG_FUNCS(pfnCVarGetFloat);
        ASSIGN_ENG_FUNCS(pfnCVarGetString);
        ASSIGN_ENG_FUNCS(pfnCVarSetFloat);
        ASSIGN_ENG_FUNCS(pfnCVarSetString);
        ASSIGN_ENG_FUNCS(pfnPEntityOfEntOffset);
        ASSIGN_ENG_FUNCS(pfnEntOffsetOfPEntity);
        ASSIGN_ENG_FUNCS(pfnPEntityOfEntIndex);
        ASSIGN_ENG_FUNCS(pfnPvAllocEntPrivateData);
        ASSIGN_ENG_FUNCS(pfnSzFromIndex);
        ASSIGN_ENG_FUNCS(pfnAllocString);
        ASSIGN_ENG_FUNCS(pfnModelIndex);
        ASSIGN_ENG_FUNCS(pfnRandomLong);
        ASSIGN_ENG_FUNCS(pfnRandomFloat);
        ASSIGN_ENG_FUNCS(pfnClientPrintf);
        ASSIGN_ENG_FUNCS(pfnEntIsOnFloor);
        ASSIGN_ENG_FUNCS(pfnDropToFloor);
        ASSIGN_ENG_FUNCS(pfnEmitSound);
        ASSIGN_ENG_FUNCS(pfnEmitAmbientSound);
        ASSIGN_ENG_FUNCS(pfnTraceLine);
        ASSIGN_ENG_FUNCS(pfnTraceToss);
        ASSIGN_ENG_FUNCS(pfnTraceMonsterHull);
        ASSIGN_ENG_FUNCS(pfnTraceHull);
        ASSIGN_ENG_FUNCS(pfnTraceModel);
        ASSIGN_ENG_FUNCS(pfnTraceTexture);
        ASSIGN_ENG_FUNCS(pfnTraceSphere);
#undef ASSIGN_ENG_FUNCS
    }

    const std::unique_ptr<enginefuncs_t> &Library::getEngineFuncs() const
    {
        return m_engineFuncs;
    }

    std::array<std::uint32_t, 2> Library::getReHLDSVersion() const
    {
        return m_rehldsVersion;
    }

    bool Library::addExtDll(void *hModule) const
    {
        return m_reHLDSFuncs->AddExtDll(hModule);
    }

    void Library::removeExtDll(void *hModule) const
    {
        m_reHLDSFuncs->RemoveExtDll(hModule);
    }

    nstd::observer_ptr<ICvar> Library::addToCache(cvar_t *cvar)
    {
        if (auto it = m_cvars.find(cvar->name); it != m_cvars.end())
        {
            return it->second;
        }

        const auto &[iter, inserted] = m_cvars.try_emplace(cvar->name, std::make_unique<Cvar>(cvar, this));
        return iter->second;
    }

    void Library::removeCmd(std::string_view cmd_name)
    {
        m_reHLDSFuncs->Cmd_RemoveCmd(cmd_name.data());
        m_srvCmds.erase(cmd_name.data());
    }

    std::uint32_t Library::getWorldmapCrc() const
    {
        return m_reServerData->GetWorldmapCrc();
    }

    ServerState Library::getState() const
    {
        return static_cast<ServerState>(m_reServerData->GetState());
    }

    void Library::initEdict(edict_t *edict)
    {
        m_edicts.try_emplace(m_origEngineFuncs->pfnIndexOfEdict(edict), std::make_unique<Edict>(edict, this));
    }

    void Library::_initGameClients()
    {
        const auto maxClientsLimit = static_cast<std::uint16_t>(m_reServerStatic->GetMaxClientsLimit());
        m_gameClients.reserve(maxClientsLimit);
        for (std::size_t i = 0; i < maxClientsLimit; i++)
        {
            m_gameClients.emplace_back(
                std::make_unique<GameClient>(m_reServerStatic->GetClient(static_cast<int>(i)), this));
        }
    }

    nstd::observer_ptr<IGameClient> Library::getGameClient(std::uint32_t index) const
    {
        return m_gameClients[index];
    }

    std::uint32_t Library::getMaxClients() const
    {
        return static_cast<std::uint32_t>(m_reServerStatic->GetMaxClients());
    }

    std::uint32_t Library::getMaxClientsLimit() const
    {
        return static_cast<std::uint32_t>(m_reServerStatic->GetMaxClientsLimit());
    }

    nstd::observer_ptr<IGameClient> Library::getGameClient(::IGameClient *gameClient) const
    {
        return m_gameClients[gameClient->GetId()];
    }

    std::uint32_t Library::getIndexOfEdict(const edict_t *edict) const
    {
        return m_origEngineFuncs->pfnIndexOfEdict(edict);
    }

    nstd::observer_ptr<globalvars_t> Library::getGlobals() const
    {
        return m_engineGlobals;
    }

    void Library::removeHooks()
    {
        m_reHookchains->ED_Alloc()->unregisterHook(ReHooks::ED_Alloc);
    }

    void Library::initPlayerEdicts()
    {
        const auto maxPlayerEdicts = static_cast<std::uint16_t>(m_reServerStatic->GetMaxClients());
        for (std::size_t i = 1; i <= maxPlayerEdicts; i++)
        {
            m_edicts.try_emplace(i, std::make_unique<Edict>(m_reServerData->GetEdict(static_cast<int>(i)), this));
        }
    }
} // namespace Anubis::Engine
