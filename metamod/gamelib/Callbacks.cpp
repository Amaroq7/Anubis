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

#include <Metamod.hpp>
#include "Callbacks.hpp"
#include <string>

namespace Metamod::GameLib::Callbacks::Engine
{
    static GameLib *gameLib;
    static ::Metamod::Engine::Engine *engine;

    void pfnGameInit()
    {
        engine = gMetaGlobal->getEngine();
        gameLib = gMetaGlobal->getGameLib();

        gameLib->pfnGameInit();
    }
    qboolean pfnClientConnect(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128])
    {
        constexpr const std::size_t REASON_REJECT_MAX_LEN = 128;

        static std::string rejectReason;
        if (rejectReason.capacity() < REASON_REJECT_MAX_LEN)
        {
            rejectReason.reserve(REASON_REJECT_MAX_LEN);
        }

        if (!gameLib->pfnClientConnect(engine->getEdict(pEntity), pszName, pszAddress, rejectReason, FuncCallType::Hooks))
        {
            strncpy(szRejectReason, rejectReason.data(), REASON_REJECT_MAX_LEN - 1);
            return FALSE;
        }
        return TRUE;
    }
    void pfnClientPutInServer(edict_t *pEntity)
    {
        gameLib->pfnClientPutInServer(engine->getEdict(pEntity), FuncCallType::Hooks);
    }
    void pfnClientCommand(edict_t *pEntity)
    {
        gameLib->pfnClientCommand(engine->getEdict(pEntity), FuncCallType::Hooks);
    }
    void pfnClientUserInfoChanged(edict_t *pEntity, char *infobuffer)
    {
        gameLib->pfnClientUserInfoChanged(engine->getEdict(pEntity), infobuffer, FuncCallType::Hooks);
    }
    void pfnServerActivate(edict_t *pEdictList, int edictCount, int clientMax)
    {
        engine->initializeEdicts(pEdictList, static_cast<std::uint32_t>(edictCount), static_cast<std::uint32_t>(clientMax));
        gameLib->setMaxClients(clientMax);
        gameLib->pfnServerActivate(static_cast<std::uint32_t>(edictCount), static_cast<std::uint32_t>(clientMax), FuncCallType::Hooks);
    }
    void pfnServerDeactivate()
    {
        gameLib->pfnServerDeactivate(FuncCallType::Hooks);
    }
    void pfnStartFrame()
    {
        gameLib->pfnStartFrame(FuncCallType::Hooks);
    }
    void pfnGameShutdown()
    {
        gameLib->pfnGameShutdown(FuncCallType::Hooks);
        gMetaGlobal.reset();
    }
}
