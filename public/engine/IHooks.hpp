/*
 *  Copyright (C) 2020-2021 Metamod++ Development Team
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

#include "Common.hpp"
#include "IHookChains.hpp"

#include <string_view>

namespace Metamod::Engine
{
    class IEdict;

    using IChangeLevelHook = IHook<void, std::string_view, std::string_view>;
    using IChangeLevelHookRegistry = IHookRegistry<void, std::string_view, std::string_view>;

    using IMessageBeginHook = IHook<void, MsgDest, std::uint32_t, const float *, IEdict *>;
    using IMessageBeginHookRegistry = IHookRegistry<void, MsgDest, std::uint32_t, const float *, IEdict *>;

    using IMessageEndHook = IHook<void>;
    using IMessageEndHookRegistry = IHookRegistry<void>;

    using IWriteByteHook = IHook<void, std::byte>;
    using IWriteByteHookRegistry = IHookRegistry<void, std::byte>;

    using IWriteCharHook = IHook<void, char>;
    using IWriteCharHookRegistry = IHookRegistry<void, char>;

    using IWriteShortHook = IHook<void, std::int16_t>;
    using IWriteShortHookRegistry = IHookRegistry<void, std::int16_t>;

    using IWriteLongHook = IHook<void, std::int32_t>;
    using IWriteLongHookRegistry = IHookRegistry<void, std::int32_t>;

    using IWriteEntityHook = IHook<void, std::int16_t>;
    using IWriteEntityHookRegistry = IHookRegistry<void, std::int16_t>;

    using IWriteAngleHook = IHook<void, float>;
    using IWriteAngleHookRegistry = IHookRegistry<void, float>;

    using IWriteCoordHook = IHook<void, float>;
    using IWriteCoordHookRegistry = IHookRegistry<void, float>;

    using IWriteStringHook = IHook<void, std::string_view>;
    using IWriteStringHookRegistry = IHookRegistry<void, std::string_view>;

    using IRegUserMsgHook = IHook<std::uint8_t, std::string_view, std::uint8_t>;
    using IRegUserMsgHookRegistry = IHookRegistry<std::uint8_t, std::string_view, std::uint8_t>;

    class IHooks
    {
    public:
        virtual ~IHooks() = default;

        virtual IChangeLevelHookRegistry *changeLevel() = 0;
        virtual IMessageBeginHookRegistry *messageBegin() = 0;
        virtual IMessageEndHookRegistry *messageEnd() = 0;
        virtual IWriteByteHookRegistry *writeByte() = 0;
        virtual IWriteCharHookRegistry *writeChar() = 0;
        virtual IWriteShortHookRegistry *writeShort() = 0;
        virtual IWriteLongHookRegistry *writeLong() = 0;
        virtual IWriteEntityHookRegistry *writeEntity() = 0;
        virtual IWriteAngleHookRegistry *writeAngle() = 0;
        virtual IWriteCoordHookRegistry *writeCoord() = 0;
        virtual IWriteStringHookRegistry *writeString() = 0;
        virtual IRegUserMsgHookRegistry *regUserMsg() = 0;
    };
}