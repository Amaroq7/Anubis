/*
 *  Copyright (C) 2023 Anubis Development Team
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

#include "BasePlayerAmmo.hpp"

namespace Anubis::Game::CStrike
{
    BasePlayerAmmo::BasePlayerAmmo(nstd::observer_ptr<Engine::IEdict> edict) : BaseEntity(edict) {};
    BasePlayerAmmo::operator CBasePlayerAmmo *() const
    {
        return reinterpret_cast<CBasePlayerAmmo *>(m_entity);
    }

    void BasePlayerAmmo::spawn() const
    {
        operator CBasePlayerAmmo *()->Spawn();
    }
    void BasePlayerAmmo::defaultTouch(nstd::observer_ptr<IBaseEntity> pOther [[maybe_unused]]) const
    {
        // HL Only
    }

    bool BasePlayerAmmo::addAmmo(nstd::observer_ptr<IBaseEntity> pOther) const
    {
        return operator CBasePlayerAmmo *()->AddAmmo(static_cast<CBaseEntity *>(*pOther)) == TRUE;
    }

    void BasePlayerAmmo::materialize() const
    {
        // HL Only
    }

    std::unique_ptr<IBaseEntity> BasePlayerAmmo::respawn() const
    {
        return std::make_unique<BaseEntity>(operator CBasePlayerAmmo *()->Respawn());
    }

} // namespace Anubis::Game::CStrike