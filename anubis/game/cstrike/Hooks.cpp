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

#include "Hooks.hpp"
#include "ReGameHeaders.hpp"

std::unique_ptr<Anubis::Game::CStrike::Hooks> gHooks;

namespace
{
    void OnRoundFreezeEnd(IReGameHook_CSGameRules_OnRoundFreezeEnd *chain)
    {
        static auto hookChain = gHooks->freezeTimeEnd();

        hookChain->callChain(
            [chain]()
            {
                chain->callNext();
            },
            [chain]()
            {
                chain->callOriginal();
            });
    }

    bool OnRoundEnd(IReGameHook_RoundEnd *chain, int winStatus, ScenarioEventEndRound event, float tmDelay)
    {
        static auto hookChain = gHooks->roundEnd();

        return hookChain->callChain(
            [chain](Anubis::Game::CStrike::WinStatus winStatus, Anubis::Game::CStrike::ScenarioEventEndRound event,
                    float tmDelay)
            {
                return chain->callNext(static_cast<int>(winStatus), static_cast<ScenarioEventEndRound>(event), tmDelay);
            },
            [chain](Anubis::Game::CStrike::WinStatus winStatus, Anubis::Game::CStrike::ScenarioEventEndRound event,
                    float tmDelay)
            {
                return chain->callOriginal(static_cast<int>(winStatus), static_cast<ScenarioEventEndRound>(event),
                                           tmDelay);
            },
            static_cast<Anubis::Game::CStrike::WinStatus>(winStatus),
            static_cast<Anubis::Game::CStrike::ScenarioEventEndRound>(event), tmDelay);
    }
} // namespace

namespace Anubis::Game::CStrike
{
    Hooks::Hooks(nstd::observer_ptr<IReGameHookchains> hooks)
        : m_roundEndHookRegistry(std::make_unique<RoundEndHookRegistry>(
              [hooks]()
              {
                  hooks->RoundEnd()->registerHook(OnRoundEnd);
              },
              [hooks]()
              {
                  hooks->RoundEnd()->unregisterHook(OnRoundEnd);
              })),
          m_freezeTimeEndHookRegistry(std::make_unique<RoundFreezeEndHookRegistry>(
              [hooks]()
              {
                  hooks->CSGameRules_OnRoundFreezeEnd()->registerHook(OnRoundFreezeEnd);
              },
              [hooks]()
              {
                  hooks->CSGameRules_OnRoundFreezeEnd()->unregisterHook(OnRoundFreezeEnd);
              }))
    {
    }

    nstd::observer_ptr<IRoundEndHookRegistry> Hooks::roundEnd()
    {
        return m_roundEndHookRegistry;
    }

    nstd::observer_ptr<IRoundFreezeEndHookRegistry> Hooks::freezeTimeEnd()
    {
        return m_freezeTimeEndHookRegistry;
    }
} // namespace Anubis::Game::CStrike
