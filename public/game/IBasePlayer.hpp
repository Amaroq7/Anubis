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

#pragma once

#include "../IHookChains.hpp"
#include "IBaseMonster.hpp"

namespace Metamod::Game
{
    class IBasePlayer : public virtual IBaseMonster
    {
        public:
            ~IBasePlayer() override = default;
            virtual void makeVIP() = 0;
            //virtual void killed() = 0;

#if defined META_CORE
        public:
            static inline std::intptr_t VTable;
#endif
    };
}