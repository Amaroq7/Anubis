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

#include "../IHookChains.hpp"
#include "IBaseMonster.hpp"

#include <optional>

/**
 * @internal A internal representation of a base player in gamedll
 */
class CBasePlayer;

namespace Anubis::Game
{
    class IBasePlayer : public virtual IBaseMonster
    {
    public:
        ~IBasePlayer() override = default;
        virtual void makeVIP() = 0;
        virtual void giveShield(bool deploy) = 0;
        virtual void removeShield() = 0;
        virtual void dropShield(bool deploy) = 0;
        [[nodiscard]] virtual bool hasShield() const = 0;
        [[nodiscard]] virtual std::optional<nstd::observer_ptr<IBaseEntity>>
            giveNamedItem(std::string_view item) const = 0;
        [[nodiscard]] virtual nstd::observer_ptr<IBaseEntity> giveNamedItemEx(std::string_view item) const = 0;
        [[nodiscard]] virtual bool hasNamedPlayerItem(std::string_view item) const = 0;
        virtual void RenewItems() = 0;
        virtual void PackDeadPlayerItems() = 0;
        virtual void RemoveAllItems(bool removeSuit) = 0;
        [[nodiscard]] virtual bool IsOnLadder() const = 0;
        [[nodiscard]] virtual bool FlashlightIsOn() const = 0;
        virtual void FlashlightTurnOn() = 0;
        virtual void FlashlightTurnOff() = 0;
        // BOOL SwitchWeapon(CBasePlayerItem *pWeapon);

        /**
         * @cond INTERNAL
         */
        /**
         * @internal Allows to get a internal representation of a base player in the gamedll.
         */
        explicit virtual operator CBasePlayer *() const = 0;

#if defined ANUBIS_ENTITY_DLL

    public:
        static inline std::intptr_t VTable;
#endif
    };
} // namespace Anubis::Game