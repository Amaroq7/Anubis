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

#include "../../observer_ptr.hpp"
#include "../../IHookChains.hpp"
#include "../../engine/Common.hpp"

namespace Anubis::Game::CStrike
{
    enum class WinStatus : std::uint8_t
    {
        None = 0,
        CTs,
        Terrorists,
        Draw,
    };

    enum class ScenarioEventEndRound : std::uint8_t
    {
        None = 0,
        TargetBomb,
        VIPEscaped,
        VIPAssassinated,
        TerroristsEscaped,
        CTsPreventEscape,
        EscapingTerroristsNeutralized,
        BombDefused,
        CTsWin,
        TerroristsWin,
        Draw,
        AllHostagesRescued,
        TargetSaved,
        HostagesNotRescued,
        TerroristsNotEscaped,
        VIPNotEscaped,
        GameCommence,
        GameRestart,
        GameOver
    };

    using IRoundEndHook = IHook<bool, WinStatus, ScenarioEventEndRound, float>;
    using IRoundEndHookRegistry = IHookRegistry<bool, WinStatus, ScenarioEventEndRound, float>;

    using IRoundFreezeEndHook = IHook<void>;
    using IRoundFreezeEndHookRegistry = IHookRegistry<void>;

    class IHooks
    {
    public:
        virtual ~IHooks() = default;

        virtual nstd::observer_ptr<IRoundEndHookRegistry> roundEnd() = 0;
        virtual nstd::observer_ptr<IRoundFreezeEndHookRegistry> freezeTimeEnd() = 0;
    };
} // namespace Anubis::Game::CStrike
