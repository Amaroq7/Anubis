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
#include "IServerState.hpp"

#include <string_view>
#include <cinttypes>
#include <cstddef>
#include <optional>

typedef struct edict_s edict_t;
typedef struct entvars_s entvars_t;
typedef struct enginefuncs_s enginefuncs_t;
typedef struct TraceResult TraceResult;
typedef struct globalvars_s globalvars_t;
typedef struct cvar_s cvar_t;
class IGameClient;
struct TraceResult;

namespace Anubis::Game
{
    class IBaseEntity;
}

namespace Anubis::Engine
{
    class IEdict;
    class ITraceResult;
    class IHooks;
    class ICvar;
    class IGameClient;

    class ILibrary
    {
    public:
        /**
         * @brief Engine API major version
         *
         */
        static constexpr MajorInterfaceVersion MAJOR_VERSION = MajorInterfaceVersion(2);

        /**
         * @brief Engine API minor version
         */
        static constexpr MinorInterfaceVersion MINOR_VERSION = MinorInterfaceVersion(0);

        /**
         * @brief Engine API version
         *
         * Major version is present in the 16 most significant bits.
         * Minor version is present in the 16 least significant bits.
         *
         * @b Examples
         *
         * To extract the major version of Engine API the following formula can be used
         * @code{cpp}
         * std::uint16_t majorVer = (VERSION >> 16) & 0xFFFF;
         * @endcode
         *
         *
         * To extract the minor version of Engine API the following formula can be used
         * @code{cpp}
         * std::uint16_t minorVer = VERSION & 0xFFFF;
         * @endcode
         */
        static constexpr InterfaceVersion VERSION = InterfaceVersion(MAJOR_VERSION << 16 | MINOR_VERSION);

    public:
        virtual ~ILibrary() = default;

        [[nodiscard]] virtual nstd::observer_ptr<IEdict> getEdict(std::uint32_t index, FuncCallType callType) const = 0;
        virtual std::unique_ptr<ITraceResult> createTraceResult() = 0;
        virtual nstd::observer_ptr<IHooks> getHooks() = 0;
        [[nodiscard]] virtual std::array<std::uint32_t, 2> getReHLDSVersion() const = 0;

