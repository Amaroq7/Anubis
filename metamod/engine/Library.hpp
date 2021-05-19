/*
 *  Copyright (C) 2020-2021 Metamod++ Development Team
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

#include <Common.hpp>
#include <engine/ILibrary.hpp>

#include "Edict.hpp"
#include "TraceResult.hpp"
#include "Hooks.hpp"
#include "Cvar.hpp"
#include <osconfig.h>
#include <usercmd.h>
#include <rehlds_api.h>
#include <engine_hlds_api.h>

#include <unordered_map>

namespace Metamod::Engine
{
    class Library final : public ILibrary
    {
    public:
        Library();
        Library(const Library &other) = delete;
        Library(Library &&other) = delete;
        ~Library() override;

        // IEngine
        Edict *getEdict(std::uint32_t index) override;
        TraceResult *createTraceResult() override;
        void freeTraceResult(ITraceResult *tr) override;
        Hooks *getHooks() override;

        PrecacheId precacheModel(std::string_view model, FuncCallType callType) const override;
        PrecacheId precacheSound(std::string_view sound, FuncCallType callType) const override;
        void changeLevel(std::string_view level1, std::string_view level2, FuncCallType callType) const override;
        void serverCommand(std::string_view cmd, FuncCallType callType) const override;
        void serverExecute(FuncCallType callType) const override;
        void registerSrvCommand(std::string_view cmd, ServerCmdCallback cb, FuncCallType callType) override;
        void messageBegin(MsgDest msgDest,
                          MsgType msgType,
                          const float *pOrigin,
                          IEdict *pEdict, FuncCallType callType) const override;
        void messageEnd(FuncCallType callType) const override;
        void writeByte(std::byte byteArg, FuncCallType callType) const override;
        void writeChar(char charArg, FuncCallType callType) const override;
        void writeShort(std::int16_t shortArg, FuncCallType callType) const override;
        void writeLong(std::int32_t longArg, FuncCallType callType) const override;
        void writeEntity(MsgEntity entArg, FuncCallType callType) const override;
        void writeAngle(MsgAngle angleArg, FuncCallType callType) const override;
        void writeCoord(MsgCoord coordArg, FuncCallType callType) const override;
        void writeString(std::string_view strArg, FuncCallType callType) const override;
        MsgType regUserMsg(std::string_view name, std::int16_t size, FuncCallType callType) const override;
        std::string_view getPlayerAuthID(IEdict *pEdict, FuncCallType callType) const override;
        UserID getPlayerUserID(IEdict *pEdict, FuncCallType callType) const override;
        std::string_view infoKeyValue(InfoBuffer infobuffer, std::string_view key, FuncCallType callType) const override;
        std::string_view cmdArgs(FuncCallType callType) const override;
        std::string_view cmdArgv(std::uint8_t argc, FuncCallType callType) const override;
        std::uint8_t cmdArgc(FuncCallType callType) const override;
        void registerCvar(std::string_view name, std::string_view value, FuncCallType callType) override;
        Cvar *getCvar(std::string_view name, FuncCallType callType) override;
        void setModel(IEdict *pEdict, std::string_view model, FuncCallType callType) override;
        Edict *createEntity(FuncCallType callType) override;
        void removeEntity(IEdict *pEdict, FuncCallType callType) override;
        void alert(AlertType alertType, std::string_view msg, FuncCallType callType) final;
        void removeCmd(std::string_view cmd_name) final;
        void print(std::string_view szMsg, FuncCallType callType) final;

        float getTime() const override;
        std::string_view getMapName() const override;
        
        Edict *getEdict(edict_t *edict) override;
        EntVars *getEntVars(entvars_t *vars) override;
        TraceResult *createTraceResult(::TraceResult *tr) override;
        const std::array<std::uint32_t, 2> &getReHLDSVersion() const;
        bool addExtDll(void *hModule) const override;
        void removeExtDll(void *hModule) const override;

        const enginefuncs_t *getEngineFuncs() const override;
        void clear(bool uninstallHooks = false);
        void initializeEdicts(edict_t *pEdictList, std::uint32_t edictCount, std::uint32_t clientMax);
        edict_t *getEngineEdictList() const;
        Cvar *addToCache(cvar_t *cvar);
        void registerCvar(cvar_t *cvar);

    private:
        template <typename t_hookregistry, typename t_origfunc, typename... t_args>
        auto _execEngineFunc(t_hookregistry *hookchain, t_origfunc origFunc, FuncCallType callType, t_args... args) const
        {
            static_assert(std::is_invocable_v<t_origfunc, t_args...>, "Invalid function passed");
            if (callType == FuncCallType::Direct)
            {
                return std::invoke(origFunc, std::forward<t_args>(args)...);
            }

            return hookchain->callChain(origFunc, std::forward<t_args>(args)...);
        }

        void _replaceFuncs();
        const RehldsFuncs_t *_initReHLDSAPI();
        void _installHooks();
        void _uninstallHooks();

    private:
        std::unique_ptr<Hooks> m_hooks;
        const RehldsFuncs_t *m_reHLDSFuncs;
        IRehldsHookchains *m_reHookchains;
        std::array<std::uint32_t, 2> m_rehldsVersion;
        std::array<std::unique_ptr<Edict>, MAX_EDICTS> m_edicts;
        std::forward_list<std::unique_ptr<TraceResult>> m_traceResults;
        std::unordered_map<std::string, std::unique_ptr<Cvar>> m_cvars;
        std::unordered_map<std::string, ServerCmdCallback> m_srvCmds;
        enginefuncs_t m_engineFuncs;
        edict_t *m_edictList;
    };
} // namespace Metamod::Engine
