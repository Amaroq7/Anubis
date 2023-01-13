/*
 *  Copyright (C) 2020-2021 Development Team
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

#include <observer_ptr.hpp>
#include <memory>

typedef int qboolean;
typedef struct edict_s edict_t;

namespace Anubis
{
    namespace Game
    {
        class Library;
    }
    namespace Engine
    {
        class ILibrary;
    }
} // namespace Anubis

namespace Anubis::Game::Callbacks::Engine
{
    nstd::observer_ptr<Library> getGame(nstd::observer_ptr<Library> game = {});
    nstd::observer_ptr<::Anubis::Engine::ILibrary> getEngine(nstd::observer_ptr<::Anubis::Engine::ILibrary> eng = {});
    void pfnGameInit();
    int pfnSpawn(edict_t *pent);
    qboolean pfnClientConnect(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]);
    void pfnClientPutInServer(edict_t *pEntity);
    void pfnClientCommand(edict_t *pEntity);
    void pfnClientUserInfoChanged(edict_t *pEntity, char *infobuffer);
    void pfnServerActivate(edict_t *pEdictList, int edictCount, int clientMax);
    void pfnServerDeactivate();
    void pfnStartFrame();
    void pfnGameShutdown();
    void pfnCvarValue(const edict_t *pEnt, const char *value);
    void pfnCvarValue2(const edict_t *pEnt, int requestID, const char *cvarName, const char *value);
    void pfnClientDisconnect (edict_t *pEntity);
} // namespace Anubis::Game::Callbacks::Engine