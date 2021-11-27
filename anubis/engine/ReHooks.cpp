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

#include "Anubis.hpp"
#include "Library.hpp"
#include "ReHooks.hpp"
#include "GameClient.hpp"
#include "Cvar.hpp"
#include "Hooks.hpp"

namespace Anubis::Engine::ReHooks
{
    void SV_DropClientHook(IRehldsHook_SV_DropClient *chain, ::IGameClient *client, bool crash, const char *string)
    {
        static auto hookChain = gAnubisApi->getEngine()->getHooks()->svDropClient();

        nstd::observer_ptr<Engine::IGameClient> gameClient = gAnubisApi->getEngine()->getGameClient(client);

        hookChain->callChain(
            [chain](nstd::observer_ptr<IGameClient> client, bool crash, std::string_view string)
            {
                chain->callNext(static_cast<::IGameClient *>(*client), crash, string.data());
            },
            [chain](nstd::observer_ptr<IGameClient> client, bool crash, std::string_view string)
            {
                chain->callOriginal(static_cast<::IGameClient *>(*client), crash, string.data());
            },
            gameClient, crash, string);
    }

    void Cvar_DirectSetHook(IRehldsHook_Cvar_DirectSet *chain, cvar_t *cvar, const char *value)
    {
        static auto hookChain = gAnubisApi->getEngine()->getHooks()->cvarDirectSetRe();

        nstd::observer_ptr<ICvar> cachedCvar = gAnubisApi->getEngine()->addToCache(cvar);

        hookChain->callChain(
            [chain](nstd::observer_ptr<ICvar> cvar, std::string_view value)
            {
                chain->callNext(static_cast<cvar_t *>(*cvar), value.data());
            },
            [chain](nstd::observer_ptr<ICvar> cvar, std::string_view value)
            {
                chain->callOriginal(static_cast<cvar_t *>(*cvar), value.data());
            },
            cachedCvar, value);
    }

    edict_t *ED_Alloc(IRehldsHook_ED_Alloc *chain)
    {
        static auto engineLib = gAnubisApi->getEngine();
        static auto hookChain = engineLib->getHooks()->edAlloc();

        return static_cast<edict_t *>(*hookChain->callChain(
            [chain]()
            {
                edict_t *edict = chain->callNext();
                engineLib->initEdict(edict);
                return engineLib->getEdict(edict);
            },
            [chain]()
            {
                edict_t *edict = chain->callOriginal();
                engineLib->initEdict(edict);
                return engineLib->getEdict(edict);
            }));
    }
} // namespace Anubis::Engine::ReHooks
