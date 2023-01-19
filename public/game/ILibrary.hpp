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
#include "../Common.hpp"

#include <string_view>
#include <filesystem>

struct DLL_FUNCTIONS;
struct NEW_DLL_FUNCTIONS;
struct edict_s;
typedef edict_s edict_t;

namespace Anubis::Engine
{
    class IEdict;
    class ITraceResult;
    struct InfoBuffer;
} // namespace Anubis::Engine

namespace Anubis::Game
{
    class IBaseEntity;
    class IBasePlayer;
    class IHooks;
    class IBasePlayerHooks;
    class IRules;

    enum class Mod : std::uint8_t
    {
        Valve = 0, /**< Half-Life */
        CStrike,   /**< Counter-Strike */
        CZero,     /**< Counter-Strike: Condition Zero */
        DoD,       /**< Day Of Defeat */
        TFC        /**< Team Fortress Classic */
    };

    class ILibrary
    {
    public:
        /**
         * @brief Game API major version
         *
         */
        static constexpr MajorInterfaceVersion MAJOR_VERSION = MajorInterfaceVersion(2);

        /**
         * @brief Game API minor version
         */
        static constexpr MinorInterfaceVersion MINOR_VERSION = MinorInterfaceVersion(0);

        /**
         * @brief Game API version
         *
         * Major version is present in the 16 most significant bits.
         * Minor version is present in the 16 least significant bits.
         *
         * @b Examples
         *
         * To extract the major version of Game API the following formula can be used
         * @code{cpp}
         * std::uint16_t majorVer = (VERSION >> 16) & 0xFFFF;
         * @endcode
         *
         *
         * To extract the minor version of Game API the following formula can be used
         * @code{cpp}
         * std::uint16_t minorVer = VERSION & 0xFFFF;
         * @endcode
         */
        static constexpr InterfaceVersion VERSION = InterfaceVersion(MAJOR_VERSION << 16 | MINOR_VERSION);

    public:
        virtual ~ILibrary() = default;

        [[nodiscard]] virtual std::string_view getName() const = 0;
        [[nodiscard]] virtual std::string_view getDesc() const = 0;
        [[nodiscard]] virtual const std::filesystem::path &getGameDir() const = 0;
        [[nodiscard]] virtual const std::filesystem::path &getPathname() const = 0;
        [[nodiscard]] virtual Mod getMod() const = 0;
        [[nodiscard]] virtual nstd::observer_ptr<IHooks> getHooks() const = 0;

        virtual bool callGameEntity(std::string_view name, nstd::observer_ptr<Engine::IEdict> pEntity) = 0;

        virtual void pfnGameInit(FuncCallType callType) = 0;
        virtual std::int32_t pfnSpawn(nstd::observer_ptr<Engine::IEdict> pEntity, FuncCallType callType) = 0;
        virtual bool pfnClientConnect(nstd::observer_ptr<Engine::IEdict> pEntity,
                                      std::string_view pszName,
                                      std::string_view pszAddress,
                                      nstd::observer_ptr<std::string> szRejectReason,
                                      FuncCallType callType) = 0;
        virtual void pfnClientPutInServer(nstd::observer_ptr<Engine::IEdict> pEntity, FuncCallType callType) = 0;
        virtual void pfnClientCommand(nstd::observer_ptr<Engine::IEdict> pEntity, FuncCallType callType) = 0;
        virtual void pfnClientUserInfoChanged(nstd::observer_ptr<Engine::IEdict> pEntity,
                                              Engine::InfoBuffer infobuffer,
                                              FuncCallType callType) = 0;

        virtual void pfnServerActivate(std::uint32_t edictCount, std::uint32_t clientMax, FuncCallType callType) = 0;
        virtual void pfnServerDeactivate(FuncCallType callType) = 0;
        virtual void pfnStartFrame(FuncCallType callType) = 0;
        virtual void pfnGameShutdown(FuncCallType callType) = 0;
        virtual void
            pfnCvarValue(nstd::observer_ptr<Engine::IEdict> player, std::string_view value, FuncCallType callType) = 0;
        virtual void pfnCvarValue2(nstd::observer_ptr<Engine::IEdict> player,
                                   std::uint32_t requestID,
                                   std::string_view cvarName,
                                   std::string_view value,
                                   FuncCallType callType) = 0;

        virtual void pfnClientDisconnect(nstd::observer_ptr<Engine::IEdict> pEntity, FuncCallType callType) = 0;

        /**
         * @brief Returns entity.
         *
         * @return Edict's base entity representation.
         */
        virtual std::unique_ptr<IBaseEntity> getBaseEntity(nstd::observer_ptr<Engine::IEdict> edict) = 0;

        /**
         * @brief Returns player entity.
         *
         * @return Edict's player entity representation.
         */
        virtual std::unique_ptr<IBasePlayer> getBasePlayer(nstd::observer_ptr<Engine::IEdict> edict) = 0;

        virtual nstd::observer_ptr<IBasePlayerHooks> getCBasePlayerHooks() = 0;
        virtual nstd::observer_ptr<IRules> getRules() const = 0;

        [[nodiscard]] virtual const std::unique_ptr<DLL_FUNCTIONS> &getDllFuncs() = 0;
        [[nodiscard]] virtual const std::unique_ptr<NEW_DLL_FUNCTIONS> &getNewDllFuncs() = 0;
        [[nodiscard]] virtual void *getSystemHandle() const = 0;
        virtual void initVFuncHooks() = 0;
        virtual std::unique_ptr<IBaseEntity> getBaseEntity(edict_t *entity) const = 0;
    };
} // namespace Anubis::Game
