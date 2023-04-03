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

#include "BaseEntity.hpp"
#include "AnubisExports.hpp"
#include "ReGameHeaders.hpp"

namespace Anubis::Game::CStrike
{
    BaseEntity::BaseEntity(nstd::observer_ptr<Engine::IEdict> edict)
        : m_edict(edict),
          m_entity(reinterpret_cast<CBaseEntity *>(edict->getPrivateData()))
    {
    }

    BaseEntity::BaseEntity(CBaseEntity *entity) : m_edict(gEngineLib->getEdict(entity->pev)), m_entity(entity) {}

    nstd::observer_ptr<Engine::IEdict> BaseEntity::edict() const
    {
        return m_edict;
    }

    void BaseEntity::remove()
    {
        if (m_entity->pev->health > 0)
        {
            // this situation can screw up monsters who can't tell their entity pointers are invalid.
            m_entity->pev->health = 0;
            gEngineLib->alert(Engine::AlertType::Aiconsole, "remove called on entity with health > 0\n",
                              FuncCallType::Direct);
        }

        gEngineLib->removeEntity(m_edict, FuncCallType::Direct);
    }

    bool BaseEntity::isAlive() const
    {
        return m_entity->IsAlive() == TRUE;
    }

    bool BaseEntity::takeHealth(float flHealth, DmgType bitsDamageType) const
    {
        return m_entity->TakeHealth(flHealth, static_cast<int>(bitsDamageType));
    }

    std::string_view BaseEntity::getTeam() const
    {
        return m_entity->TeamID();
    }

    bool BaseEntity::takeDamage(nstd::observer_ptr<IBaseEntity> pevInflictor,
                                nstd::observer_ptr<IBaseEntity> pevAttacker,
                                float flDamage,
                                DmgType bitsDamageType)
    {
        return m_entity->TakeDamage(static_cast<entvars_t *>(*pevInflictor), static_cast<entvars_t *>(*pevAttacker),
                                    flDamage, static_cast<int>(bitsDamageType)) == TRUE;
    }

    BaseEntity::operator CBaseEntity *() const
    {
        return m_entity;
    }

    BaseEntity::operator entvars_t *() const
    {
        return m_entity->pev;
    }

    bool BaseEntity::isValid() const
    {
        return !m_edict->isFree() && m_edict->getPrivateData();
    }

    bool BaseEntity::isPlayer() const
    {
        return m_entity->IsPlayer() == TRUE;
    }

    void BaseEntity::spawn() const
    {
        operator CBaseEntity *()->Spawn();
    }

    std::unique_ptr<IBaseEntity> BaseEntity::respawn() const
    {
        return std::make_unique<BaseEntity>(operator CBaseEntity *()->Respawn());
    }
} // namespace Anubis::Game::CStrike
