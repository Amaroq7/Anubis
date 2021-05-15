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

#include "Library.hpp"

namespace Metamod::Game::VFunc
{
    void vSpawnHook(
#if defined __linux__
        void *instance
#endif
    );

    std::int32_t vTakeDamageHook(
#if defined __linux__
        void *instance,
#endif
        entvars_t *pevInflictor,
        entvars_t *pevAttacker,
        float flDamage,
        std::int32_t bitsDamageType
    );

    void vTraceAttack(
#if defined __linux__
        void *instance,
#endif
        entvars_t *pevAttacker,
        float flDamage,
        Vector vecDir,
        ::TraceResult *ptr,
        std::int32_t bitsDamageType
    );
} // namespace