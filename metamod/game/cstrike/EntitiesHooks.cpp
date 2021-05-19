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

#include "EntitiesHooks.hpp"
#include "VFuncCallbacks.hpp"
#include "MetaExports.hpp"

std::unique_ptr<Metamod::Game::CStrike::BasePlayerHooks> gBasePlayerHooks;

namespace Metamod::Game::CStrike
{
    BasePlayerHooks::BasePlayerHooks() :
        m_spawn([]() {
            gReGameAPI->GetHookchains()->CBasePlayer_Spawn()->registerHook(Game::VFunc::vCBasePlayerSpawn);
        }, []() {
            gReGameAPI->GetHookchains()->CBasePlayer_Spawn()->unregisterHook(Game::VFunc::vCBasePlayerSpawn);
        }),
        m_takeDamage([]() {
            gReGameAPI->GetHookchains()->CBasePlayer_TakeDamage()->registerHook(Game::VFunc::vCBasePlayerTakeDamage);
        }, []() {
            gReGameAPI->GetHookchains()->CBasePlayer_TakeDamage()->unregisterHook(Game::VFunc::vCBasePlayerTakeDamage);
        }),
        m_traceAttack([]() {
            gReGameAPI->GetHookchains()->CBasePlayer_TraceAttack()->registerHook(Game::VFunc::vCBasePlayerTraceAttack);
        }, []() {
            gReGameAPI->GetHookchains()->CBasePlayer_TraceAttack()->unregisterHook(Game::VFunc::vCBasePlayerTraceAttack);
        }),
        m_killed([]() {
            gReGameAPI->GetHookchains()->CBasePlayer_Killed()->registerHook(Game::VFunc::vCBasePlayerKilled);
        }, []() {
            gReGameAPI->GetHookchains()->CBasePlayer_Killed()->unregisterHook(Game::VFunc::vCBasePlayerKilled);
        })
    {}

    BasePlayerSpawnHookRegistry *BasePlayerHooks::spawn()
    {
        return &m_spawn;
    }

    BasePlayerTakeDamageHookRegistry *BasePlayerHooks::takeDamage()
    {
        return &m_takeDamage;
    }

    BasePlayerTraceAttackHookRegistry *BasePlayerHooks::traceAttack()
    {
        return &m_traceAttack;
    }

    BasePlayerKilledHookRegistry *BasePlayerHooks::killed()
    {
        return &m_killed;
    }
}
