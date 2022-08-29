/*
 *  Copyright (C) 2021 Anubis Development Team
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

#include <IMsg.hpp>
#include <string>

namespace Anubis
{
    class Msg final : public IMsg
    {
    public:
        Msg(std::string_view name, Engine::MsgType msgType, Engine::MsgSize size = Engine::MsgSize(-1));
        [[nodiscard]] std::string_view getName() const final;
        [[nodiscard]] Engine::MsgSize getSize() const final;
        [[nodiscard]] Engine::MsgType getType() const final;

    private:
        std::string m_name;
        Engine::MsgType m_type;
        Engine::MsgSize m_size;
    };
} // namespace Anubis