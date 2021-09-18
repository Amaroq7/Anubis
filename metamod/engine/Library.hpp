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

struct RehldsFuncs_t;
class IRehldsHookchains;
class IRehldsServerData;
class IRehldsApi;

#include <osconfig.h>
#include <extdll.h>
#include <util.h>
#include <usercmd.h>

#include "Hooks.hpp"
#include "TraceResult.hpp"
#include "Edict.hpp"
#include "EntVars.hpp"
#include "GameClient.hpp"
#include "Cvar.hpp"

#include <unordered_map>
#include <forward_list>
#include <array>

namespace Metamod::Engine
{
    class Edict;
    class Cvar;
    class Hooks;
    class TraceResult;

    class Library final : public ILibrary
    {
    public:
        Library();
        Library(const Library &other) = delete;
        Library(Library &&other) = delete;
        ~Library() final = default;

        // IEngine
        Edict *getEdict(std::uint32_t index) final;
        TraceResult *createTraceResult() final;
        void freeTraceResult(ITraceResult *tr) final;
        Hooks *getHooks() final;
        [[nodiscard]] std::array<std::uint32_t, 2> getReHLDSVersion() const final;

        [[nodiscard]] PrecacheId precacheModel(std::string_view model, FuncCallType callType) const final;
        [[nodiscard]] PrecacheId precacheSound(std::string_view sound, FuncCallType callType) const final;
        [[nodiscard]] PrecacheId precacheGeneric(std::string_view generic, FuncCallType callType) const final;
        void changeLevel(std::string_view level1, std::string_view level2, FuncCallType callType) const final;
        void serverCommand(std::string_view cmd, FuncCallType callType) const final;
        void serverExecute(FuncCallType callType) const final;
        void registerSrvCommand(std::string_view cmd, ServerCmdCallback cb, FuncCallType callType) final;
        void messageBegin(MsgDest msgDest,
                          MsgType msgType,
                          const float *pOrigin,
                          IEdict *pEdict, FuncCallType callType) const final;
        void messageEnd(FuncCallType callType) const final;
        void writeByte(std::byte byteArg, FuncCallType callType) const final;
        void writeChar(char charArg, FuncCallType callType) const final;
        void writeShort(std::int16_t shortArg, FuncCallType callType) const final;
        void writeLong(std::int32_t longArg, FuncCallType callType) const final;
        void writeEntity(MsgEntity entArg, FuncCallType callType) const final;
        void writeAngle(MsgAngle angleArg, FuncCallType callType) const final;
        void writeCoord(MsgCoord coordArg, FuncCallType callType) const final;
        void writeString(std::string_view strArg, FuncCallType callType) const final;
        [[nodiscard]] MsgType regUserMsg(std::string_view name, std::int16_t size, FuncCallType callType) const final;
        std::string_view getPlayerAuthID(IEdict *pEdict, FuncCallType callType) const final;
        UserID getPlayerUserID(IEdict *pEdict, FuncCallType callType) const final;
        [[nodiscard]] std::string_view infoKeyValue(InfoBuffer infobuffer, std::string_view key, FuncCallType callType) const final;
        [[nodiscard]] std::string_view cmdArgs(FuncCallType callType) const final;
        [[nodiscard]] std::string_view cmdArgv(std::uint8_t argc, FuncCallType callType) const final;
        [[nodiscard]] std::uint8_t cmdArgc(FuncCallType callType) const final;
        void registerCvar(std::string_view name, std::string_view value, FuncCallType callType) final;
        ICvar *getCvar(std::string_view name, FuncCallType callType) final;
        void setModel(IEdict *pEdict, std::string_view model, FuncCallType callType) final;
        IEdict *createEntity(FuncCallType callType) final;
        void removeEntity(IEdict *pEdict, FuncCallType callType) final;
        void alert(AlertType alertType, std::string_view msg, FuncCallType callType) final;
        void removeCmd(std::string_view cmd_name) final;
        void print(std::string_view szMsg, FuncCallType callType) final;

        [[nodiscard]] float getTime() const final;
        [[nodiscard]] std::string_view getMapName() const final;
        
        Edict *getEdict(edict_t *edict) final;
        EntVars *getEntVars(entvars_t *vars) final;
        TraceResult *createTraceResult(::TraceResult *tr) final;
        bool addExtDll(void *hModule) const final;
        void removeExtDll(void *hModule) const final;
        [[nodiscard]] std::uint32_t getWorldmapCrc() const final;
        [[nodiscard]] ServerState getState() const final;

        [[nodiscard]] const enginefuncs_t *getEngineFuncs() const final;
        void initializeEdicts(edict_t *pEdictList, std::uint32_t edictCount, std::uint32_t clientMax) final;
        [[nodiscard]] edict_t *getEngineEdictList() const final;
        [[nodiscard]] bool isDedicatedServer(FuncCallType callType) const final;
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

    private:
        IRehldsApi *m_reHLDSAPI;
        const RehldsFuncs_t *m_reHLDSFuncs = nullptr;
        IRehldsHookchains *m_reHookchains = nullptr;
        IRehldsServerData *m_reServerData = nullptr;
        std::unique_ptr<Hooks> m_hooks;
        std::array<std::uint32_t, 2> m_rehldsVersion = {0u, 0u};
        std::array<std::unique_ptr<Edict>, MAX_EDICTS> m_edicts;
        std::forward_list<std::unique_ptr<TraceResult>> m_traceResults;
        std::unordered_map<std::string, std::unique_ptr<Cvar>> m_cvars;
        std::unordered_map<std::string, ServerCmdCallback> m_srvCmds;
        enginefuncs_t m_engineFuncs = {};
        edict_t *m_edictList = nullptr;
    };
} // namespace Metamod::Engine
