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
#include <game/VFuncHelpers.hpp>

#include <unordered_map>

namespace Metamod::Game::Valve
{
    using BasePlayerSpawnHook = ClassHook<void, IBasePlayer *>;
    using BasePlayerSpawnHookRegistry = ClassHookRegistry<void, IBasePlayer *>;

    using BasePlayerTakeDamageHook = ClassHook<bool, IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, int>;
    using BasePlayerTakeDamageHookRegistry = ClassHookRegistry<bool, IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, int>;

    using BasePlayerTraceAttackHook = ClassHook<void, IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, std::int32_t>;
    using BasePlayerTraceAttackHookRegistry = ClassHookRegistry<void, IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, std::int32_t>;

    using BasePlayerKilledHook = ClassHook<void, IBasePlayer *, Engine::IEntVars *, GibType>;
    using BasePlayerKilledHookRegistry = ClassHookRegistry<void, IBasePlayer *, Engine::IEntVars *, GibType>;

    class BasePlayerHooks final : public IBasePlayerHooks
    {
    public:
        explicit BasePlayerHooks(const std::unordered_map<std::string, std::uint32_t> &vOffsets);
        ~BasePlayerHooks() final = default;

        BasePlayerSpawnHookRegistry *spawn() final;
        BasePlayerTakeDamageHookRegistry *takeDamage() final;
        BasePlayerTraceAttackHookRegistry *traceAttack() final;
        BasePlayerKilledHookRegistry *killed() final;

    private:
        BasePlayerSpawnHookRegistry m_spawn;
        BasePlayerTakeDamageHookRegistry m_takeDamage;
        BasePlayerTraceAttackHookRegistry m_traceAttack;
        BasePlayerKilledHookRegistry m_killed;
    };
}

extern std::unique_ptr<Metamod::Game::Valve::BasePlayerHooks> gBasePlayerHooks;
