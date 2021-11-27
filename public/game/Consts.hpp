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

#include <cinttypes>

namespace Anubis::Game
{
    enum class GibType : std::uint8_t
    {
        Normal = 0, /**< Gib corpse depending on the dmg type and its amount */
        Never,      /**< Never gib corpse */
        Always      /**< Always gib corpse */
    };

    enum class DmgType : std::uint32_t
    {
        Generic = 0,            /**< generic damage was done */
        Crush = (1 << 0),       /**< crushed by falling or moving object */
        Bullet = (1 << 1),      /**< shot */
        Slash = (1 << 2),       /**< cut, clawed, stabbed */
        Burn = (1 << 3),        /**< heat burned */
        Freeze = (1 << 4),      /**< frozen */
        Fall = (1 << 5),        /**< fell too far */
        Blast = (1 << 6),       /**< explosive blast damage */
        Club = (1 << 7),        /**< crowbar, punch, headbutt */
        Shock = (1 << 8),       /**< electric shock */
        Sonic = (1 << 9),       /**< sound pulse shockwave */
        EnergyBeam = (1 << 10), /**< laser or other high energy beam */
        NeverGib = (1 << 12),   /**< with this bit OR'd in, no damage type will be able to gib victims upon death */
        AlwaysGib = (1 << 13),  /**< with this bit OR'd in, any damage type can be made to gib victims upon death */
        Drown = (1 << 14),      /**< Drowning */

        // time-based damage
        TimeBased = ~(0x3FFFu), /**< mask for time-based damage */

        Paralyze = (1 << 15),     /**< slows affected creature down */
        NerveGas = (1 << 16),     /**< nerve toxins, very bad */
        Poison = (1 << 17),       /**< blood poisoning */
        Radiation = (1 << 18),    /**< radiation exposure */
        DrownRecover = (1 << 19), /**< drowning recovery */
        Acid = (1 << 20),         /**< toxic chemicals or acid burns */
        SlowBurn = (1 << 21),     /**< in an oven */
        SlowFreeze = (1 << 22),   /**< in a subzero freezer */
        Mortar = (1 << 23),       /**< Hit by air raid (done to distinguish grenade from mortar) */
        Explosion = (1 << 24),    /**< F.e. grenade explosion in CS */

        GibCorpse =
            (Crush | Fall | Blast | Sonic | Club), /**< These are the damage types that are allowed to gib corpses */

        ShownHud = (Poison | Acid | Freeze | SlowFreeze | Drown | Burn | SlowBurn | NerveGas | Radiation |
                    Shock) /**< These are the damage types that have client hud art */
    };
}; // namespace Anubis::Game