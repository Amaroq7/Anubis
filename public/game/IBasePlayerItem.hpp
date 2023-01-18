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
#include "IBaseAnimating.hpp"

struct ItemInfo;
class CBasePlayerItem;

namespace Anubis::Game
{
    class IBasePlayer;
    class IBasePlayerWeapon;

    enum class ItemFlag : std::uint8_t
    {
        None = 0,
        SelectOnEmpty = (1 << 0),
        NoAutoReload = (1 << 1),
        NoAutoSwitchEmpty = (1 << 2),
        LimitInWorld = (1 << 3),
        Exhaustible = (1 << 4)
    };

    class IItemInfo
    {
    public:
        virtual ~IItemInfo() = default;

        virtual std::uint8_t getSlot() const = 0;
        virtual std::uint8_t getPosition() const = 0;
        virtual std::string_view getAmmo1() const = 0;
        virtual std::int16_t getMaxAmmo1() const = 0;
        virtual std::string_view getAmmo2() const = 0;
        virtual std::int16_t getMaxAmmo2() const = 0;
        virtual std::string_view getName() const = 0;
        virtual std::int16_t getMaxClip() const = 0;
        virtual std::uint8_t getId() const = 0;
        virtual ItemFlag getFlags() const = 0;
        virtual std::int16_t getWeight() const = 0;
    };

    class IBasePlayerItem : public virtual IBaseAnimating
    {
    public:
        virtual ~IBasePlayerItem() = default;

        virtual void setObjectCollisionBox() const = 0;

        // return TRUE if the item you want the item added to the player inventory
        virtual bool addToPlayer(nstd::observer_ptr<IBasePlayer> pPlayer) const = 0;

        // return TRUE if you want your duplicate removed from world
        virtual bool addDuplicate(nstd::observer_ptr<IBasePlayerItem> pItem) const = 0;

        virtual void destroyItem() const = 0;

        // default weapon touch
        virtual void defaultTouch(nstd::observer_ptr<IBaseEntity> pOther) const = 0;

        // when an item is first spawned, this think is run to determine when the object has hit the ground.
        virtual void fallThink() const = 0;

        // make a weapon visible and tangible
        virtual void materialize() const = 0;

        // the weapon desires to become visible and tangible, if the game rules allow for it
        virtual void attemptToMaterialize() const = 0;

        virtual std::unique_ptr<IItemInfo> getItemInfo() const = 0;
        virtual bool canDeploy() const = 0;

        // returns is deploy was successful
        virtual bool deploy() const = 0;

        // can this weapon be put away right now?
        virtual bool canHolster() const = 0;
        virtual void holster(bool skipLocal) const = 0;

        virtual void updateItemInfo() const = 0;

        // called each frame by the player PreThink
        virtual void itemPreFrame() const = 0;

        // called each frame by the player PostThink
        virtual void itemPostFrame() const = 0;

        virtual void drop() const = 0;
        virtual void kill() const = 0;
        virtual void attachToPlayer(nstd::observer_ptr<IBasePlayer> pPlayer) const = 0;

        virtual std::int16_t primaryAmmoIndex() const = 0;
        virtual std::int16_t secondaryAmmoIndex() const = 0;

        virtual bool updateClientData(nstd::observer_ptr<IBasePlayer> pPlayer) const = 0;
        virtual std::unique_ptr<IBasePlayerWeapon> getWeaponPtr() const = 0;
        virtual std::uint8_t getItemSlot() const = 0;

        virtual float GetMaxSpeed() const = 0;

    public:
        virtual explicit operator ::CBasePlayerItem *() const = 0;
    };
} // namespace Anubis::Game
