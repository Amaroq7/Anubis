/*
 *  Copyright (C) 2023 Anubis Development Team
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

#include <game/cstrike/IHooks.hpp>
#include <HookChains.hpp>

class IReGameHookchains;

namespace Anubis::Game::CStrike
{
    using RoundEndHook = Hook<bool, WinStatus, ScenarioEventEndRound, float>;
    using RoundEndHookRegistry = HookRegistry<bool, WinStatus, ScenarioEventEndRound, float>;

    using RoundFreezeEndHook = Hook<void>;
    using RoundFreezeEndHookRegistry = HookRegistry<void>;

    class Hooks final : public IHooks
    {
    public:
        explicit Hooks(nstd::observer_ptr<IReGameHookchains> hooks);
        ~Hooks() final = default;

        nstd::observer_ptr<IRoundEndHookRegistry> roundEnd() final;
        nstd::observer_ptr<IRoundFreezeEndHookRegistry> freezeTimeEnd() final;

    private:
        std::unique_ptr<RoundEndHookRegistry> m_roundEndHookRegistry;
        std::unique_ptr<RoundFreezeEndHookRegistry> m_freezeTimeEndHookRegistry;
    };
} // namespace Anubis::Game::CStrike

extern std::unique_ptr<Anubis::Game::CStrike::Hooks> gHooks;