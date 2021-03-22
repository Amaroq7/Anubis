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

#include "IEdict.hpp"
#include "Common.hpp"
#include <functional>
#include <string_view>

namespace Metamod::Engine 
{
    class IFuncs
    {
        public:
            virtual ~IFuncs() = default;

            virtual std::uint32_t precacheModel(std::string_view model) const = 0;
            virtual std::uint32_t precacheSound(std::string_view sound) const = 0;
            virtual void changeLevel(std::string_view level1, std::string_view level2 = {}) const = 0;
            virtual void serverCommand(std::string_view cmd) const = 0;
            virtual void serverExecute() const = 0;
            virtual void registerSrvCommand(std::string_view cmd, ServerCmdCallback cb) const = 0;
            virtual void messageBegin(MsgDest msgDest,
                                  std::uint32_t msgType,
                                  const float *pOrigin = nullptr,
                                  IEdict *pEdict = nullptr) const = 0;
            virtual void messageEnd() const = 0;
            virtual void writeByte(std::byte byteArg) const = 0;
            virtual void writeChar(char charArg) const = 0;
            virtual void writeShort(std::int16_t shortArg) const = 0;
            virtual void writeLong(std::int32_t longArg) const = 0;
            virtual void writeEntity(std::int16_t entArg) const = 0;
            virtual void writeAngle(float angleArg) const = 0;
            virtual void writeCoord(float coordArg) const = 0;
            virtual void writeString(std::string_view strArg) const = 0;
            virtual std::uint8_t regUserMsg(std::string_view name, std::uint8_t size) const = 0;
    };
}