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

#include "BaseEntity.hpp"
#include "MetaExports.hpp"

#include <extdll.h>
#include <osconfig.h>
#include <util.h>
#include <cbase.h>

#include <engine/Common.hpp>
#include <engine/IEntVars.hpp>

namespace Metamod::Game::Valve
{
    BaseEntity::BaseEntity(Engine::IEdict *edict)
        : m_edict(edict), m_entity(reinterpret_cast<CBaseEntity *>(edict->getPrivateData()))
    {}

    Metamod::Engine::IEdict *BaseEntity::edict() const
    {
        return m_edict;
    }

    void BaseEntity::remove()
    {
        if (m_entity->pev->health > 0)
        {
            // this situation can screw up monsters who can't tell their entity pointers are invalid.
            m_entity->pev->health = 0;
            gEngineLib->alert(Engine::AlertType::Aiconsole, "remove called on entity with health > 0\n", FuncCallType::Direct);
        }

        gEngineLib->removeEntity(m_edict, FuncCallType::Direct);
    }

    bool BaseEntity::isAlive() const
    {
        return m_entity->IsAlive() == TRUE;
    }

    int BaseEntity::takeHealth(float flHealth, int bitsDamageType) const
    {
        return m_entity->TakeHealth(flHealth, bitsDamageType);
    }

    std::string_view BaseEntity::getTeam() const
    {
        return m_entity->TeamID();
    }

    bool BaseEntity::takeDamage(Engine::IEntVars *pevInflictor,
                                Engine::IEntVars *pevAttacker,
                                float flDamage,
                                std::int32_t bitsDamageType)
    {
        return m_entity->TakeDamage(static_cast<entvars_t *>(*pevInflictor),
                                    static_cast<entvars_t *>(*pevAttacker),
                                    flDamage, bitsDamageType) == TRUE;
    }

    BaseEntity::operator CBaseEntity *() const
    {
        return m_entity;
    }
} // namespace Valve
