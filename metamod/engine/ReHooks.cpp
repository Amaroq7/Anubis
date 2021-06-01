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

#include "Library.hpp"
#include "ReHooks.hpp"
#include "GameClient.hpp"
#include "Cvar.hpp"

namespace Metamod::Engine::ReHooks
{
    static Library *gEngine;

    void init(Library *engine)
    {
        gEngine = engine;
    }

    void SV_DropClientHook(IRehldsHook_SV_DropClient *chain, ::IGameClient *client, bool crash, const char *string)
    {
        static SVDropClientHookRegistry *hookchain = gEngine->getHooks()->svDropClient();
        GameClient gameClient(client, gEngine);

        hookchain->callChain([chain](IGameClient *client, bool crash, std::string_view string) {
            chain->callNext(static_cast<::IGameClient *>(*client), crash, string.data());
        }, [chain](IGameClient *client, bool crash, std::string_view string) {
            chain->callOriginal(static_cast<::IGameClient *>(*client), crash, string.data());
        }, &gameClient, crash, string);
    }

    void Cvar_DirectSetHook(IRehldsHook_Cvar_DirectSet *chain, cvar_t *cvar, const char *value)
    {
        static CvarDirectSetHookRegistry *hookchain = gEngine->getHooks()->cvarDirectSet();

        auto metaCvar = gEngine->addToCache(cvar);

        hookchain->callChain([chain](ICvar *cvar, std::string_view value) {
            chain->callNext(static_cast<cvar_t *>(*cvar), value.data());
        }, [chain](ICvar *cvar, std::string_view value) {
            chain->callOriginal(static_cast<cvar_t *>(*cvar), value.data());
        }, metaCvar, value);
    }
}
