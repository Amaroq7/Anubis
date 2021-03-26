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
#include <extdll.h>
#include <osconfig.h>
#include <util.h>
#include <cbase.h>
#include <enginecallback.h>

namespace Metamod::GameLib::Entities::Valve
{
    BaseEntity::BaseEntity(const Engine::Edict *edict)
      : m_edict(const_cast<Engine::Edict *>(edict))
    {}

    Metamod::Engine::Edict *BaseEntity::edict() const
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
            ALERT(at_aiconsole, "SUB_Remove called on entity with health > 0\n");
        }

        REMOVE_ENTITY(ENT(baseEntity->pev));
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

    std::int32_t BaseEntity::takeDamage(Engine::IEntVars *pevInflictor,
                                        Engine::IEntVars *pevAttacker,
                                        float flDamage,
                                        std::int32_t bitsDamageType)
    {
        /*VTableHook *hook = gSPGlobal->getVTableManager()->getHook(IVTableHookManager::vFuncType::TakeDamage);

        // VFunc is hooked so call the original
        if (hook && !m_callHooks)
        {
            return hook->execOriginalFunc<std::int32_t, entvars_t *, entvars_t *, float, std::int32_t>(
                operator CBaseEntity *(), *static_cast<Engine::EntVars *>(pevInflictor),
                *static_cast<Engine::EntVars *>(pevAttacker), flDamage, bitsDamageType);
        }*/

        return operator CBaseEntity *()->TakeDamage(*static_cast<Engine::EntVars *>(pevInflictor),
                                                    *static_cast<Engine::EntVars *>(pevAttacker), flDamage,
                                                    bitsDamageType);
    }

    BaseEntity::operator CBaseEntity *() const
    {
        return reinterpret_cast<CBaseEntity *>(GET_PRIVATE(*m_edict));
    }

    void BaseEntity::setCallHooks(bool call)
    {
        m_callHooks = call;
    }

    std::intptr_t BaseEntity::_getCurrentVTable()
    {
        return *reinterpret_cast<intptr_t *>(GET_PRIVATE(*m_edict));
    }
} // namespace Valve
