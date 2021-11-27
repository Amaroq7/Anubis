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

#include "engine/Common.hpp"

#include <string>

namespace Anubis
{
    /**
     * @brief Info about registered message.
     *
     * Contains information about registered message.
     * Returned by getUserMsgInfo().
     *
     */
    class IMsg
    {
    public:
#if !defined DOXYGEN_SKIP
        virtual ~IMsg() = default;
#endif

        /**
         * @brief Retrieves User Message name.
         *
         *
         * @return Null terminated name.
         */
        [[nodiscard]] virtual std::string_view getName() const = 0;

        /**
         * @brief Retrieves User Message size.
         *
         * @return Size of the message
         */
        [[nodiscard]] virtual Engine::MsgSize getSize() const = 0;

        /**
         * @brief Retrieves User Message id.
         *
         * @return Id of the message.
         */
        [[nodiscard]] virtual Engine::MsgType getType() const = 0;
    };
} // namespace Anubis