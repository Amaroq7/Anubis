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

#include "EntitiesHooks.hpp"
#include "VFuncCallbacks.hpp"
#include "BasePlayer.hpp"

namespace Anubis::Game::Valve
{
    const std::unique_ptr<BasePlayerHooks> &getBasePlayerHooks(std::unordered_map<std::string, std::uint32_t> &&vOffsets)
    {
        static auto basePlayerHooks = std::make_unique<Game::Valve::BasePlayerHooks>(std::move(vOffsets));
        return basePlayerHooks;
    }

    BasePlayerHooks::BasePlayerHooks(std::unordered_map<std::string, std::uint32_t> &&vOffsets)
        : m_spawn(std::make_unique<BasePlayerSpawnHookRegistry>(IBasePlayer::VTable,
                                                                vOffsets.at("spawn"),
                                                                reinterpret_cast<intptr_t>(VFunc::vSpawnHook))),
          m_takeDamage(
              std::make_unique<BasePlayerTakeDamageHookRegistry>(IBasePlayer::VTable,
                                                                 vOffsets.at("takedamage"),
                                                                 reinterpret_cast<intptr_t>(VFunc::vTakeDamageHook))),
          m_traceAttack(
              std::make_unique<BasePlayerTraceAttackHookRegistry>(IBasePlayer::VTable,
                                                                  vOffsets.at("traceattack"),
                                                                  reinterpret_cast<intptr_t>(VFunc::vTraceAttack))),
          m_killed(std::make_unique<BasePlayerKilledHookRegistry>(IBasePlayer::VTable,
                                                                  vOffsets.at("killed"),
                                                                  reinterpret_cast<intptr_t>(VFunc::vKilled)))
    {
    }

    nstd::observer_ptr<IBasePlayerSpawnHookRegistry> BasePlayerHooks::spawn()
    {
        return m_spawn;
    }

    nstd::observer_ptr<IBasePlayerTakeDamageHookRegistry> BasePlayerHooks::takeDamage()
    {
        return m_takeDamage;
    }

    nstd::observer_ptr<IBasePlayerTraceAttackHookRegistry> BasePlayerHooks::traceAttack()
    {
        return m_traceAttack;
    }

    nstd::observer_ptr<IBasePlayerKilledHookRegistry> BasePlayerHooks::killed()
    {
        return m_killed;
    }

    nstd::observer_ptr<IBasePlayerGiveShieldHookRegistry> BasePlayerHooks::giveShield()
    {
        return {};
    }

    nstd::observer_ptr<IBasePlayerDropShieldHookRegistry> BasePlayerHooks::dropShield()
    {
        return {};
    }
} // namespace Anubis::Game::Valve
