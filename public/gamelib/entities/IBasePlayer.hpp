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

#include <IHookChains.hpp>
#include <gamelib/entities/IBaseMonster.hpp>

namespace Metamod::GameLib::Entities
{
    class IBasePlayer;

    using IBasePlayerSpawnHook = IClassHook<void, IBasePlayer *>;
    using IBasePlayerSpawnHookRegistry = IClassHookRegistry<void, IBasePlayer *>;

    using IBasePlayerTakeDamageHook = IClassHook<void, IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, int>;
    using IBasePlayerTakeDamageHookRegistry = IClassHookRegistry<void, IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, int>;

    using IBasePlayerTraceAttackHook = IClassHook<void, IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, int>;
    using IBasePlayerTraceAttackHookRegistry = IClassHookRegistry<void, IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, int>;

    class IBasePlayerHooks
    {
    public:
        virtual ~IBasePlayerHooks() = default;

        virtual IBasePlayerSpawnHookRegistry *spawn() = 0;
        virtual IBasePlayerTakeDamageHookRegistry *takeDamage() = 0;
        virtual IBasePlayerTraceAttackHookRegistry *traceAttack() = 0;
    };

    class IBasePlayer : public virtual IBaseMonster
    {
        public:
            ~IBasePlayer() override = default;

            virtual void makeVIP() = 0;
    };
}