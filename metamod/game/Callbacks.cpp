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

#include "Callbacks.hpp"

#include <engine/ILibrary.hpp>
#include "../Metamod.hpp"

#include <extdll.h>
#include "Library.hpp"

#include <cstring>

#if !defined FALSE
    #define FALSE 0
    #define TRUE (!0)
#endif

namespace Metamod::Game::Callbacks::Engine
{
    static Library *gGame;
    static ::Metamod::Engine::ILibrary *gEngine;

    void init(Library *game, ::Metamod::Engine::ILibrary *engine)
    {
        gGame = game;
        gEngine = engine;
    }

    void pfnGameInit()
    {
        gGame->pfnGameInit(FuncCallType::Hooks);
    }

    int pfnSpawn(edict_t *pent)
    {
        static bool vFuncInitialized = false;
        if (!vFuncInitialized)
        {
            gGame->initVFuncHooks();
            vFuncInitialized = true;
        }

        return gGame->pfnSpawn(gEngine->getEdict(pent), FuncCallType::Hooks);
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

        if (!gGame->pfnClientConnect(gEngine->getEdict(pEntity), pszName, pszAddress, rejectReason, FuncCallType::Hooks))
        {
            strncpy(szRejectReason, rejectReason.data(), REASON_REJECT_MAX_LEN - 1);
            return FALSE;
        }
        return TRUE;
    }

    void pfnClientPutInServer(edict_t *pEntity)
    {
        gGame->pfnClientPutInServer(gEngine->getEdict(pEntity), FuncCallType::Hooks);
    }

    void pfnClientCommand(edict_t *pEntity)
    {
        gGame->pfnClientCommand(gEngine->getEdict(pEntity), FuncCallType::Hooks);
    }

    void pfnClientUserInfoChanged(edict_t *pEntity, char *infobuffer)
    {
        gGame->pfnClientUserInfoChanged(gEngine->getEdict(pEntity), infobuffer, FuncCallType::Hooks);
    }

    void pfnServerActivate(edict_t *pEdictList, int edictCount, int clientMax)
    {
        gEngine->initializeEdicts(pEdictList, static_cast<std::uint32_t>(edictCount), static_cast<std::uint32_t>(clientMax));
        gGame->setMaxClients(clientMax);
        gGame->initVFuncHooks();
        gGame->pfnServerActivate(static_cast<std::uint32_t>(edictCount), static_cast<std::uint32_t>(clientMax), FuncCallType::Hooks);
    }

    void pfnServerDeactivate()
    {
        gGame->pfnServerDeactivate(FuncCallType::Hooks);
    }

    void pfnStartFrame()
    {
        gGame->pfnStartFrame(FuncCallType::Hooks);
    }

    void pfnGameShutdown()
    {
        gGame->pfnGameShutdown(FuncCallType::Hooks);
        gMetaGlobal->freePluginsResources();
        gEngine->removeExtDll(gGame->getSystemHandle());
    }
}