        /* Engine funcs */
        [[nodiscard]] virtual PrecacheId precacheModel(std::string_view model, FuncCallType callType) const = 0;
        [[nodiscard]] virtual PrecacheId precacheSound(std::string_view sound, FuncCallType callType) const = 0;
        [[nodiscard]] virtual PrecacheId precacheGeneric(std::string_view generic, FuncCallType callType) const = 0;
        virtual void changeLevel(std::string_view level1, std::string_view level2, FuncCallType callType) const = 0;
        virtual void serverCommand(std::string_view cmd, FuncCallType callType) const = 0;
        virtual void serverExecute(FuncCallType callType) const = 0;
        virtual void registerSrvCommand(std::string_view cmd, ServerCmdCallback cb, FuncCallType callType) = 0;
        virtual void messageBegin(MsgDest msgDest,
                                  MsgType msgType,
                                  std::optional<std::array<float, 3>> pOrigin,
                                  nstd::observer_ptr<IEdict> pEdict,
                                  FuncCallType callType) const = 0;
        virtual void messageEnd(FuncCallType callType) const = 0;
        virtual void writeByte(std::byte byteArg, FuncCallType callType) const = 0;
        virtual void writeChar(char charArg, FuncCallType callType) const = 0;
        virtual void writeShort(std::int16_t shortArg, FuncCallType callType) const = 0;
        virtual void writeLong(std::int32_t longArg, FuncCallType callType) const = 0;
        virtual void writeEntity(MsgEntity entArg, FuncCallType callType) const = 0;
        virtual void writeAngle(MsgAngle angleArg, FuncCallType callType) const = 0;
        virtual void writeCoord(MsgCoord coordArg, FuncCallType callType) const = 0;
        virtual void writeString(std::string_view strArg, FuncCallType callType) const = 0;
        [[nodiscard]] virtual MsgType
            regUserMsg(std::string_view name, Engine::MsgSize size, FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::string_view getPlayerAuthID(nstd::observer_ptr<IEdict> pEdict,
                                                               FuncCallType callType) const = 0;
        [[nodiscard]] virtual UserID getPlayerUserID(nstd::observer_ptr<IEdict> pEdict,
                                                     FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::string_view
            infoKeyValue(InfoBuffer infobuffer, std::string_view key, FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::string_view cmdArgs(FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::string_view cmdArgv(std::uint8_t argc, FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::uint8_t cmdArgc(FuncCallType callType) const = 0;
        virtual void registerCvar(std::string_view name, std::string_view value, FuncCallType callType) = 0;
        virtual nstd::observer_ptr<ICvar> getCvar(std::string_view name, FuncCallType callType) = 0;
        virtual void setModel(nstd::observer_ptr<IEdict> pEdict, std::string_view model, FuncCallType callType) = 0;
        virtual nstd::observer_ptr<IEdict> createEntity(FuncCallType callType) = 0;
        virtual void removeEntity(nstd::observer_ptr<IEdict> pEdict, FuncCallType callType) = 0;
        virtual void alert(AlertType alertType, std::string_view msg, FuncCallType callType) = 0;
        virtual void print(std::string_view szMsg, FuncCallType callType) = 0;
        [[nodiscard]] virtual std::string_view getString(StringOffset offset, FuncCallType callType) const = 0;
        virtual void
            cvarDirectSet(nstd::observer_ptr<ICvar> cvar, std::string_view value, FuncCallType callType) const = 0;
        [[nodiscard]] virtual float getTime() const = 0;
        [[nodiscard]] virtual std::string_view getMapName() const = 0;
        virtual bool addExtDll(void *hModule) const = 0;
        virtual void removeExtDll(void *hModule) const = 0;
        virtual void removeCmd(std::string_view cmd_name) = 0;
        [[nodiscard]] virtual std::uint32_t getWorldmapCrc() const = 0;
        [[nodiscard]] virtual ServerState getState() const = 0;

        [[nodiscard]] virtual bool isDedicatedServer(FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::pair<std::size_t, std::string_view> checkEngParm(std::string_view parm,
                                                                                    FuncCallType callType) const = 0;
        virtual void queryClientCvarValue(nstd::observer_ptr<IEdict> player,
                                          std::string_view cvarName,
                                          FuncCallType callType) const = 0;
        virtual void queryClientCvarValue2(nstd::observer_ptr<IEdict> player,
                                           std::string_view cvarName,
                                           std::uint32_t requestID,
                                           FuncCallType callType) const = 0;
        [[nodiscard]] virtual nstd::observer_ptr<IGameClient> getGameClient(std::uint32_t index) const = 0;
        [[nodiscard]] virtual std::uint32_t getMaxClients() const = 0;
        [[nodiscard]] virtual std::uint32_t getMaxClientsLimit() const = 0;
        [[nodiscard]] virtual std::uint32_t getIndexOfEdict(nstd::observer_ptr<IEdict> edict,
                                                            FuncCallType callType) const = 0;

        [[nodiscard]] virtual std::string getGameDir(FuncCallType callType) const = 0;
        [[nodiscard]] virtual float getCvarFloat(std::string_view cvarName, FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::string_view getCvarString(std::string_view cvarName,
                                                             FuncCallType callType) const = 0;
        virtual void setCvarFloat(std::string_view cvarName, float value, FuncCallType callType) const = 0;
        virtual void setCvarString(std::string_view cvarName, std::string_view value, FuncCallType callType) const = 0;
        [[nodiscard]] virtual EntityOffset getEntityOffset(nstd::observer_ptr<IEdict> edict,
                                                           FuncCallType callType) const = 0;
        [[nodiscard]] virtual nstd::observer_ptr<IEdict> getEntityOfEntOffset(EntityOffset entOffset,
                                                                              FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::unique_ptr<Game::IBaseEntity> allocEntPrivateData(nstd::observer_ptr<IEdict> edict,
                                                                                     std::int32_t classSize,
                                                                                     FuncCallType callType) const = 0;

        [[nodiscard]] StringOffset makeString(std::string_view str) const
        {
            return StringOffset(str.data() - getString(StringOffset(0), FuncCallType::Direct).data());
        }

        [[nodiscard]] virtual StringOffset allocString(std::string_view str, FuncCallType callType) const = 0;

        virtual ModelIndex modelIndex(std::string_view model, FuncCallType callType) const = 0;
        virtual std::int32_t randomLong(std::int32_t lLow, std::int32_t lHigh, FuncCallType callType) const = 0;
        virtual float randomFloat(float flLow, float flHigh, FuncCallType callType) const = 0;

        virtual void clientPrint(nstd::observer_ptr<IEdict> pEdict,
                                 PrintType ptype,
                                 std::string_view szMsg,
                                 FuncCallType callType) const = 0;

        virtual bool entIsOnFloor (nstd::observer_ptr<IEdict> e, FuncCallType callType) const = 0;
	    virtual std::int8_t dropToFloor (nstd::observer_ptr<IEdict> e, FuncCallType callType) const = 0;
        virtual void emitSound(nstd::observer_ptr<IEdict> entity, Channel channel, std::string_view sample, float volume, float attenuation, SoundFlags fFlags, Pitch pitch, FuncCallType callType) const = 0;
        virtual void emitAmbientSound (nstd::observer_ptr<IEdict> entity, float *pos, std::string_view samp, float vol, float attenuation, SoundFlags fFlags, Pitch pitch, FuncCallType callType) const = 0;
        virtual void traceLine (const float *v1, const float *v2, int fNoMonsters, nstd::observer_ptr<IEdict> pentToSkip, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const = 0;
        virtual void traceToss (nstd::observer_ptr<IEdict> pent, nstd::observer_ptr<IEdict> pentToIgnore, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const = 0;
        virtual bool traceMonsterHull (nstd::observer_ptr<IEdict> pEdict, const float *v1, const float *v2, int fNoMonsters, nstd::observer_ptr<IEdict> pentToSkip, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const = 0;
        virtual void traceHull (const float *v1, const float *v2, int fNoMonsters, int hullNumber, nstd::observer_ptr<IEdict> pentToSkip, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const = 0;
        virtual void traceModel (const float *v1, const float *v2, int hullNumber, nstd::observer_ptr<IEdict> pent, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const = 0;
        virtual std::string_view traceTexture (nstd::observer_ptr<IEdict> pTextureEntity, const float *v1, const float *v2, FuncCallType callType) const = 0;
        virtual void traceSphere (const float *v1, const float *v2, int fNoMonsters, float radius, nstd::observer_ptr<IEdict> pentToSkip, nstd::observer_ptr<ITraceResult> ptr, FuncCallType callType) const = 0;

        [[nodiscard]] virtual nstd::observer_ptr<IEdict> getEdict(const edict_t *edict) const = 0;
        [[nodiscard]] virtual nstd::observer_ptr<IEdict> getEdict(const entvars_t *vars) const = 0;
        virtual std::unique_ptr<ITraceResult> createTraceResult(::TraceResult *tr) = 0;
        [[nodiscard]] virtual const std::unique_ptr<enginefuncs_t> &getEngineFuncs() const = 0;
        [[nodiscard]] virtual nstd::observer_ptr<IGameClient> getGameClient(::IGameClient *gameClient) const = 0;
        virtual void initEdict(edict_t *edict) = 0;
        [[nodiscard]] virtual std::uint32_t getIndexOfEdict(const edict_t *edict) const = 0;
        [[nodiscard]] virtual nstd::observer_ptr<globalvars_t> getGlobals() const = 0;
        virtual nstd::observer_ptr<ICvar> addToCache(cvar_t *cvar) = 0;
        virtual void removeHooks() = 0;
    };
} // namespace Anubis::Engine
