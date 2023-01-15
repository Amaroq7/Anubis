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

#include "GameClient.hpp"
#include "Hooks.hpp"
#include "Cvar.hpp"

#include <rehlds_api.h>
#include <engine_hlds_api.h>

#include <unordered_map>
#include <forward_list>
#include <array>
#include <vector>

namespace Anubis::Engine
{
    class Library final : public ILibrary
    {
    public:
        Library(std::unique_ptr<enginefuncs_t> &&engineFuncs, nstd::observer_ptr<globalvars_t> globals);
        Library(const Library &other) = delete;
        Library(Library &&other) = delete;
        ~Library() final = default;

        [[nodiscard]] nstd::observer_ptr<IEdict> getEdict(std::uint32_t index, FuncCallType callType) const final;
        std::unique_ptr<ITraceResult> createTraceResult() final;
        nstd::observer_ptr<IHooks> getHooks() final;
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
                          std::optional<std::array<float, 3>> pOrigin,
                          nstd::observer_ptr<IEdict> pEdict,
                          FuncCallType callType) const final;
        void messageEnd(FuncCallType callType) const final;
        void writeByte(std::byte byteArg, FuncCallType callType) const final;
        void writeChar(char charArg, FuncCallType callType) const final;
        void writeShort(std::int16_t shortArg, FuncCallType callType) const final;
        void writeLong(std::int32_t longArg, FuncCallType callType) const final;
        void writeEntity(MsgEntity entArg, FuncCallType callType) const final;
        void writeAngle(MsgAngle angleArg, FuncCallType callType) const final;
        void writeCoord(MsgCoord coordArg, FuncCallType callType) const final;
        void writeString(std::string_view strArg, FuncCallType callType) const final;
        [[nodiscard]] MsgType regUserMsg(std::string_view name, MsgSize size, FuncCallType callType) const final;
        [[nodiscard]] std::string_view getPlayerAuthID(nstd::observer_ptr<IEdict> pEdict,
                                                       FuncCallType callType) const final;
        [[nodiscard]] UserID getPlayerUserID(nstd::observer_ptr<IEdict> pEdict, FuncCallType callType) const final;
        [[nodiscard]] std::string_view
            infoKeyValue(InfoBuffer infobuffer, std::string_view key, FuncCallType callType) const final;
        [[nodiscard]] std::string_view cmdArgs(FuncCallType callType) const final;
        [[nodiscard]] std::string_view cmdArgv(std::uint8_t argc, FuncCallType callType) const final;
        [[nodiscard]] std::uint8_t cmdArgc(FuncCallType callType) const final;
        void registerCvar(std::string_view name, std::string_view value, FuncCallType callType) final;
        nstd::observer_ptr<ICvar> getCvar(std::string_view name, FuncCallType callType) final;
        void setModel(nstd::observer_ptr<IEdict> pEdict, std::string_view model, FuncCallType callType) final;
        nstd::observer_ptr<IEdict> createEntity(FuncCallType callType) final;
        void removeEntity(nstd::observer_ptr<IEdict> pEdict, FuncCallType callType) final;
        void alert(AlertType alertType, std::string_view msg, FuncCallType callType) final;
        void removeCmd(std::string_view cmd_name) final;
        void print(std::string_view szMsg, FuncCallType callType) final;
        [[nodiscard]] std::string_view getString(StringOffset offset, FuncCallType callType) const final;
        void cvarDirectSet(nstd::observer_ptr<ICvar> cvar, std::string_view value, FuncCallType callType) const final;

        [[nodiscard]] float getTime() const final;
        [[nodiscard]] std::string_view getMapName() const final;

        nstd::observer_ptr<IEdict> getEdict(const edict_t *edict) const final;
        nstd::observer_ptr<IEdict> getEdict(const entvars_t *vars) const final;
        std::unique_ptr<ITraceResult> createTraceResult(::TraceResult *tr) final;
        bool addExtDll(void *hModule) const final;
        void removeExtDll(void *hModule) const final;
        [[nodiscard]] std::uint32_t getWorldmapCrc() const final;
        [[nodiscard]] ServerState getState() const final;

