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

#include "BasePlayerItem.hpp"
#include "BasePlayer.hpp"
#include "BasePlayerWeapon.hpp"

namespace Anubis::Game::CStrike
{
    ItemInfo::ItemInfo() : m_itemInfo(std::make_unique<::ItemInfo>()) {}
    ItemInfo::ItemInfo(::ItemInfo *itemInfo) : m_itemInfo(itemInfo) {}

    ItemInfo::operator ::ItemInfo *() const
    {
        return std::visit(
            [](auto &&arg)
            {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, ::ItemInfo *>)
                {
                    return arg;
                }
                else
                {
                    return arg.get();
                }
            },
            m_itemInfo);
    }

    std::uint8_t ItemInfo::getSlot() const
    {
        return static_cast<std::uint8_t>(operator ::ItemInfo *()->iSlot);
    }

    std::uint8_t ItemInfo::getPosition() const
    {
        return static_cast<std::uint8_t>(operator ::ItemInfo *()->iPosition);
    }

    std::string_view ItemInfo::getAmmo1() const
    {
        return operator ::ItemInfo *()->pszAmmo1;
    }

    std::int16_t ItemInfo::getMaxAmmo1() const
    {
        return static_cast<std::int16_t>(operator ::ItemInfo *()->iMaxAmmo1);
    }

    std::string_view ItemInfo::getAmmo2() const
    {
        return operator ::ItemInfo *()->pszAmmo2;
    }

    std::int16_t ItemInfo::getMaxAmmo2() const
    {
        return static_cast<std::int16_t>(operator ::ItemInfo *()->iMaxAmmo2);
    }

    std::string_view ItemInfo::getName() const
    {
        return operator ::ItemInfo *()->pszName;
    }

    std::int16_t ItemInfo::getMaxClip() const
    {
        return static_cast<std::int16_t>(operator ::ItemInfo *()->iMaxClip);
    }

    std::uint8_t ItemInfo::getId() const
    {
        return static_cast<std::uint8_t>(operator ::ItemInfo *()->iId);
    }

    ItemFlag ItemInfo::getFlags() const
    {
        return static_cast<ItemFlag>(operator ::ItemInfo *()->iFlags);
    }

    std::int16_t ItemInfo::getWeight() const
    {
        return static_cast<std::int16_t>(operator ::ItemInfo *()->iWeight);
    }

    BasePlayerItem::BasePlayerItem(nstd::observer_ptr<Engine::IEdict> edict) : BaseAnimating(edict) {}

    BasePlayerItem::operator ::CBasePlayerItem *() const
    {
        return static_cast<CBasePlayerItem *>(m_entity);
    }

    void BasePlayerItem::setObjectCollisionBox() const
    {
        operator ::CBasePlayerItem *()->SetObjectCollisionBox();
    }

    bool BasePlayerItem::addToPlayer(nstd::observer_ptr<IBasePlayer> pPlayer) const
    {
        return operator ::CBasePlayerItem *()->AddToPlayer(static_cast<CBasePlayer *>(*pPlayer)) == TRUE;
    }

    bool BasePlayerItem::addDuplicate(nstd::observer_ptr<IBasePlayerItem> pItem) const
    {
        return operator ::CBasePlayerItem *()->AddDuplicate(static_cast<CBasePlayerItem *>(*pItem)) == TRUE;
    }

    void BasePlayerItem::destroyItem() const
    {
        // HL Only
    }

    void BasePlayerItem::defaultTouch(nstd::observer_ptr<IBaseEntity> pOther [[maybe_unused]]) const
    {
        // HL Only
    }

    void BasePlayerItem::fallThink() const
    {
        // HL Only
    }

    void BasePlayerItem::materialize() const
    {
        // HL Only
    }

    void BasePlayerItem::attemptToMaterialize() const
    {
        // HL Only
    }

    std::unique_ptr<IItemInfo> BasePlayerItem::getItemInfo() const
    {
        auto itemInfo = std::make_unique<ItemInfo>();
        operator ::CBasePlayerItem *()->GetItemInfo(static_cast<::ItemInfo *>(*itemInfo));
        return itemInfo;
    }

    bool BasePlayerItem::canDeploy() const
    {
        return operator ::CBasePlayerItem *()->CanDeploy() == TRUE;
    }

    bool BasePlayerItem::deploy() const
    {
        return operator ::CBasePlayerItem *()->Deploy() == TRUE;
    }

    bool BasePlayerItem::canHolster() const
    {
        return operator ::CBasePlayerItem *()->CanHolster() == TRUE;
    }

    void BasePlayerItem::holster(bool skipLocal) const
    {
        operator ::CBasePlayerItem *()->Holster(static_cast<int>(skipLocal));
    }

    void BasePlayerItem::updateItemInfo() const
    {
        operator ::CBasePlayerItem *()->UpdateItemInfo();
    }

    void BasePlayerItem::itemPreFrame() const
    {
        operator ::CBasePlayerItem *()->ItemPreFrame();
    }

    void BasePlayerItem::itemPostFrame() const
    {
        operator ::CBasePlayerItem *()->ItemPostFrame();
    }

    void BasePlayerItem::drop() const
    {
        operator ::CBasePlayerItem *()->Drop();
    }

    void BasePlayerItem::kill() const
    {
        operator ::CBasePlayerItem *()->Kill();
    }

    void BasePlayerItem::attachToPlayer(nstd::observer_ptr<IBasePlayer> pPlayer) const
    {
        operator ::CBasePlayerItem *()->AttachToPlayer(static_cast<CBasePlayer *>(*pPlayer));
    }

    std::int16_t BasePlayerItem::primaryAmmoIndex() const
    {
        return static_cast<std::int16_t>(operator ::CBasePlayerItem *()->PrimaryAmmoIndex());
    }

    std::int16_t BasePlayerItem::secondaryAmmoIndex() const
    {
        return static_cast<std::int16_t>(operator ::CBasePlayerItem *()->SecondaryAmmoIndex());
    }

    bool BasePlayerItem::updateClientData(nstd::observer_ptr<IBasePlayer> pPlayer) const
    {
        return static_cast<bool>(operator ::CBasePlayerItem *()->UpdateClientData(
            static_cast<CBasePlayer *>(*pPlayer)));
    }

    std::unique_ptr<IBasePlayerWeapon> BasePlayerItem::getWeaponPtr() const
    {
        return {};
    }

    std::uint8_t BasePlayerItem::getItemSlot() const
    {
        return static_cast<std::uint8_t>(operator ::CBasePlayerItem *()->iItemSlot());
    }

    float BasePlayerItem::GetMaxSpeed() const
    {
        return operator ::CBasePlayerItem *()->GetMaxSpeed();
    }
} // namespace Anubis::Game::CStrike
