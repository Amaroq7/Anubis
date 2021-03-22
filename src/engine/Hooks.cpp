/*
 *  Copyright (C) 2020 Metamod++ Development Team
 *
 *  This file is part of Metamod++.
 *
 *  Metamod++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Metamod++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Metamod++.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <engine/Hooks.hpp>

namespace Metamod::Engine
{
    ChangeLevelHookRegistry *Hooks::changeLevel()
    {
        return &m_changeLevelRegistry;
    }

    MessageBeginHookRegistry *Hooks::messageBegin()
    {
        return &m_messageBeginRegistry;
    }

    MessageEndHookRegistry *Hooks::messageEnd()
    {
        return &m_messageEndRegistry;
    }

    WriteByteHookRegistry *Hooks::writeByte()
    {
        return &m_writeByteRegistry;
    }

    WriteCharHookRegistry *Hooks::writeChar()
    {
        return &m_writeCharRegistry;
    }

    WriteShortHookRegistry *Hooks::writeShort()
    {
        return &m_writeShortRegistry;
    }

    WriteLongHookRegistry *Hooks::writeLong()
    {
        return &m_writeLongRegistry;
    }

    WriteEntityHookRegistry *Hooks::writeEntity()
    {
        return &m_writeEntityRegistry;
    }

    WriteAngleHookRegistry *Hooks::writeAngle()
    {
        return &m_writeAngleRegistry;
    }

    WriteCoordHookRegistry *Hooks::writeCoord()
    {
        return &m_writeCoordRegistry;
    }

    WriteStringHookRegistry *Hooks::writeString()
    {
        return &m_writeStringRegistry;
    }

    RegUserMsgHookRegistry *Hooks::regUserMsg()
    {
        return &m_regUserMsgRegistry;
    }
}