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

#include <game/IBasePlayerAmmo.hpp>

#include "BaseEntity.hpp"

class CBasePlayerAmmo;

namespace Anubis::Game::Valve
{
    class BasePlayerAmmo : public BaseEntity, public virtual IBasePlayerAmmo
    {
    public:
        explicit BasePlayerAmmo(nstd::observer_ptr<Engine::IEdict> edict);
        ~BasePlayerAmmo() override = default;
        void defaultTouch(nstd::observer_ptr<IBaseEntity> pOther) const final; // default weapon touch
        bool addAmmo(nstd::observer_ptr<IBaseEntity> pOther) const final;

        void materialize() const final;

        explicit operator CBasePlayerAmmo *() const final;
    };
} // namespace Anubis::Game::Valve