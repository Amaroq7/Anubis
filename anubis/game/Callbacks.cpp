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

#include "Callbacks.hpp"

#include <engine/ILibrary.hpp>
#include <Anubis.hpp>

#include <extdll.h>
#include "Library.hpp"

#include <cstring>
#include <utility>

#if !defined FALSE
    #define FALSE 0
    #define TRUE  (!0)
#endif

namespace Anubis::Game::Callbacks::Engine
{
    nstd::observer_ptr<Library> getGame(nstd::observer_ptr<Library> game)
    {
        static nstd::observer_ptr<Library> gameInstance = game;
        return gameInstance;
    }

    nstd::observer_ptr<::Anubis::Engine::ILibrary> getEngine(nstd::observer_ptr<::Anubis::Engine::ILibrary> eng)
    {
        static nstd::observer_ptr<::Anubis::Engine::ILibrary> engineInstance = eng;
        return engineInstance;
    }

    void pfnGameInit()
    {
        getGame()->pfnGameInit(FuncCallType::Hooks);
    }

    int pfnSpawn(edict_t *pent)
    {
        return getGame()->pfnSpawn(getEngine()->getEdict(pent), FuncCallType::Hooks);
    }

    qboolean pfnClientConnect(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128])
    {
        constexpr const std::size_t REASON_REJECT_MAX_LEN = 128;

        static std::string rejectReason;
        if (rejectReason.capacity() < REASON_REJECT_MAX_LEN)
        {
            rejectReason.reserve(REASON_REJECT_MAX_LEN);
        }
        rejectReason.clear();

        if (!getGame()->pfnClientConnect(getEngine()->getEdict(pEntity), pszName, pszAddress, rejectReason,
                                         FuncCallType::Hooks))
        {
#if defined __linux__
            strncpy(szRejectReason, rejectReason.data(), REASON_REJECT_MAX_LEN - 1);
#elif defined _WIN32
            strncpy_s(szRejectReason, 128, rejectReason.data(), _TRUNCATE);
#endif
            return FALSE;
        }
        return TRUE;
    }

    void pfnClientPutInServer(edict_t *pEntity)
    {
        getGame()->pfnClientPutInServer(getEngine()->getEdict(pEntity), FuncCallType::Hooks);
    }

    void pfnClientCommand(edict_t *pEntity)
    {
        getGame()->pfnClientCommand(getEngine()->getEdict(pEntity), FuncCallType::Hooks);
    }

    void pfnClientUserInfoChanged(edict_t *pEntity, char *infobuffer)
    {
        getGame()->pfnClientUserInfoChanged(getEngine()->getEdict(pEntity), ::Anubis::Engine::InfoBuffer(infobuffer),
                                            FuncCallType::Hooks);
    }

    void pfnServerActivate(edict_t *pEdictList, int edictCount, int clientMax)
    {
        getGame()->setMaxClients(clientMax);
        getGame()->setEdictList(pEdictList);
        getGame()->pfnServerActivate(static_cast<std::uint32_t>(edictCount), static_cast<std::uint32_t>(clientMax),
                                     FuncCallType::Hooks);
    }

    void pfnServerDeactivate()
    {
        getGame()->pfnServerDeactivate(FuncCallType::Hooks);
    }

    void pfnStartFrame()
    {
        getGame()->pfnStartFrame(FuncCallType::Hooks);
    }

    void pfnGameShutdown()
    {
        getGame()->pfnGameShutdown(FuncCallType::Hooks);
        gAnubisApi->freePluginsResources();
        getEngine()->removeExtDll(getGame()->getSystemHandle());
        getEngine()->removeHooks();
        getGame()->freeEntitiesDLL();
    }

    void pfnCvarValue(const edict_t *pEnt, const char *value)
    {
        getGame()->pfnCvarValue(getEngine()->getEdict(pEnt), value, FuncCallType::Hooks);
    }

    void pfnCvarValue2(const edict_t *pEnt, int requestID, const char *cvarName, const char *value)
    {
        getGame()->pfnCvarValue2(getEngine()->getEdict(pEnt), static_cast<std::uint32_t>(requestID), cvarName, value,
                                 FuncCallType::Hooks);
    }
} // namespace Anubis::Game::Callbacks::Engine
