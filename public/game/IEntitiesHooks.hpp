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

#include "ILibrary.hpp"
#include "../IHookChains.hpp"
#include "Consts.hpp"

#include <observer_ptr.hpp>

#include <string>

namespace Anubis::Engine
{
    class IEdict;
    class ITraceResult;
} // namespace Anubis::Engine

namespace Anubis::Game
{
    class IBaseEntity;
    class IBasePlayer;

    using IBasePlayerSpawnHook = IClassHook<void, nstd::observer_ptr<IBasePlayer>>;
    using IBasePlayerSpawnHookRegistry = IClassHookRegistry<void, nstd::observer_ptr<IBasePlayer>>;

    using IBasePlayerTakeDamageHook = IClassHook<bool,
                                                 nstd::observer_ptr<IBasePlayer>,
                                                 nstd::observer_ptr<IBaseEntity>,
                                                 nstd::observer_ptr<IBaseEntity>,
                                                 float &,
                                                 DmgType>;
    using IBasePlayerTakeDamageHookRegistry = IClassHookRegistry<bool,
                                                                 nstd::observer_ptr<IBasePlayer>,
                                                                 nstd::observer_ptr<IBaseEntity>,
                                                                 nstd::observer_ptr<IBaseEntity>,
                                                                 float &,
                                                                 DmgType>;

    using IBasePlayerTraceAttackHook = IClassHook<void,
                                                  nstd::observer_ptr<IBasePlayer>,
                                                  nstd::observer_ptr<IBaseEntity>,
                                                  float,
                                                  float *,
                                                  const std::unique_ptr<Engine::ITraceResult> &,
                                                  DmgType>;
    using IBasePlayerTraceAttackHookRegistry = IClassHookRegistry<void,
                                                                  nstd::observer_ptr<IBasePlayer>,
                                                                  nstd::observer_ptr<IBaseEntity>,
                                                                  float,
                                                                  float *,
                                                                  const std::unique_ptr<Engine::ITraceResult> &,
                                                                  DmgType>;

    using IBasePlayerKilledHook =
        IClassHook<void, nstd::observer_ptr<IBasePlayer>, nstd::observer_ptr<IBaseEntity>, GibType>;
    using IBasePlayerKilledHookRegistry =
        IClassHookRegistry<void, nstd::observer_ptr<IBasePlayer>, nstd::observer_ptr<IBaseEntity>, GibType>;

    using IBasePlayerGiveShieldHook = IClassHook<void, nstd::observer_ptr<IBasePlayer>, bool>;
    using IBasePlayerGiveShieldHookRegistry = IClassHookRegistry<void, nstd::observer_ptr<IBasePlayer>, bool>;

    using IBasePlayerDropShieldHook =
        IClassHook<nstd::observer_ptr<IBaseEntity>, nstd::observer_ptr<IBasePlayer>, bool>;
    using IBasePlayerDropShieldHookRegistry =
        IClassHookRegistry<nstd::observer_ptr<IBaseEntity>, nstd::observer_ptr<IBasePlayer>, bool>;

    class IBasePlayerHooks
    {
    public:
        virtual ~IBasePlayerHooks() = default;

        virtual nstd::observer_ptr<IBasePlayerSpawnHookRegistry> spawn() = 0;
        virtual nstd::observer_ptr<IBasePlayerTakeDamageHookRegistry> takeDamage() = 0;
        virtual nstd::observer_ptr<IBasePlayerTraceAttackHookRegistry> traceAttack() = 0;
        virtual nstd::observer_ptr<IBasePlayerKilledHookRegistry> killed() = 0;
        virtual nstd::observer_ptr<IBasePlayerGiveShieldHookRegistry> giveShield() = 0;
        virtual nstd::observer_ptr<IBasePlayerDropShieldHookRegistry> dropShield() = 0;
    };
} // namespace Anubis::Game
