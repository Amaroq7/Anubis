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
#include "ReGameHeaders.hpp"
#include "EntityHolder.hpp"

namespace Anubis::Game::CStrike
{
    BasePlayer::BasePlayer(nstd::observer_ptr<Engine::IEdict> edict) : BaseMonster(edict) {}

    BasePlayer::operator CBasePlayer *() const
    {
        return reinterpret_cast<CBasePlayer *>(m_entity);
    }

    void BasePlayer::makeVIP()
    {
        operator CBasePlayer *()->CSPlayer()->MakeVIP();
    }

    void BasePlayer::giveShield(bool deploy)
    {
        operator CBasePlayer *()->CSPlayer()->GiveShield(deploy);
    }

    void BasePlayer::removeShield()
    {
        operator CBasePlayer *()->CSPlayer()->RemoveShield();
    }

    void BasePlayer::dropShield(bool deploy)
    {
        operator CBasePlayer *()->CSPlayer()->DropShield(deploy);
    }

    bool BasePlayer::hasShield() const
    {
        return operator CBasePlayer *()->m_bOwnsShield;
    }

    std::optional<nstd::observer_ptr<IBaseEntity>> BasePlayer::giveNamedItem(std::string_view item) const
    {
        return getEntityHolder()->getBaseEntity(operator CBasePlayer *()->CSPlayer()->GiveNamedItem(item.data()));
    }

    nstd::observer_ptr<IBaseEntity> BasePlayer::giveNamedItemEx(std::string_view item) const
    {
        return getEntityHolder()->getBaseEntity(operator CBasePlayer *()->CSPlayer()->GiveNamedItemEx(item.data()));
    }

    bool BasePlayer::hasNamedPlayerItem(std::string_view item) const
    {
        return operator CBasePlayer *()->CSPlayer()->HasNamedPlayerItem(item.data());
    }

    void BasePlayer::renewItems()
    { /* HL Only */
    }

    void BasePlayer::packDeadPlayerItems()
    { /* HL Only */
    }

    void BasePlayer::removeAllItems(bool removeSuit)
    {
        operator CBasePlayer *()->CSPlayer()->RemoveAllItems(removeSuit);
    }

    bool BasePlayer::isOnLadder() const
    {
        return m_entity->pev->movetype == MOVETYPE_FLY;
    }

    bool BasePlayer::flashlightIsOn() const
    {
        return m_entity->pev->effects & EF_DIMLIGHT;
    }

    void BasePlayer::flashlightTurnOn()
    {
        // operator CBasePlayer *()->FlashlightTurnOn();
    }

    void BasePlayer::flashlightTurnOff()
    {
        // operator CBasePlayer *()->FlashlightTurnOff();
    }
} // namespace Anubis::Game::CStrike
