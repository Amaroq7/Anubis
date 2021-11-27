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
#include <cstddef>
#include <memory>

#if defined ANUBIS_CORE || defined ANUBIS_ENTITY_DLL
typedef struct edict_s edict_t;
typedef struct entvars_s entvars_t;
#endif

namespace Anubis::Engine
{
    enum class EntFlags : std::uint32_t
    {
        Fly = (1u << 0),  // Changes the SV_Movestep() behavior to not need to be on ground
        Swim = (1u << 1), // Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
        Conveyor = (1u << 2),
        Client = (1u << 3),
        InWater = (1u << 4),
        Monster = (1u << 5),
        GodMode = (1u << 6),
        NoTarget = (1u << 7),
        SkipLocalHost = (1u << 8),  // Don't send entity to local host, it's predicting this entity itself
        OnGround = (1u << 9),       // At rest / on the ground
        PartialGround = (1u << 10), // not all corners are valid
        WaterJump = (1u << 11),     // player jumping out of water
        Frozen = (1u << 12),        // Player is frozen for 3rd person camera
        FakeClient = (1u << 13),    // JAC: fake client, simulated server side; don't send network messages to them
        Ducking = (1u << 14),       // Player flag -- Player is fully crouched
        Float = (1u << 15),         // Apply floating force to this entity when in water
        Graphed = (1u << 16),       // worldgraph has this ent listed as something that blocks a connection

        ImmuneWater = (1u << 17),
        ImmuneSlime = (1u << 18),
        ImmuneLava = (1u << 19),

        Proxy = (1u << 20),       // This is a spectator proxy
        AlwaysThink = (1u << 21), // Brush model flag -- call think every frame regardless of nextthink - ltime (for
        // constantly changing velocity/path)
        BaseVelocity =
            (1u << 22), // Base velocity has been applied this frame (used to convert base velocity into momentum)
        MonsterClip = (1u << 23), // Only collide in with monsters who have FL_MONSTERCLIP set
        OnTrain = (1u << 24),     // Player is _controlling_ a train, so movement commands should be ignored on client
        // during prediction.
        WorldBrush = (1u << 25), // Not moveable/removeable brush entity (really part of the world, but represented
        // as an entity for transparency or something)
        Spectator = (1u << 26),    // This client is a spectator, don't run touch functions, etc.
        CustomEntity = (1u << 29), // This is a custom entity
        KillMe = (1u << 30),       // This entity is marked for death -- This allows the engine to kill ents at the
        // appropriate time
        Dormant = (1u << 31) // Entity is dormant, no updates to client
    };

    class IEdict
    {
    public:
        virtual ~IEdict() = default;

        /**
         * @brief Returns edict's index.
         *
         * @return Edict's index.
         */
        [[nodiscard]] virtual std::uint32_t getIndex() const = 0;

        /**
         * @brief Returns edict's index.
         *
         * @return Edict's index.
         */
        [[nodiscard]] virtual std::uint32_t getSerialNumber() const = 0;

        /**
         * @brief Checks if edict is freed.
         *
         * @return True if edict is freed, false otherwise.
         */
        [[nodiscard]] virtual bool isFree() const = 0;

        [[nodiscard]] virtual EntFlags getFlags() const = 0;
        virtual void setFlags(EntFlags flags) = 0;

#if defined ANUBIS_CORE || defined ANUBIS_ENTITY_DLL
        [[nodiscard]] virtual void *getPrivateData() const = 0;
        virtual explicit operator edict_t *() const = 0;
        virtual explicit operator entvars_t *() const = 0;
#endif
    };
} // namespace Anubis::Engine