        [[nodiscard]] const std::unique_ptr<enginefuncs_t> &getEngineFuncs() const final;
        [[nodiscard]] bool isDedicatedServer(FuncCallType callType) const final;
        [[nodiscard]] std::pair<std::size_t, std::string_view> checkEngParm(std::string_view parm,
                                                                            FuncCallType callType) const final;
        void queryClientCvarValue(nstd::observer_ptr<IEdict> player,
                                  std::string_view cvarName,
                                  FuncCallType callType) const final;
        void queryClientCvarValue2(nstd::observer_ptr<IEdict> player,
                                   std::string_view cvarName,
                                   std::uint32_t requestID,
                                   FuncCallType callType) const final;
        [[nodiscard]] nstd::observer_ptr<IGameClient> getGameClient(std::uint32_t index) const final;
        [[nodiscard]] std::uint32_t getMaxClients() const final;
        [[nodiscard]] std::uint32_t getMaxClientsLimit() const final;
        nstd::observer_ptr<ICvar> addToCache(cvar_t *cvar) final;
        void registerCvar(cvar_t *cvar);
        void initEdict(edict_t *edict) final;
        [[nodiscard]] nstd::observer_ptr<IGameClient> getGameClient(::IGameClient *gameClient) const final;
        [[nodiscard]] std::uint32_t getIndexOfEdict(nstd::observer_ptr<IEdict> edict,
                                                    FuncCallType callType) const final;
        [[nodiscard]] std::uint32_t getIndexOfEdict(const edict_t *edict) const final;
        [[nodiscard]] std::string getGameDir(FuncCallType callType) const final;
        [[nodiscard]] float getCvarFloat(std::string_view cvarName, FuncCallType callType) const final;
        [[nodiscard]] std::string_view getCvarString(std::string_view cvarName, FuncCallType callType) const final;
        void setCvarFloat(std::string_view cvarName, float value, FuncCallType callType) const final;
        void setCvarString(std::string_view cvarName, std::string_view value, FuncCallType callType) const final;
        [[nodiscard]] nstd::observer_ptr<globalvars_t> getGlobals() const final;
        [[nodiscard]] EntityOffset getEntityOffset(nstd::observer_ptr<IEdict> edict, FuncCallType callType) const final;
        [[nodiscard]] nstd::observer_ptr<IEdict> getEntityOfEntOffset(EntityOffset entOffset,
                                                                      FuncCallType callType) const final;

        [[nodiscard]] nstd::observer_ptr<Game::IBaseEntity> allocEntPrivateData(nstd::observer_ptr<IEdict> edict,
                                                                                std::int32_t classSize,
                                                                                FuncCallType callType) const final;

        [[nodiscard]] StringOffset allocString(std::string_view str, FuncCallType callType) const final;

        ModelIndex modelIndex (std::string_view model, FuncCallType callType) const final;
        std::int32_t randomLong (std::int32_t lLow, std::int32_t  lHigh, FuncCallType callType) const final;
	    float randomFloat (float flLow, float flHigh, FuncCallType callType) const final;

        void clientPrint (nstd::observer_ptr<IEdict> pEdict, PrintType ptype, std::string_view szMsg, FuncCallType callType) const final;

        bool entIsOnFloor (nstd::observer_ptr<IEdict> e, FuncCallType callType) const final;
	    std::int8_t dropToFloor (nstd::observer_ptr<IEdict> e, FuncCallType callType) const final;
        void emitSound(nstd::observer_ptr<IEdict> entity, Channel channel, std::string_view sample, float volume, float attenuation, SoundFlags fFlags, Pitch pitch, FuncCallType callType) const final;
        void emitAmbientSound (nstd::observer_ptr<IEdict> entity, float *pos, std::string_view samp, float vol, float attenuation, SoundFlags fFlags, Pitch pitch, FuncCallType callType) const final;
        void traceLine (const float *v1, const float *v2, int fNoMonsters, nstd::observer_ptr<IEdict> pentToSkip, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const final;
        void traceToss (nstd::observer_ptr<IEdict> pent, nstd::observer_ptr<IEdict> pentToIgnore, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const final;
        bool traceMonsterHull (nstd::observer_ptr<IEdict> pEdict, const float *v1, const float *v2, int fNoMonsters, nstd::observer_ptr<IEdict> pentToSkip, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const final;
        void traceHull (const float *v1, const float *v2, int fNoMonsters, int hullNumber, nstd::observer_ptr<IEdict> pentToSkip, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const final;
        void traceModel (const float *v1, const float *v2, int hullNumber, nstd::observer_ptr<IEdict> pent, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const final;
        std::string_view traceTexture (nstd::observer_ptr<IEdict> pTextureEntity, const float *v1, const float *v2, FuncCallType callType) const final;
        void traceSphere (const float *v1, const float *v2, int fNoMonsters, float radius, nstd::observer_ptr<IEdict> pentToSkip, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const final;

        void removeHooks() final;
        void initPlayerEdicts();

    private:
        void _initGameClients();
        void _replaceFuncs();
        nstd::observer_ptr<const RehldsFuncs_t> _initReHLDSAPI();

    private:
        std::unique_ptr<enginefuncs_t> m_engineFuncs;
        std::unique_ptr<enginefuncs_t> m_origEngineFuncs;
        nstd::observer_ptr<globalvars_t> m_engineGlobals;
        nstd::observer_ptr<IRehldsApi> m_reHLDSAPI;
        nstd::observer_ptr<CSysModule> m_engineLibrary;
        nstd::observer_ptr<const RehldsFuncs_t> m_reHLDSFuncs;
        nstd::observer_ptr<IRehldsHookchains> m_reHookchains;
        nstd::observer_ptr<IRehldsServerData> m_reServerData;
        nstd::observer_ptr<IRehldsServerStatic> m_reServerStatic;
        std::unique_ptr<Hooks> m_hooks;
        std::array<std::uint32_t, 2> m_rehldsVersion = {0u, 0u};
        std::unordered_map<std::uint32_t, std::unique_ptr<IEdict>> m_edicts;
        std::unordered_map<std::string, std::unique_ptr<ICvar>> m_cvars;
        std::unordered_map<std::string, ServerCmdCallback> m_srvCmds;
        std::vector<std::unique_ptr<IGameClient>> m_gameClients;
    };
} // namespace Anubis::Engine
