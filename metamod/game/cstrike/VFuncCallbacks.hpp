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

#include <cinttypes>

#include "ReGameHeaders.hpp"

namespace Metamod::Game::VFunc
{
    void vCBasePlayerSpawn(IReGameHook_CBasePlayer_Spawn *hook, CBasePlayer *player);

     BOOL vCBasePlayerTakeDamage(IReGameHook_CBasePlayer_TakeDamage *hook,
        CBasePlayer *instance,
        entvars_t *pevInflictor,
        entvars_t *pevAttacker,
        float &flDamage,
        std::int32_t bitsDamageType
    );

    void vCBasePlayerTraceAttack(IReGameHook_CBasePlayer_TraceAttack *hook,
        CBasePlayer *player,
        entvars_t *pevAttacker,
        float flDamage,
        Vector &vecDir,
        ::TraceResult *ptr,
        std::int32_t bitsDamageType
    );

    void vCBasePlayerKilled(IReGameHook_CBasePlayer_Killed *hook,
                            CBasePlayer *player, entvars_t *pevAttacker, int iGib);

    void vCBasePlayerGiveShield(IReGameHook_CBasePlayer_GiveShield *hook, CBasePlayer *player, bool deploy);
    CBaseEntity *vCBasePlayerDropShield(IReGameHook_CBasePlayer_DropShield *hook, CBasePlayer *player, bool deploy);
} // namespace