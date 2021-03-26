/*
 *  Copyright (C) 2020-2021 MetamodCPP Development Team
 *
 *  This file is part of MetamodCPP.
 *
 *  MetamodCPP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  MetamodCPP is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with MetamodCPP.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <osconfig.h>
#include <extdll.h>
#include <usercmd.h>
#include "ReHooks.hpp"
#include <Metamod.hpp>
#include "GameClient.hpp"
#include "Cvar.hpp"

namespace Metamod::Engine::ReHooks
{
    void SV_DropClientHook(IRehldsHook_SV_DropClient *chain, ::IGameClient *client, bool crash, const char *string)
    {
        static SVDropClientHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->svDropClient();
        GameClient gameClient(client);

        hookchain->callChain([chain](IGameClient *client, bool crash, std::string_view string) {
            chain->callNext(*dynamic_cast<GameClient *>(client), crash, string.data());
        }, [chain](IGameClient *client, bool crash, std::string_view string) {
            chain->callOriginal(*dynamic_cast<GameClient *>(client), crash, string.data());
        }, &gameClient, crash, string);
    }

    void Cvar_DirectSetHook(IRehldsHook_Cvar_DirectSet *chain, cvar_t *cvar, const char *value)
    {
        static Engine *engine = gMetaGlobal->getEngine();
        static CvarDirectSetHookRegistry *hookchain = engine->getHooks()->cvarDirectSet();

        auto metaCvar = engine->registerCvar(cvar);

        hookchain->callChain([chain](ICvar *cvar, std::string_view value) {
            chain->callNext(*dynamic_cast<Cvar *>(cvar), value.data());
        }, [chain](ICvar *cvar, std::string_view value) {
            chain->callOriginal(*dynamic_cast<Cvar *>(cvar), value.data());
        }, metaCvar, value);
    }
}
