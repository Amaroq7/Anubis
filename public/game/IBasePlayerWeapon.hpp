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

#pragma once

#include <observer_ptr.hpp>
#include "IBasePlayerItem.hpp"

class CBasePlayerWeapon;

namespace Anubis::Game
{
    class IBasePlayerWeapon : public virtual IBasePlayerItem
    {
    public:
        virtual ~IBasePlayerWeapon() = default;

        virtual bool addToPlayer(nstd::observer_ptr<IBasePlayer> pPlayer) const = 0;
        virtual bool addDuplicate(nstd::observer_ptr<IBasePlayerItem> pItem) const = 0;

        // Return TRUE if you can add ammo to yourself when picked up
        virtual bool extractAmmo(nstd::observer_ptr<IBasePlayerWeapon> pWeapon) const = 0;

        // Return TRUE if you can add ammo to yourself when picked up
        virtual bool extractClipAmmo(nstd::observer_ptr<IBasePlayerWeapon> pWeapon) const = 0;

        // Return TRUE if you want to add yourself to the player
        virtual bool addWeapon() const = 0;

        // updates HUD state
        virtual void updateItemInfo() const = 0;

        virtual bool playEmptySound() const = 0;
        virtual void resetEmptySound() const = 0;

        // skiplocal is true if client is predicting weapon animations
        virtual void sendWeaponAnim(int iAnim, bool skipLocal, int body) const = 0;

        virtual bool canDeploy() const = 0;
        virtual bool isUseable() const = 0;

        // called each frame by the player PostThink
        virtual void itemPostFrame() const = 0;

        // called by CBasePlayerWeapons ItemPostFrame()
        virtual void primaryAttack() const = 0;
        virtual void secondaryAttack() const = 0;
        virtual void reload() const = 0;

        // called when no buttons pressed
        virtual void weaponIdle() const = 0;

        // sends hud info to client dll, if things have changed
        bool updateClientData(nstd::observer_ptr<IBasePlayer> pPlayer) const = 0;
        virtual void retireWeapon() const = 0;

        virtual bool shouldWeaponIdle() const = 0;
        virtual void holster(bool skipLocal) const = 0;
        virtual bool useDecrement() const = 0;

        virtual std::int16_t primaryAmmoIndex() const = 0;
        virtual std::int16_t secondaryAmmoIndex() const = 0;

        virtual std::unique_ptr<IBasePlayerWeapon> getWeaponPtr() const = 0;

    public:
        virtual explicit operator ::CBasePlayerWeapon *() const = 0;
    };
} // namespace Anubis::Game
