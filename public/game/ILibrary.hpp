/*
 *  Copyright (C) 2020 Metamod++ Development Team
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

#include <string_view>
#include <StdFSWrapper.hpp>

namespace Metamod::Engine
{
    class IEdict;
    class IEntVars;
    class ITraceResult;
}

namespace Metamod::Game
{
    class IBaseEntity;
    class IBasePlayer;
    class IHooks;
    class IBasePlayerHooks;

    enum class Mod : std::uint8_t
    {
        Valve = 0,
        CStrike,
        CZero,
    };

    enum class GibType : std::uint8_t
    {
        Normal = 0,
        Never,
        Always
    };

    class ILibrary
    {
    public:
        virtual ~ILibrary() = default;

        virtual std::string_view getName() const = 0;
        virtual std::string_view getDesc() const = 0;
        virtual const fs::path &getGameDir() const = 0;
        virtual const fs::path &getPathname() const = 0;
        virtual Mod getMod() const = 0;
        virtual IHooks *getHooks() const = 0;

        virtual bool callGameEntity(std::string_view name, Engine::IEntVars *pev) = 0;

        virtual void pfnGameInit(FuncCallType callType = FuncCallType::Direct) = 0;
        virtual std::int32_t pfnSpawn(Engine::IEdict *edict, FuncCallType callType) = 0;
        virtual bool pfnClientConnect(Engine::IEdict *pEntity, std::string_view pszName,
                                      std::string_view pszAddress, std::string &szRejectReason,
                                      FuncCallType callType = FuncCallType::Direct) = 0;
        virtual void pfnClientPutInServer(Engine::IEdict *pEntity, FuncCallType callType = FuncCallType::Direct) = 0;
        virtual void pfnClientCommand(Engine::IEdict *pEntity, FuncCallType callType = FuncCallType::Direct) = 0;
        virtual void pfnClientUserInfoChanged(Engine::IEdict *pEntity,
                                              char *infobuffer,
                                              FuncCallType callType = FuncCallType::Direct) = 0;

        virtual void pfnServerActivate(std::uint32_t edictCount,
                                       std::uint32_t clientMax,
                                       FuncCallType callType = FuncCallType::Direct) = 0;
        virtual void pfnServerDeactivate(FuncCallType callType = FuncCallType::Direct) = 0;
        virtual void pfnStartFrame(FuncCallType callType = FuncCallType::Direct) = 0;
        virtual void pfnGameShutdown(FuncCallType callType = FuncCallType::Direct) = 0;

        /**
         * @brief Returns entity.
         *
         * @return Edict's base entity representation.
         */
        virtual IBaseEntity *getBaseEntity(Engine::IEdict *edict) = 0;

        /**
         * @brief Returns player entity.
         *
         * @return Edict's player entity representation.
         */
        virtual IBasePlayer *getBasePlayer(Engine::IEdict *edict) = 0;

        virtual IBasePlayerHooks *getCBasePlayerHooks() = 0;
    };
}
