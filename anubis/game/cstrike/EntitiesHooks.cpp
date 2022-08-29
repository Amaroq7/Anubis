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
#include "AnubisExports.hpp"

namespace Anubis::Game::CStrike
{
    const std::unique_ptr<BasePlayerHooks> &getBasePlayerHooks()
    {
        static auto basePlayerHooks = std::make_unique<Game::CStrike::BasePlayerHooks>(gReGameAPI->GetHookchains());
        return basePlayerHooks;
    }

    BasePlayerHooks::BasePlayerHooks(IReGameHookchains *rehooks)
        : m_spawn(std::make_unique<BasePlayerSpawnHookRegistry>(
              [rehooks]()
              {
                  rehooks->CBasePlayer_Spawn()->registerHook(Game::VFunc::vCBasePlayerSpawn);
              },
              [rehooks]()
              {
                  rehooks->CBasePlayer_Spawn()->unregisterHook(Game::VFunc::vCBasePlayerSpawn);
              })),
          m_takeDamage(std::make_unique<BasePlayerTakeDamageHookRegistry>(
              [rehooks]()
              {
                  rehooks->CBasePlayer_TakeDamage()->registerHook(Game::VFunc::vCBasePlayerTakeDamage);
              },
              [rehooks]()
              {
                  rehooks->CBasePlayer_TakeDamage()->unregisterHook(Game::VFunc::vCBasePlayerTakeDamage);
              })),
          m_traceAttack(std::make_unique<BasePlayerTraceAttackHookRegistry>(
              [rehooks]()
              {
                  rehooks->CBasePlayer_TraceAttack()->registerHook(Game::VFunc::vCBasePlayerTraceAttack);
              },
              [rehooks]()
              {
                  rehooks->CBasePlayer_TraceAttack()->unregisterHook(Game::VFunc::vCBasePlayerTraceAttack);
              })),
          m_killed(std::make_unique<BasePlayerKilledHookRegistry>(
              [rehooks]()
              {
                  rehooks->CBasePlayer_Killed()->registerHook(Game::VFunc::vCBasePlayerKilled);
              },
              [rehooks]()
              {
                  rehooks->CBasePlayer_Killed()->unregisterHook(Game::VFunc::vCBasePlayerKilled);
              })),
          m_giveShield(std::make_unique<BasePlayerGiveShieldHookRegistry>(
              [rehooks]()
              {
                  rehooks->CBasePlayer_GiveShield()->registerHook(Game::VFunc::vCBasePlayerGiveShield);
              },
              [rehooks]()
              {
                  rehooks->CBasePlayer_GiveShield()->unregisterHook(Game::VFunc::vCBasePlayerGiveShield);
              })),
          m_dropShield(std::make_unique<BasePlayerDropShieldHookRegistry>(
              [rehooks]()
              {
                  rehooks->CBasePlayer_DropShield()->registerHook(Game::VFunc::vCBasePlayerDropShield);
              },
              [rehooks]()
              {
                  rehooks->CBasePlayer_DropShield()->unregisterHook(Game::VFunc::vCBasePlayerDropShield);
              }))
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
        return m_giveShield;
    }

    nstd::observer_ptr<IBasePlayerDropShieldHookRegistry> BasePlayerHooks::dropShield()
    {
        return m_dropShield;
    }
} // namespace Anubis::Game::CStrike
