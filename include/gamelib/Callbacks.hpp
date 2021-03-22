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

#include <extdll.h>
#include <engine/Edict.hpp>

#include <string>

namespace Metamod::GameLib::Callbacks::Direct
{
    void pfnGameInit();
    bool pfnClientConnect(Engine::IEdict *pEntity, std::string_view pszName, std::string_view pszAddress, std::string &szRejectReason);
    void pfnClientPutInServer(Engine::IEdict *pEntity);
    void pfnClientCommand(Engine::IEdict *pEntity);
    void pfnClientUserInfoChanged(Engine::IEdict *pEntity, char *infobuffer);
    void pfnServerActivate(std::uint32_t edictCount, std::uint32_t clientMax);
    void pfnServerDeactivate();
    void pfnStartFrame();
    void pfnGameShutdown();
}

namespace Metamod::GameLib::Callbacks::Engine
{
    void pfnGameInit();
    qboolean pfnClientConnect(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]);
    void pfnClientPutInServer(edict_t *pEntity);
    void pfnClientCommand(edict_t *pEntity);
    void pfnClientUserInfoChanged(edict_t *pEntity, char *infobuffer);
    void pfnServerActivate(edict_t *pEdictList, int edictCount, int clientMax);
    void pfnServerDeactivate();
    void pfnStartFrame();
    void pfnGameShutdown();
}