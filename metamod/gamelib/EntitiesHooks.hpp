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

#include <gamelib/IEntitiesHooks.hpp>
#include <HookChains.hpp>

namespace Metamod::GameLib
{
    using BasePlayerSpawnHook = ClassHook<void, Entities::IBasePlayer *>;
    using BasePlayerSpawnHookRegistry = ClassHookRegistry<void, Entities::IBasePlayer *>;

    using BasePlayerTakeDamageHook = ClassHook<std::int32_t, Entities::IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, int>;
    using BasePlayerTakeDamageHookRegistry = ClassHookRegistry<std::int32_t, Entities::IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, int>;

    using BasePlayerTraceAttackHook = ClassHook<void, Entities::IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, int>;
    using BasePlayerTraceAttackHookRegistry = ClassHookRegistry<void, Entities::IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, int>;

    class BasePlayerHooks : public IBasePlayerHooks
    {
    public:
        ~BasePlayerHooks() override = default;

        BasePlayerSpawnHookRegistry *spawn() override;
        BasePlayerTakeDamageHookRegistry *takeDamage() override;
        BasePlayerTraceAttackHookRegistry *traceAttack() override;

    private:
        BasePlayerSpawnHookRegistry m_spawn;
        BasePlayerTakeDamageHookRegistry m_takeDamage;
        BasePlayerTraceAttackHookRegistry m_traceAttack;
    };
}
