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

#include "HookChains.hpp"
#include <game/IBasePlayer.hpp>
#include "BaseMonster.hpp"

#include <memory>

class CBasePlayer;

namespace Anubis::Game::Valve
{
    class BasePlayer : public BaseMonster, public virtual IBasePlayer
    {
    public:
        static constexpr const char *CLASS_NAME = "player";

    public:
        explicit BasePlayer(nstd::observer_ptr<Engine::IEdict> edict);
        ~BasePlayer() override = default;

    public:
        void makeVIP() override;
        void giveShield(bool deploy) override;
        void removeShield() override;
        void dropShield(bool deploy) override;
        [[nodiscard]] bool hasShield() const override;

    public:
        explicit operator CBasePlayer *() const;
    };
} // namespace Anubis::Game::Valve