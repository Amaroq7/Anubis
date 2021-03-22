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
#include <gamelib/Callbacks.hpp>
#include <gamelib/Funcs.hpp>

namespace
{
    inline edict_t *MetaEdToEngineEd(Metamod::Engine::IEdict *ed)
    {
        return *dynamic_cast<Metamod::Engine::Edict *>(ed);
    }

    inline const edict_t *MetaEdToEngineEd(const Metamod::Engine::IEdict *ed)
    {
        return *dynamic_cast<const Metamod::Engine::Edict *>(ed);
    }
}

namespace Metamod::GameLib::Callbacks::Direct
{
    void pfnGameInit()
    {
        gEntityInterface.pfnGameInit();
    }

    bool pfnClientConnect(::Metamod::Engine::IEdict *pEntity, std::string_view pszName, std::string_view pszAddress, std::string &szRejectReason)
    {
        constexpr const std::size_t REASON_REJECT_MAX_LEN = 128;
        if (szRejectReason.capacity() < REASON_REJECT_MAX_LEN)
        {
            szRejectReason.reserve(REASON_REJECT_MAX_LEN);
        }
        return static_cast<bool>(gEntityInterface.pfnClientConnect(MetaEdToEngineEd(pEntity), pszName.data(), pszAddress.data(), szRejectReason.data()));
    }

    void pfnClientPutInServer(::Metamod::Engine::IEdict *pEntity)
    {
        gEntityInterface.pfnClientPutInServer(MetaEdToEngineEd(pEntity));
    }

    void pfnClientCommand(::Metamod::Engine::IEdict *pEntity)
    {
        gEntityInterface.pfnClientCommand(MetaEdToEngineEd(pEntity));
    }

    void pfnClientUserInfoChanged(::Metamod::Engine::IEdict *pEntity, char *infobuffer)
    {
        gEntityInterface.pfnClientUserInfoChanged(MetaEdToEngineEd(pEntity), infobuffer);
    }

    void pfnServerActivate(std::uint32_t edictCount [[maybe_unused]], std::uint32_t clientMax [[maybe_unused]])
    {
        gEntityInterface.pfnServerActivate(gMetaGlobal->getEngine()->getEngineEdictList(), edictCount, clientMax);
    }

    void pfnServerDeactivate()
    {
        gEntityInterface.pfnServerDeactivate();
    }

    void pfnStartFrame()
    {
        gEntityInterface.pfnStartFrame();
    }

    void pfnGameShutdown()
    {
        gMetaGlobal->getEngine()->getReHLDSFuncs()->removeExtDll(gMetaGlobal->getGameLib()->getSystemHandle());
        if (gNewDLLFunctions.pfnGameShutdown)
        {
            gNewDLLFunctions.pfnGameShutdown();
        }
    }
}

namespace Metamod::GameLib::Callbacks::Engine
{
    static Funcs *dllFuncs = gMetaGlobal->getGameLib()->getFuncs(false);
    static ::Metamod::Engine::Engine *engine = gMetaGlobal->getEngine();
    void pfnGameInit()
    {
        dllFuncs->pfnGameInit();
    }
    qboolean pfnClientConnect(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128])
    {
        constexpr const std::size_t REASON_REJECT_MAX_LEN = 128;

        static std::string rejectReason;
        if (rejectReason.capacity() < REASON_REJECT_MAX_LEN)
        {
            rejectReason.reserve(REASON_REJECT_MAX_LEN);
        }

        if (!dllFuncs->pfnClientConnect(engine->getEdict(pEntity), pszName, pszAddress, rejectReason))
        {
            strncpy(szRejectReason, rejectReason.data(), REASON_REJECT_MAX_LEN - 1);
            return FALSE;
        }
        return TRUE;
    }
    void pfnClientPutInServer(edict_t *pEntity)
    {
        dllFuncs->pfnClientPutInServer(engine->getEdict(pEntity));
    }
    void pfnClientCommand(edict_t *pEntity)
    {
        dllFuncs->pfnClientCommand(engine->getEdict(pEntity));
    }
    void pfnClientUserInfoChanged(edict_t *pEntity, char *infobuffer)
    {
        dllFuncs->pfnClientUserInfoChanged(engine->getEdict(pEntity), infobuffer);
    }
    void pfnServerActivate(edict_t *pEdictList, int edictCount, int clientMax)
    {
        gMetaGlobal->getEngine()->initializeEdicts(pEdictList, static_cast<std::uint32_t>(edictCount), static_cast<std::uint32_t>(clientMax));
        dllFuncs->pfnServerActivate(static_cast<std::uint32_t>(edictCount), static_cast<std::uint32_t>(clientMax));
    }
    void pfnServerDeactivate()
    {
        dllFuncs->pfnServerDeactivate();
    }
    void pfnStartFrame()
    {
        dllFuncs->pfnStartFrame();
    }
    void pfnGameShutdown()
    {
        dllFuncs->pfnGameShutdown();
    }
}