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

#include "BasePlayer.hpp"
#include "AnubisExports.hpp"

#include <extdll.h>
#include <tier0/platform.h>
#include <util.h>
#include <cbase.h>
#include <player.h>

namespace Anubis::Game::Valve
{
    BasePlayer::BasePlayer(nstd::observer_ptr<Engine::IEdict> edict) : BaseMonster(edict) {}

    BasePlayer::operator CBasePlayer *() const
    {
        return reinterpret_cast<CBasePlayer *>(m_entity);
    }

    void BasePlayer::makeVIP()
    { /* CStrike only */
    }
    void BasePlayer::giveShield(bool deploy [[maybe_unused]])
    { /* CStrike only */
    }
    void BasePlayer::removeShield()
    { /* CStrike only */
    }
    void BasePlayer::dropShield(bool deploy [[maybe_unused]])
    { /* CStrike only */
    }
    bool BasePlayer::hasShield() const
    {
        /* CStrike only */
        return false;
    }

    std::optional<nstd::observer_ptr<IBaseEntity>> BasePlayer::giveNamedItem(std::string_view item) const
    {
        operator CBasePlayer *()->GiveNamedItem(item.data());
        return std::nullopt;
    }
    nstd::observer_ptr<IBaseEntity> BasePlayer::giveNamedItemEx(std::string_view item [[maybe_unused]]) const
    {
        /* CStrike only */
        return {};
    }

    bool BasePlayer::hasNamedPlayerItem(std::string_view item) const
    {
        return operator CBasePlayer *()->HasNamedPlayerItem(item.data());
    }

    void BasePlayer::RenewItems()
    {
        operator CBasePlayer *()->RenewItems();
    }

    void BasePlayer::PackDeadPlayerItems()
    {
        operator CBasePlayer *()->PackDeadPlayerItems();
    }

    void BasePlayer::RemoveAllItems(bool removeSuit)
    {
        operator CBasePlayer *()->RemoveAllItems(removeSuit);
    }

    bool BasePlayer::IsOnLadder() const
    {
        return operator CBasePlayer *()->IsOnLadder();
    }

    bool BasePlayer::FlashlightIsOn() const
    {
        return operator CBasePlayer *()->FlashlightIsOn();
    }

    void BasePlayer::FlashlightTurnOn()
    {
        operator CBasePlayer *()->FlashlightTurnOn();
    }

    void BasePlayer::FlashlightTurnOff()
    {
        operator CBasePlayer *()->FlashlightTurnOff();
    }

} // namespace Anubis::Game::Valve
