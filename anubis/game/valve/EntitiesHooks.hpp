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

#include <game/IEntitiesHooks.hpp>
#include <HookChains.hpp>
#include <observer_ptr.hpp>

#include <unordered_map>

namespace Anubis::Game::Valve
{
    using BasePlayerSpawnHook = ClassHook<void, nstd::observer_ptr<IBasePlayer>>;
    using BasePlayerSpawnHookRegistry = ClassHookRegistry<void, nstd::observer_ptr<IBasePlayer>>;

    using BasePlayerTakeDamageHook = ClassHook<bool,
                                               nstd::observer_ptr<IBasePlayer>,
                                               nstd::observer_ptr<IBaseEntity>,
                                               nstd::observer_ptr<IBaseEntity>,
                                               float &,
                                               DmgType>;
    using BasePlayerTakeDamageHookRegistry = ClassHookRegistry<bool,
                                                               nstd::observer_ptr<IBasePlayer>,
                                                               nstd::observer_ptr<IBaseEntity>,
                                                               nstd::observer_ptr<IBaseEntity>,
                                                               float &,
                                                               DmgType>;

    using BasePlayerTraceAttackHook = ClassHook<void,
                                                nstd::observer_ptr<IBasePlayer>,
                                                nstd::observer_ptr<IBaseEntity>,
                                                float,
                                                float *,
                                                const std::unique_ptr<Engine::ITraceResult> &,
                                                DmgType>;
    using BasePlayerTraceAttackHookRegistry = ClassHookRegistry<void,
                                                                nstd::observer_ptr<IBasePlayer>,
                                                                nstd::observer_ptr<IBaseEntity>,
                                                                float,
                                                                float *,
                                                                const std::unique_ptr<Engine::ITraceResult> &,
                                                                DmgType>;

    using BasePlayerKilledHook =
        ClassHook<void, nstd::observer_ptr<IBasePlayer>, nstd::observer_ptr<IBaseEntity>, GibType>;
    using BasePlayerKilledHookRegistry =
        ClassHookRegistry<void, nstd::observer_ptr<IBasePlayer>, nstd::observer_ptr<IBaseEntity>, GibType>;

    using BasePlayerGiveShieldHook = ClassHook<void, nstd::observer_ptr<IBasePlayer>, bool>;
    using BasePlayerGiveShieldHookRegistry = ClassHookRegistry<void, nstd::observer_ptr<IBasePlayer>, bool>;

    using BasePlayerDropShieldHook = ClassHook<nstd::observer_ptr<IBaseEntity>, nstd::observer_ptr<IBasePlayer>, bool>;
    using BasePlayerDropShieldHookRegistry =
        ClassHookRegistry<nstd::observer_ptr<IBaseEntity>, nstd::observer_ptr<IBasePlayer>, bool>;

    class BasePlayerHooks final : public IBasePlayerHooks
    {
    public:
        explicit BasePlayerHooks(std::unordered_map<std::string, std::uint32_t> &&vOffsets);
        ~BasePlayerHooks() final = default;

        nstd::observer_ptr<IBasePlayerSpawnHookRegistry> spawn() final;
        nstd::observer_ptr<IBasePlayerTakeDamageHookRegistry> takeDamage() final;
        nstd::observer_ptr<IBasePlayerTraceAttackHookRegistry> traceAttack() final;
        nstd::observer_ptr<IBasePlayerKilledHookRegistry> killed() final;
        nstd::observer_ptr<IBasePlayerGiveShieldHookRegistry> giveShield() final;
        nstd::observer_ptr<IBasePlayerDropShieldHookRegistry> dropShield() final;

    private:
        std::unique_ptr<BasePlayerSpawnHookRegistry> m_spawn;
        std::unique_ptr<BasePlayerTakeDamageHookRegistry> m_takeDamage;
        std::unique_ptr<BasePlayerTraceAttackHookRegistry> m_traceAttack;
        std::unique_ptr<BasePlayerKilledHookRegistry> m_killed;
    };

    const std::unique_ptr<BasePlayerHooks> &
        getBasePlayerHooks(std::unordered_map<std::string, std::uint32_t> &&vOffsets = {});
} // namespace Anubis::Game::Valve
