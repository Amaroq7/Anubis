/*
 *  Copyright (C) 2023 Anubis Development Team
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

#include "BasePlayerWeapon.hpp"
#include "BasePlayer.hpp"

#include <engine/IEdict.hpp>

#include <extdll.h>
#include <tier0/platform.h>
#include <util.h>
#include <cbase.h>
#include <weapons.h>

namespace Anubis::Game::Valve
{
    BasePlayerWeapon::BasePlayerWeapon(nstd::observer_ptr<Engine::IEdict> edict) : BasePlayerItem(edict) {}

    BasePlayerWeapon::operator ::CBasePlayerWeapon *() const
    {
        return static_cast<CBasePlayerWeapon *>(m_entity);
    }

    bool BasePlayerWeapon::addToPlayer(nstd::observer_ptr<IBasePlayer> pPlayer) const
    {
        return operator ::CBasePlayerWeapon *()->AddToPlayer(static_cast<CBasePlayer *>(*pPlayer)) == TRUE;
    }

    bool BasePlayerWeapon::addDuplicate(nstd::observer_ptr<IBasePlayerItem> pItem) const
    {
        return operator ::CBasePlayerWeapon *()->AddDuplicate(static_cast<CBasePlayerItem *>(*pItem)) == TRUE;
    }

    bool BasePlayerWeapon::extractAmmo(nstd::observer_ptr<IBasePlayerWeapon> pWeapon) const
    {
        return operator ::CBasePlayerWeapon *()->ExtractAmmo(static_cast<CBasePlayerWeapon *>(*pWeapon)) == TRUE;
    }

    bool BasePlayerWeapon::extractClipAmmo(nstd::observer_ptr<IBasePlayerWeapon> pWeapon) const
    {
        return operator ::CBasePlayerWeapon *()->ExtractClipAmmo(static_cast<CBasePlayerWeapon *>(*pWeapon)) == TRUE;
    }

    bool BasePlayerWeapon::addWeapon() const
    {
        return operator ::CBasePlayerWeapon *()->AddWeapon() == TRUE;
    }

    void BasePlayerWeapon::updateItemInfo() const
    {
        operator ::CBasePlayerWeapon *()->UpdateItemInfo();
    }

    bool BasePlayerWeapon::playEmptySound() const
    {
        return operator ::CBasePlayerWeapon *()->PlayEmptySound() == TRUE;
    }

    void BasePlayerWeapon::resetEmptySound() const
    {
        operator ::CBasePlayerWeapon *()->ResetEmptySound();
    }

    void BasePlayerWeapon::sendWeaponAnim(int iAnim, bool skipLocal, int body) const
    {
        operator ::CBasePlayerWeapon *()->SendWeaponAnim(iAnim, static_cast<int>(skipLocal), body);
    }

    bool BasePlayerWeapon::canDeploy() const
    {
        return operator ::CBasePlayerWeapon *()->CanDeploy() == TRUE;
    }

    bool BasePlayerWeapon::isUseable() const
    {
        return operator ::CBasePlayerWeapon *()->IsUseable() == TRUE;
    }

    void BasePlayerWeapon::itemPostFrame() const
    {
        operator ::CBasePlayerWeapon *()->ItemPostFrame();
    }

    void BasePlayerWeapon::primaryAttack() const
    {
        operator ::CBasePlayerWeapon *()->PrimaryAttack();
    }

    void BasePlayerWeapon::secondaryAttack() const
    {
        operator ::CBasePlayerWeapon *()->SecondaryAttack();
    }

    void BasePlayerWeapon::reload() const
    {
        operator ::CBasePlayerWeapon *()->Reload();
    }

    void BasePlayerWeapon::weaponIdle() const
    {
        operator ::CBasePlayerWeapon *()->WeaponIdle();
    }

    bool BasePlayerWeapon::updateClientData(nstd::observer_ptr<IBasePlayer> pPlayer) const
    {
        return operator ::CBasePlayerWeapon *()->UpdateClientData(static_cast<CBasePlayer *>(*pPlayer)) == TRUE;
    }

    void BasePlayerWeapon::retireWeapon() const
    {
        operator ::CBasePlayerWeapon *()->RetireWeapon();
    }

    bool BasePlayerWeapon::shouldWeaponIdle() const
    {
        return operator ::CBasePlayerWeapon *()->ShouldWeaponIdle() == TRUE;
    }

    void BasePlayerWeapon::holster(bool skipLocal) const
    {
        operator ::CBasePlayerWeapon *()->Holster(static_cast<int>(skipLocal));
    }

    bool BasePlayerWeapon::useDecrement() const
    {
        return operator ::CBasePlayerWeapon *()->UseDecrement() == TRUE;
    }

    std::int16_t BasePlayerWeapon::primaryAmmoIndex() const
    {
        return static_cast<std::int16_t>(operator ::CBasePlayerWeapon *()->PrimaryAmmoIndex());
    }

    std::int16_t BasePlayerWeapon::secondaryAmmoIndex() const
    {
        return static_cast<std::int16_t>(operator ::CBasePlayerWeapon *()->SecondaryAmmoIndex());
    }

    std::unique_ptr<IBasePlayerWeapon> BasePlayerWeapon::getWeaponPtr() const
    {
        auto weaponPtr = std::make_unique<BasePlayerWeapon>(m_edict);
        return weaponPtr;
    }
} // namespace Anubis::Game::Valve
