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
} // namespace Anubis::Engine
