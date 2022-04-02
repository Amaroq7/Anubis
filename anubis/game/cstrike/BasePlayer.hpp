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

#include <game/IBasePlayer.hpp>
#include "BaseMonster.hpp"

class CBasePlayer;

namespace Anubis::Game::CStrike
{
    class BasePlayer final : public BaseMonster, public virtual IBasePlayer
    {
    public:
        static constexpr const char *CLASS_NAME = "player";

    public:
        explicit BasePlayer(nstd::observer_ptr<Engine::IEdict> edict);
        ~BasePlayer() final = default;

    public:
        void makeVIP() final;
        void giveShield(bool deploy) final;
        void removeShield() final;
        void dropShield(bool deploy) final;
        [[nodiscard]] bool hasShield() const final;
        [[nodiscard]] std::optional<nstd::observer_ptr<IBaseEntity>> giveNamedItem(std::string_view item) const final;
        [[nodiscard]] nstd::observer_ptr<IBaseEntity> giveNamedItemEx(std::string_view item) const final;
        [[nodiscard]] bool hasNamedPlayerItem(std::string_view item) const final;
        void RenewItems() final;
        void PackDeadPlayerItems() final;
        void RemoveAllItems(bool removeSuit) final;
        [[nodiscard]] bool IsOnLadder() const final;
        [[nodiscard]] bool FlashlightIsOn() const final;
        void FlashlightTurnOn() final;
        void FlashlightTurnOff() final;

    public:
        explicit operator CBasePlayer *() const;
    };
} // namespace Anubis::Game::CStrike