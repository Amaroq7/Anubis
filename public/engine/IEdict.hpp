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

#include <engine/Common.hpp>

#include <cinttypes>
#include <cstddef>
#include <memory>

typedef struct edict_s edict_t;
typedef struct entvars_s entvars_t;

namespace Anubis::Engine
{
    enum class FixAngle : std::uint8_t
    {
        Nothing = 0,
        Force,
        AddAVelocity
    };

    class IEdict
    {
    public:
        enum class SolidType : std::uint8_t
        {
            Not = 0,
            Trigger,
            BBox,
            SlideBox,
            BSP
        };

        enum class MoveType : std::uint8_t
        {
            None = 0,
            Walk = 3,
            Step = 4,
            Fly = 5,
            Toss = 6,
            Push = 7,
            NoClip = 8,
            FlyMissle = 9,
            Bounce = 10,
            BounceMissile = 11,
            Follow = 12,
            PushStep = 13
        };

        enum class Flag : std::uint32_t
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
            OnTrain = (1u << 24), // Player is _controlling_ a train, so movement commands should be ignored on client
            // during prediction.
            WorldBrush = (1u << 25), // Not moveable/removeable brush entity (really part of the world, but represented
            // as an entity for transparency or something)
            Spectator = (1u << 26),    // This client is a spectator, don't run touch functions, etc.
            CustomEntity = (1u << 29), // This is a custom entity
            KillMe = (1u << 30),       // This entity is marked for death -- This allows the engine to kill ents at the
            // appropriate time
            Dormant = (1u << 31) // Entity is dormant, no updates to client
        };

        enum class VecProperty : std::uint8_t
        {
            Origin = 0,
            OldOrigin,
            Velocity,
            BaseVelocity,
            ClBaseVelocity,
            MoveDir,
            Angles,
            AVelocity,
            PunchAngle,
            ViewingAngle,
            EndPos,
            StartPos,
            AbsMin,
            AbsMax,
            Mins,
            Maxs,
            Size,
            RenderColor,
            ViewingOffset,
            User1,
            User2,
            User3,
            User4
        };

        enum class StrProperty : std::uint8_t
        {
            ClassName = 0,
            GlobalName,
            Model,
            ViewModel,
            WeaponModel,
            Target,
            TargetName,
            NetName,
            Message,
            Noise,
            Noise1,
            Noise2,
            Noise3
        };

        enum class FlProperty : std::uint8_t
        {
            ImpactTime = 0,
            StartTime,
            IdealPitch,
            IdealYaw,
            SpeedPitch,
            SpeedYaw,
            LTime,
            NextThink,
            Gravity,
            Friction,
            Frame,
            AnimTime,
            FrameRate,
            Scale,
            RenderAmount,
            Health,
            Frags,
            TakeDamage,
            MaxHealth,
            TeleportTime,
            ArmorType,
            ArmorValue,
            DmgTake,
            DmgSave,
            Dmg,
            DmgTime,
            Speed,
            AirFinished,
            PainFinished,
            RadSuitFinished,
            MaxSpeed,
            Fov,
            FallVelocity,
            User1,
            User2,
            User3,
            User4
        };

        enum class IntProperty : std::uint8_t
        {
            Skin = 0,
            Body,
            Sequence,
            GaitSequence,
            Weapons,
            Team,
            WaterLevel,
            WaterType,
            PlayerClass,
            WeaponAnim,
            PushMSec,
            InDuck,
            TimeStepSound,
            SwimTime,
            DuckTime,
            StepLeft,
            GameState,
            GroupInfo,
            User1,
            User2,
            User3,
            User4
        };

        enum class ShortProperty : std::uint8_t
        {
            Button = 0,
            OldButtons
        };

        enum class UShortProperty : std::uint8_t
        {
            ColorMap = 0
        };

        enum class ByteProperty : std::uint8_t
        {
            Impulse = 0
        };

        enum class EdictProperty : std::uint8_t
        {
            Chain = 0,
            DmgInflictor,
            Enemy,
            Aiment,
            Owner,
            GroundEntity,
            User1,
            User2,
            User3,
            User4
        };

        enum class RenderMode : std::uint8_t
        {
            Normal = 0,
            TransColor,
            TransTexture,
            Glow,
            TransAlpha,
            TransAdd
        };

