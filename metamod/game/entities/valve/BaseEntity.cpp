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

#include "BaseEntity.hpp"
#include "MetaExports.hpp"

#include <extdll.h>
#include <osconfig.h>
#include <util.h>
#include <cbase.h>

#include <engine/Common.hpp>
#include <engine/IEntVars.hpp>

namespace Metamod::Game::Entities::Valve
{
    BaseEntity::BaseEntity(Engine::IEdict *edict) : m_edict(edict) {}

    Metamod::Engine::IEdict *BaseEntity::edict() const
    {
        return m_edict;
    }

    void BaseEntity::remove()
    {
        CBaseEntity *baseEntity = operator CBaseEntity *();
        if (baseEntity->pev->health > 0)
        {
            // this situation can screw up monsters who can't tell their entity pointers are invalid.
            baseEntity->pev->health = 0;
            gEngineLib->alert(Engine::AlertType::Aiconsole, "SUB_Remove called on entity with health > 0\n", FuncCallType::Direct);
        }

        gEngineLib->removeEntity(m_edict, FuncCallType::Direct);
    }

    bool BaseEntity::isAlive() const
    {
        return operator CBaseEntity *()->IsAlive() == 1;
    }

    int BaseEntity::takeHealth(float flHealth, int bitsDamageType) const
    {
        return operator CBaseEntity *()->TakeHealth(flHealth, bitsDamageType);
    }

    std::string_view BaseEntity::getTeam() const
    {
        return operator CBaseEntity *()->TeamID();
    }

    bool BaseEntity::takeDamage(Engine::IEntVars *pevInflictor,
                                Engine::IEntVars *pevAttacker,
                                float flDamage,
                                std::int32_t bitsDamageType, FuncCallType callType [[maybe_unused]])
    {
        return operator CBaseEntity *()->TakeDamage(*pevInflictor, *pevAttacker, flDamage, bitsDamageType) == TRUE;
    }

    BaseEntity::operator CBaseEntity *() const
    {
        return reinterpret_cast<CBaseEntity *>(m_edict->getPrivateData());
    }
} // namespace Valve
