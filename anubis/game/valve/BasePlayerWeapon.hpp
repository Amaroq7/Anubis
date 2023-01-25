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

#include <game/IBasePlayerWeapon.hpp>
#include "BasePlayerItem.hpp"

namespace Anubis::Game::Valve
{
    class BasePlayerWeapon : public BasePlayerItem, public virtual IBasePlayerWeapon
    {
    public:
        explicit BasePlayerWeapon(nstd::observer_ptr<Engine::IEdict> edict);
        ~BasePlayerWeapon() override = default;

        bool addToPlayer(nstd::observer_ptr<IBasePlayer> pPlayer) const override;
        bool addDuplicate(nstd::observer_ptr<IBasePlayerItem> pItem) const override;

        // Return TRUE if you can add ammo to yourself when picked up
        bool extractAmmo(nstd::observer_ptr<IBasePlayerWeapon> pWeapon) const override;

        // Return TRUE if you can add ammo to yourself when picked up
        bool extractClipAmmo(nstd::observer_ptr<IBasePlayerWeapon> pWeapon) const override;

        // Return TRUE if you want to add yourself to the player
        bool addWeapon() const override;

        // updates HUD state
        void updateItemInfo() const override;

        bool playEmptySound() const override;
        void resetEmptySound() const override;

        // skiplocal is true if client is predicting weapon animations
        void sendWeaponAnim(int iAnim, bool skipLocal, int body) const override;

        bool canDeploy() const override;
        bool isUseable() const override;

        // called each frame by the player PostThink
        void itemPostFrame() const override;

        // called by CBasePlayerWeapons ItemPostFrame()
        void primaryAttack() const override;
        void secondaryAttack() const override;
        void reload() const override;

        // called when no buttons pressed
        void weaponIdle() const override;

        // sends hud info to client dll, if things have changed
        bool updateClientData(nstd::observer_ptr<IBasePlayer> pPlayer) const override;
        void retireWeapon() const override;

        bool shouldWeaponIdle() const override;
        void holster(bool skipLocal) const override;
        bool useDecrement() const override;

        std::int16_t primaryAmmoIndex() const override;
        std::int16_t secondaryAmmoIndex() const override;

        std::unique_ptr<IBasePlayerWeapon> getWeaponPtr() const override;

    public:
        virtual explicit operator ::CBasePlayerWeapon *() const final;
    };
} // namespace Anubis::Game::Valve
