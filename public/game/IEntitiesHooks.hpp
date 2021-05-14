/*
 *  Copyright (C) 2020 Metamod++ Development Team
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

#include <string>

namespace Metamod::Engine
{
    class IEdict;
    class IEntVars;
    class ITraceResult;
}

namespace Metamod::Game
{
    namespace Entities
    {
        class IBaseEntity;
        class IBasePlayer;
    }

    using IBasePlayerSpawnHook = IClassHook<void, Entities::IBasePlayer *>;
    using IBasePlayerSpawnHookRegistry = IClassHookRegistry<void, Entities::IBasePlayer *>;

    using IBasePlayerTakeDamageHook = IClassHook<std::int32_t, Entities::IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, int>;
    using IBasePlayerTakeDamageHookRegistry = IClassHookRegistry<std::int32_t, Entities::IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, int>;

    using IBasePlayerTraceAttackHook = IClassHook<void, Entities::IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, int>;
    using IBasePlayerTraceAttackHookRegistry = IClassHookRegistry<void, Entities::IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, int>;

    class IBasePlayerHooks
    {
    public:
        virtual ~IBasePlayerHooks() = default;

        virtual IBasePlayerSpawnHookRegistry *spawn() = 0;
        virtual IBasePlayerTakeDamageHookRegistry *takeDamage() = 0;
        virtual IBasePlayerTraceAttackHookRegistry *traceAttack() = 0;
    };
}
