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

#include <game/IEntitiesHooks.hpp>
#include <HookChains.hpp>
#include "VFuncHelpers.hpp"

namespace Metamod::Game
{
    using BasePlayerSpawnHook = ClassHook<void, Entities::IBasePlayer *>;
    using BasePlayerSpawnHookRegistry = ClassHookRegistry<void, Entities::IBasePlayer *>;

    using BasePlayerTakeDamageHook = ClassHook<std::int32_t, Entities::IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, int>;
    using BasePlayerTakeDamageHookRegistry = ClassHookRegistry<std::int32_t, Entities::IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, int>;

    using BasePlayerTraceAttackHook = ClassHook<void, Entities::IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, std::int32_t>;
    using BasePlayerTraceAttackHookRegistry = ClassHookRegistry<void, Entities::IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, std::int32_t>;

    class BasePlayerHooks final : public IBasePlayerHooks
    {
    public:
        BasePlayerHooks(const std::unordered_map<std::string, std::uint32_t> &vOffsets);
        ~BasePlayerHooks() final = default;

        BasePlayerSpawnHookRegistry *spawn() final;
        BasePlayerTakeDamageHookRegistry *takeDamage() final;
        BasePlayerTraceAttackHookRegistry *traceAttack() final;

    private:
        BasePlayerSpawnHookRegistry m_spawn;
        BasePlayerTakeDamageHookRegistry m_takeDamage;
        BasePlayerTraceAttackHookRegistry m_traceAttack;
    };
}
