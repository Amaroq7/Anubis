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

#include <engine/IFuncs.hpp>

namespace Metamod::Engine
{
    class Funcs : public Engine::IFuncs
    {
        public:
            Funcs(bool direct = false);
            std::uint32_t precacheModel(std::string_view model) const override;
            std::uint32_t precacheSound(std::string_view sound) const override;
            void changeLevel(std::string_view level1, std::string_view level2) const override;
            void serverCommand(std::string_view cmd) const override;
            void serverExecute() const override;
            void registerSrvCommand(std::string_view cmd, Engine::ServerCmdCallback cb) const override;
            void messageBegin(Engine::MsgDest msgDest, std::uint32_t msgType, const float *pOrigin, Engine::IEdict *pEdict) const override;
            void messageEnd() const override;
            void writeByte(std::byte byteArg) const override;
            void writeChar(char charArg) const override;
            void writeShort(std::int16_t shortArg) const override;
            void writeLong(std::int32_t longArg) const override;
            void writeEntity(std::int16_t entArg) const override;
            void writeAngle(float angleArg) const override;
            void writeCoord(float coordArg) const override;
            void writeString(std::string_view strArg) const override;
            std::uint8_t regUserMsg(std::string_view name, std::uint8_t size) const override;

        private:
            bool m_direct = false;
    };
}