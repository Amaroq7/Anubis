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

#include <cinttypes>

typedef struct entvars_s entvars_t;
struct TraceResult;
class Vector;

extern std::intptr_t CWorldVTable;
extern std::intptr_t CWorldSpawnOrigFn;

#if defined _WIN32
    #define FASTCALL __fastcall
#else
    #define FASTCALL
#endif

namespace Anubis::Game::VFunc
{
    void FASTCALL vSpawnHook(void *instance
#if defined _WIN32
                             ,
                             void *notUsed
#endif
    );

    std::int32_t FASTCALL vTakeDamageHook(void *instance,
#if defined _WIN32
                                          void *notUsed,
#endif
                                          entvars_t *pevInflictor,
                                          entvars_t *pevAttacker,
                                          float flDamage,
                                          std::int32_t bitsDamageType);

    void FASTCALL vTraceAttack(void *instance,
#if defined _WIN32
                               void *notUsed,
#endif
                               entvars_t *pevAttacker,
                               float flDamage,
                               Vector vecDir,
                               ::TraceResult *ptr,
                               std::int32_t bitsDamageType);

    void FASTCALL vKilled(void *instance,
#if defined _WIN32
                          void *notUsed,
#endif
                          entvars_t *pevAttacker,
                          int iGib);

    extern std::uint32_t gPevOffset;
} // namespace Anubis::Game::VFunc