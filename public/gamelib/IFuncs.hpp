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

#include <functional>
#include <string_view>
#include <any>

namespace Metamod::Engine
{
    class IEdict;
}

namespace Metamod::GameLib
{
    class IFuncs
    {
    public:
        virtual ~IFuncs() = default;

        virtual void pfnGameInit() = 0;
        virtual bool pfnClientConnect(Engine::IEdict *pEntity, std::string_view pszName, std::string_view pszAddress, std::string &szRejectReason) = 0;
        virtual void pfnClientPutInServer(Engine::IEdict *pEntity) = 0;
        virtual void pfnClientCommand(Engine::IEdict *pEntity) = 0;
        virtual void pfnClientUserInfoChanged(Engine::IEdict *pEntity, char *infobuffer) = 0;
        virtual void pfnServerActivate(std::uint32_t edictCount, std::uint32_t clientMax) = 0;
        virtual void pfnServerDeactivate() = 0;
        virtual void pfnStartFrame() = 0;
        virtual void pfnGameShutdown() = 0;
    };
}