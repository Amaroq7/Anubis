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

#include "BaseEntity.hpp"
#include <game/IBaseDelay.hpp>

namespace Anubis::Game::Valve
{
    class BaseDelay : public BaseEntity, public virtual IBaseDelay
    {
    public:
        explicit BaseDelay(nstd::observer_ptr<Engine::IEdict> edict) : BaseEntity(edict) {}
        ~BaseDelay() override = default;
    };
} // namespace Anubis::Game::Valve