        enum class RenderFx : std::uint8_t
        {
            None = 0,
            PulseSlow,
            PulseFast,
            PulseSlowWide,
            PulseFastWide,
            FadeSlow,
            FadeFast,
            SolidSlow,
            SolidFast,
            StrobeSlow,
            StrobeFast,
            StrobeFaster,
            FlickerSlow,
            FlickerFast,
            NoDissipation,
            Distort,
            Hologram,
            FxDeadPlayer,
            FxExplode,
            FxGlowShell,
            FxClampMinScale,
            FxLightMultiplier
        };

        enum class DeadFlag : std::uint8_t
        {
            No = 0,
            Dying,
            Dead,
            Respawnable,
            DiscardBody
        };

        enum class SpawnFlag
        {
            NotInDeathMatch = 1 << 11,
            NoRespawn = 1 << 30
        };

        enum class Effects : std::uint16_t
        {
            None = 0,
            BrightField = (1 << 0),
            MuzzleFlash = (1 << 1),
            BrightLight = (1 << 2),
            DimLight = (1 << 3),
            InvLight = (1 << 4),
            NoInterp = (1 << 5),
            Light = (1 << 6),
            NoDraw = (1 << 7),
            NightVision = (1 << 8),
            SniperLaser = (1 << 9),
            FiberCamera = (1 << 10)
        };

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

        [[nodiscard]] virtual Flag getFlags() const = 0;

        [[nodiscard]] virtual std::array<float, 3> getVecProperty(VecProperty property) const = 0;
        [[nodiscard]] virtual StringOffset getStrProperty(StrProperty property) const = 0;
        [[nodiscard]] virtual float getFlProperty(FlProperty property) const = 0;
        [[nodiscard]] virtual std::int32_t getIntProperty(IntProperty property) const = 0;
        [[nodiscard]] virtual std::int16_t getShortProperty(ShortProperty property) const = 0;
        [[nodiscard]] virtual std::uint16_t getUShortProperty(UShortProperty property) const = 0;
        [[nodiscard]] virtual std::byte getByteProperty(ByteProperty property) const = 0;
        [[nodiscard]] virtual nstd::observer_ptr<IEdict> getEdictProperty(EdictProperty property) const = 0;

        [[nodiscard]] virtual FixAngle getFixAngle() const = 0;
        [[nodiscard]] virtual PrecacheId getModelIndex() const = 0;
        [[nodiscard]] virtual MoveType getMoveType() const = 0;
        [[nodiscard]] virtual SolidType getSolidType() const = 0;

        [[nodiscard]] virtual Effects getEffects() const = 0;
        [[nodiscard]] virtual std::array<std::byte, 4> getController() const = 0;
        [[nodiscard]] virtual std::array<std::byte, 2> getBlending() const = 0;

        [[nodiscard]] virtual RenderMode getRenderMode() const = 0;
        [[nodiscard]] virtual DeadFlag getDeadFlag() const = 0;
        [[nodiscard]] virtual SpawnFlag getSpawnFlag() const = 0;

        virtual void setFlags(Flag flags) = 0;

        virtual void setVecProperty(VecProperty property, std::array<float, 3> value) = 0;
        virtual void setStrProperty(StrProperty property, StringOffset value) = 0;
        virtual void setFlProperty(FlProperty property, float value) = 0;
        virtual void setIntProperty(IntProperty property, std::int32_t value) = 0;
        virtual void setShortProperty(ShortProperty property, std::int16_t value) = 0;
        virtual void setUShortProperty(UShortProperty property, std::uint16_t value) = 0;
        virtual void setByteProperty(ByteProperty property, std::byte value) = 0;
        virtual void setEdictProperty(EdictProperty property, nstd::observer_ptr<IEdict> value) = 0;

        virtual void setFixAngle(FixAngle fixAngle) = 0;
        virtual void setModelIndex(PrecacheId modelIndex) = 0;
        virtual void setMoveType(MoveType moveType) = 0;
        virtual void setSolidType(SolidType solidType) = 0;

        virtual void setEffects(Effects effects) = 0;
        virtual void setController(std::array<std::byte, 4> controller) = 0;
        virtual void setBlending(std::array<std::byte, 2> blending) = 0;

        virtual void setRenderMode(RenderMode renderMode) = 0;
        virtual void setDeadFlag(DeadFlag deadFlag) = 0;
        virtual void setSpawnFlag(SpawnFlag spawnFlag) = 0;

        [[nodiscard]] virtual void *getPrivateData() const = 0;
        virtual explicit operator edict_t *() const = 0;
        virtual explicit operator entvars_t *() const = 0;
    };
} // namespace Anubis::Engine