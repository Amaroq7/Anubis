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

#include <string_view>

#include "Common.hpp"
#include "IPlugin.hpp"
#include "engine/IEngine.hpp"
#include "engine/IHooks.hpp"
#include "HookChains.hpp"
#include "gamelib/IGameLib.hpp"
#include "entities/IBasePlayer.hpp"

namespace Metamod
{
    enum class GameType : std::uint8_t
    {
        Valve = 0,
        CStrike,
        CZero,
    };

    class IMetamod
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;
        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

    public:
        struct RegMsg
        {
            std::string_view name;
            std::int16_t size;
            std::uint8_t id;
        };

        virtual ~IMetamod() = default;

        virtual std::uint32_t getInterfaceVersion() const = 0;
        virtual Engine::IEngine *getEngine() const = 0;
        virtual GameLib::IGameLib *getGameLib() const = 0;
        virtual GameType getGameType() const = 0;
        virtual const RegMsg *getMsgInfo(std::string_view name) const = 0;
    };

    C_DLLEXPORT IPlugin *MetaQuery();
    using fnMetaQuery = IPlugin *(*)();

    C_DLLEXPORT bool MetaInit();
    using fnMetaInit = bool *(*)(IMetamod *api);
}