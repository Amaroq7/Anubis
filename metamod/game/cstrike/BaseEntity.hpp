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

#include <game/IBaseEntity.hpp>
#include <engine/IEdict.hpp>

#include <string_view>

class CBaseEntity;

namespace Metamod::Game::CStrike
{
    class BaseEntity : public virtual IBaseEntity
    {
        public:
            explicit BaseEntity(Engine::IEdict *edict);
            ~BaseEntity() override = default;

            Engine::IEdict *edict() const override;
            void remove() override;
            bool isAlive() const override;
            std::string_view getTeam() const override;
            int takeHealth(float flHealth, int bitsDamageType) const override;
            bool takeDamage(Engine::IEntVars *pevInflictor, Engine::IEntVars *pevAttacker, float flDamage, std::int32_t bitsDamageType) override;

        protected:
            Engine::IEdict *m_edict;

        private:
            operator CBaseEntity *() const;
    };
}