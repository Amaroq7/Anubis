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

#include <cinttypes>
#include <utility>

namespace Anubis::Engine
{
    class IHudTextParams
    {
    public:
        virtual ~IHudTextParams() = default;
        enum class Prop : std::uint8_t
        {
            fadeInTime = 0,
            fadeOutTime,
            holdTime,
            fxTime
        };

        virtual void setPosX(float x) = 0;
        virtual void setPosY(float y) = 0;
        virtual void setEffect(int effect) = 0;
        virtual void setColor(std::byte r, std::byte g, std::byte b, std::byte a) = 0;
        virtual void setColorEffect(std::byte r, std::byte g, std::byte b, std::byte a) = 0;
        virtual void setProp(Prop property, float value) = 0;
        virtual void setChannel(std::uint32_t channel) = 0;
    };
} // namespace Anubis::Engine