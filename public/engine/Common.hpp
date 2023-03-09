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

#include "../Common.hpp"
#include <functional>

namespace Anubis::Engine
{
    enum class MsgDest : std::uint8_t
    {
        Broadcast = 0,     /**< Unreliable to all */
        One = 1,           /**< Reliable to one (msg_entity) */
        All = 2,           /**< Reliable to all */
        Init = 3,          /**< Write to the init string */
        Pvs = 4,           /**< Ents in PVS of org */
        Pas = 5,           /**< Ents in PAS of org */
        PvsR = 6,          /**< Reliable to PVS */
        PasR = 7,          /**< Reliable to PAS */
        OneUnreliable = 8, /**< Send to one client, but don't put in reliable stream, put in unreliable datagram */
        Spec = 9,          /**< Sends to all spectator proxies */
    };

    /**
     *  Hit group.
     */
    enum class HitGroup : std::uint8_t
    {
        Generic = 0,  /**< Generic */
        Head = 1,     /**< Head */
        Chest = 2,    /**< Chest */
        Stomach = 3,  /**< Stomach */
        LeftArm = 4,  /**< Left arm */
        RightArm = 5, /**< Right arm */
        LeftLeg = 6,  /**< Left leg */
        RightLeg = 7  /**< Right leg */
    };

    enum class AlertType : std::uint8_t
    {
        Notice = 0,
        Console,   // same as at_notice, but forces a ConPrintf, not a message box
        Aiconsole, // same as at_console, but only shown if developer level is 2!
        Warning,
        Error,
        Logged // Server print to console ( only in multiplayer games ).
    };

    enum class PrintType : std::uint8_t
    {
        Console = 0,
        Center,
        Chat
    };

    enum class Channel : std::uint16_t
    {
        Auto = 0,
        Weapon,
        Voice,
        Item,
        Body,
        Stream,
        Static,
        NetworkVoiceBase,
        NetworkVoiceEnd = 500,
        Bot
    };

    enum class Pitch : std::uint8_t
    {
        Norm = 100,
        Low = 95,
        High = 120
    };

    enum class SoundFlags : std::uint16_t
    {
        Volume = (1 << 0),
        Attenuation = (1 << 1),
        Large_index = (1 << 2),
        Pitch = (1 << 3),
        Sentence = (1 << 4),
        Stop = (1 << 5),
        Change_vol = (1 << 6),
        Change_pitch = (1 << 7),
        Spawning = (1 << 8)
    };

    enum class TraceMonsters : std::uint16_t
    {
        Normal = 0,
        No,
        Missle,
        IgnoreGlass = 0x100,
        IgnoreGlassNormal = (Normal | 0x100),
        IgnoreGlassNo = (No | 0x100),
        IgnoreGlassMissle = (Missle | 0x100)
    };

    enum class HullNumber : std::uint8_t
    {
        Point = 0,
        Human,
        Large,
        Head
    };

    /**
     * Server command callback.
     */
    using ServerCmdCallback = std::function<void()>;

    ANUBIS_STRONG_TYPEDEF(std::int16_t, MsgEntity)
    ANUBIS_STRONG_TYPEDEF(float, MsgAngle)
    ANUBIS_STRONG_TYPEDEF(float, MsgCoord)
    ANUBIS_STRONG_TYPEDEF(std::uint16_t, PrecacheId)
    ANUBIS_STRONG_TYPEDEF(std::uint8_t, MsgType)
    ANUBIS_STRONG_TYPEDEF_PTR(char, InfoBuffer)
    ANUBIS_STRONG_TYPEDEF(std::int32_t, UserID)
    ANUBIS_STRONG_TYPEDEF(std::int16_t, MsgSize)
    ANUBIS_STRONG_TYPEDEF(std::uint32_t, StringOffset)
    ANUBIS_STRONG_TYPEDEF(std::uint32_t, EntityOffset)
    ANUBIS_STRONG_TYPEDEF(std::uint16_t, ModelIndex)
    ANUBIS_STRONG_TYPEDEF(float, SndAttenuation)
    ANUBIS_STRONG_TYPEDEF(float, SndVolume)

    constexpr inline SndAttenuation toSndAttenution(float attenution)
    {
        return SndAttenuation {attenution};
    }

    constexpr inline SndVolume toSndVolume(float volume)
    {
        return SndVolume {volume};
    }

    namespace Attenuation
    {
        constexpr SndAttenuation None = toSndAttenution(0.f);
        constexpr SndAttenuation Norm = toSndAttenution(0.8f);
        constexpr SndAttenuation Idle = toSndAttenution(2.f);
        constexpr SndAttenuation Static = toSndAttenution(1.25f);
    } // namespace Attenuation

    namespace Volume
    {
        constexpr SndVolume Normal = toSndVolume(1.f);
    } // namespace Volume

} // namespace Anubis::Engine
