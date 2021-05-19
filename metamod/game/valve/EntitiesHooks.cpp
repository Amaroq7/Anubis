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
#include "BasePlayer.hpp"

std::unique_ptr<Metamod::Game::Valve::BasePlayerHooks> gBasePlayerHooks;

namespace Metamod::Game::Valve
{
    BasePlayerHooks::BasePlayerHooks(const std::unordered_map<std::string, std::uint32_t> &vOffsets) :
        m_spawn(IBasePlayer::VTable, vOffsets.at("spawn"), reinterpret_cast<intptr_t>(VFunc::vSpawnHook)),
        m_takeDamage(IBasePlayer::VTable, vOffsets.at("takedamage"), reinterpret_cast<intptr_t>(VFunc::vTakeDamageHook)),
        m_traceAttack(IBasePlayer::VTable, vOffsets.at("traceattack"), reinterpret_cast<intptr_t>(VFunc::vTraceAttack))
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

    /*BasePlayerKilledHookRegistry *BasePlayerHooks::killed()
    {
        return &m_killed;
    }*/
}
