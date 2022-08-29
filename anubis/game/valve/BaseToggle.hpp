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

#include <game/IBaseToggle.hpp>
#include "BaseAnimating.hpp"

namespace Anubis::Game::Valve
{
    class BaseToggle : public BaseAnimating, public virtual IBaseToggle
    {
    public:
        explicit BaseToggle(nstd::observer_ptr<Engine::IEdict> edict) : BaseAnimating(edict) {}

        ~BaseToggle() override = default;
    };
} // namespace Anubis::Game::Valve