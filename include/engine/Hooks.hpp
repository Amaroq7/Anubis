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

#pragma once

#include <engine/Common.hpp>
#include <engine/IHooks.hpp>
#include "HookChains.hpp"

namespace Metamod::Engine
{
    using ChangeLevelHook = Hook<void, std::string_view, std::string_view>;
    using ChangeLevelHookRegistry = HookRegistry<void, std::string_view, std::string_view>;

    using MessageBeginHook = Hook<void, MsgDest, std::uint32_t, const float *, IEdict *>;
    using MessageBeginHookRegistry = HookRegistry<void, MsgDest, std::uint32_t, const float *, IEdict *>;

    using MessageEndHook = Hook<void>;
    using MessageEndHookRegistry = HookRegistry<void>;

    using WriteByteHook = Hook<void, std::byte>;
    using WriteByteHookRegistry = HookRegistry<void, std::byte>;

    using WriteCharHook = Hook<void, char>;
    using WriteCharHookRegistry = HookRegistry<void, char>;

    using WriteShortHook = Hook<void, std::int16_t>;
    using WriteShortHookRegistry = HookRegistry<void, std::int16_t>;

    using WriteLongHook = Hook<void, std::int32_t>;
    using WriteLongHookRegistry = HookRegistry<void, std::int32_t>;

    using WriteEntityHook = Hook<void, std::int16_t>;
    using WriteEntityHookRegistry = HookRegistry<void, std::int16_t>;

    using WriteAngleHook = Hook<void, float>;
    using WriteAngleHookRegistry = HookRegistry<void, float>;

    using WriteCoordHook = Hook<void, float>;
    using WriteCoordHookRegistry = HookRegistry<void, float>;

    using WriteStringHook = Hook<void, std::string_view>;
    using WriteStringHookRegistry = HookRegistry<void, std::string_view>;

    using RegUserMsgHook = Hook<std::uint8_t, std::string_view, std::uint8_t>;
    using RegUserMsgHookRegistry = HookRegistry<std::uint8_t, std::string_view, std::uint8_t>;

    class Hooks : public IHooks
    {
    public:
        ~Hooks() override = default;

        ChangeLevelHookRegistry *changeLevel() override;
        MessageBeginHookRegistry *messageBegin() override;
        MessageEndHookRegistry *messageEnd() override;
        WriteByteHookRegistry *writeByte() override;
        WriteCharHookRegistry *writeChar() override;
        WriteShortHookRegistry *writeShort() override;
        WriteLongHookRegistry *writeLong() override;
        WriteEntityHookRegistry *writeEntity() override;
        WriteAngleHookRegistry *writeAngle() override;
        WriteCoordHookRegistry *writeCoord() override;
        WriteStringHookRegistry *writeString() override;
        RegUserMsgHookRegistry *regUserMsg() override;

    private:
        ChangeLevelHookRegistry m_changeLevelRegistry;
        MessageBeginHookRegistry m_messageBeginRegistry;
        MessageEndHookRegistry m_messageEndRegistry;
        WriteByteHookRegistry m_writeByteRegistry;
        WriteCharHookRegistry m_writeCharRegistry;
        WriteShortHookRegistry m_writeShortRegistry;
        WriteLongHookRegistry m_writeLongRegistry;
        WriteEntityHookRegistry m_writeEntityRegistry;
        WriteAngleHookRegistry m_writeAngleRegistry;
        WriteCoordHookRegistry m_writeCoordRegistry;
        WriteStringHookRegistry m_writeStringRegistry;
        RegUserMsgHookRegistry m_regUserMsgRegistry;
    };
}