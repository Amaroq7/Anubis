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

namespace Anubis::Engine
{
    class IEdict;
    class ITraceResult;
} // namespace Anubis::Engine

#include "../observer_ptr.hpp"
#include "Consts.hpp"

#include <string_view>
#include <memory>

/**
 * @internal A internal representation of a base entity in gamedll
 */
class CBaseEntity;

/**
 * @internal Entity variables
 */
struct entvars_s;

namespace Anubis::Game
{
    class IBaseEntity
    {
    public:
        virtual ~IBaseEntity() = default;
        [[nodiscard]] virtual nstd::observer_ptr<Engine::IEdict> edict() const = 0;
        virtual void remove() = 0;
        [[nodiscard]] virtual bool isAlive() const = 0;
        [[nodiscard]] virtual std::string_view getTeam() const = 0;
        [[nodiscard]] virtual bool takeHealth(float flHealth, DmgType bitsDamageType) const = 0;
        [[nodiscard]] virtual bool takeDamage(nstd::observer_ptr<IBaseEntity> pevInflictor,
                                              nstd::observer_ptr<IBaseEntity> pevAttacker,
                                              float flDamage,
                                              DmgType bitsDamageType) = 0;
        [[nodiscard]] virtual bool isValid() const = 0;
        [[nodiscard]] virtual bool isPlayer() const = 0;
        virtual void spawn() const = 0;
        virtual std::unique_ptr<IBaseEntity> respawn() const = 0;

        /**
         * @cond INTERNAL
         */
        /**
         * @internal Allows to get a internal representation of a base entity in the gamedll.
         */
        explicit virtual operator CBaseEntity *() const = 0;

        /**
         * @internal Allows to get a internal representation of an entity's variables.
         */
        explicit virtual operator entvars_s *() const = 0;
        /**
         * @endcond
         */
    };
} // namespace Anubis::Game
