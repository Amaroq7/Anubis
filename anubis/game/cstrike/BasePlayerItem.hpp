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

#include <game/IBasePlayerItem.hpp>
#include "BaseAnimating.hpp"

#include <extdll.h>
#include <tier0/platform.h>
#include <util.h>
#include <cbase.h>
#include <weapons.h>

#include <variant>

namespace Anubis::Game::CStrike
{
    class ItemInfo final : public IItemInfo
    {
    public:
        ItemInfo();
        ItemInfo(::ItemInfo *itemInfo);
        ~ItemInfo() final = default;

    public:
        std::uint8_t getSlot() const final;
        std::uint8_t getPosition() const final;
        std::string_view getAmmo1() const final;
        std::int16_t getMaxAmmo1() const final;
        std::string_view getAmmo2() const final;
        std::int16_t getMaxAmmo2() const final;
        std::string_view getName() const final;
        std::int16_t getMaxClip() const final;
        std::uint8_t getId() const final;
        ItemFlag getFlags() const final;
        std::int16_t getWeight() const final;

    public:
        explicit operator ::ItemInfo *() const;

    private:
        std::variant<::ItemInfo *, std::unique_ptr<::ItemInfo>> m_itemInfo;
    };

    class BasePlayerItem : public BaseAnimating, public virtual IBasePlayerItem
    {
    public:
        BasePlayerItem(nstd::observer_ptr<Engine::IEdict> edict);
        ~BasePlayerItem() override = default;

        void setObjectCollisionBox() const override;

        bool addToPlayer(nstd::observer_ptr<IBasePlayer> pPlayer) const override;

        bool addDuplicate(nstd::observer_ptr<IBasePlayerItem> pItem) const override;

        void destroyItem() const override;

        void defaultTouch(nstd::observer_ptr<IBaseEntity> pOther) const override;
        void fallThink() const override;
        void materialize() const override;

        void attemptToMaterialize() const override;

        std::unique_ptr<IItemInfo> getItemInfo() const override;
        bool canDeploy() const override;

        bool deploy() const override;

        bool canHolster() const override;
        void holster(bool skipLocal) const override;

        void updateItemInfo() const override;

        void itemPreFrame() const override;

        void itemPostFrame() const override;

        void drop() const override;
        void kill() const override;
        void attachToPlayer(nstd::observer_ptr<IBasePlayer> pPlayer) const override;

        std::int16_t primaryAmmoIndex() const override;
        std::int16_t secondaryAmmoIndex() const override;

        bool updateClientData(nstd::observer_ptr<IBasePlayer> pPlayer) const override;
        std::unique_ptr<IBasePlayerWeapon> getWeaponPtr() const override;
        std::uint8_t getItemSlot() const override;

        float GetMaxSpeed() const override;

    public:
        explicit operator ::CBasePlayerItem *() const final;
    };
} // namespace Anubis::Game::CStrike
