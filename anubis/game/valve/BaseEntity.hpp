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

#include <game/IBaseEntity.hpp>
#include <engine/IEdict.hpp>

#include <string_view>
#include <memory>

class CBaseEntity;

namespace Anubis::Game::Valve
{
    class BaseEntity : public virtual IBaseEntity
    {
    public:
        explicit BaseEntity(nstd::observer_ptr<Engine::IEdict> edict);
        ~BaseEntity() override = default;

        [[nodiscard]] nstd::observer_ptr<Engine::IEdict> edict() const final;
        void remove() final;
        [[nodiscard]] bool isAlive() const final;
        [[nodiscard]] std::string_view getTeam() const final;
        [[nodiscard]] bool takeHealth(float flHealth, DmgType bitsDamageType) const final;
        bool takeDamage(nstd::observer_ptr<IBaseEntity> pevInflictor,
                        nstd::observer_ptr<IBaseEntity> pevAttacker,
                        float flDamage,
                        DmgType bitsDamageType) final;

        [[nodiscard]] bool isValid() const final;
        [[nodiscard]] bool isPlayer() const final;

    public:
        explicit operator CBaseEntity *() const final;
        explicit operator entvars_t *() const final;

    public:
        void updateSerialNumber(std::uint32_t serialNumber);

    protected:
        nstd::observer_ptr<Engine::IEdict> m_edict;
        CBaseEntity *m_entity;
        std::uint32_t m_serialNumber;
    };
} // namespace Anubis::Game::Valve