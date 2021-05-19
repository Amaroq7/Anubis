/*
 *  Copyright (C) 2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "BaseEntity.hpp"

#include "MetaExports.hpp"

#include <engine/IEntVars.hpp>

#include <extdll.h>
#include <enginecallback.h>
#include <tier0/platform.h>
#include <cbase.h>

namespace Metamod::Game::Entities::CStrike
{
    BaseEntity::BaseEntity(Engine::IEdict *edict)
      : m_edict(const_cast<Engine::IEdict *>(edict))
    {}

    Engine::IEdict *BaseEntity::edict() const
    {
        return m_edict;
    }

    void BaseEntity::remove()
    {
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
                                        std::int32_t bitsDamageType)
    {
        return operator CBaseEntity *()->TakeDamage(*pevInflictor, *pevAttacker, flDamage, bitsDamageType) == TRUE;
    }

    BaseEntity::operator CBaseEntity *() const
    {
        return reinterpret_cast<CBaseEntity *>(m_edict->getPrivateData());
    }
} // namespace Valve
