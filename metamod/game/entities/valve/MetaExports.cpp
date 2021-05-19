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

#include "MetaExports.hpp"
#include "EntitiesHooks.hpp"
#include "EntityHolder.hpp"

#include <engine/IEdict.hpp>
#include <game/entities/valve/BasePlayer.hpp>

Metamod::Game::ILibrary *gGameLib;
Metamod::Engine::ILibrary *gEngineLib;
Metamod::IMetamod *gMetamod;
std::unordered_map<std::string, std::uint32_t> gVOffsets;

C_DLLEXPORT void InitGameEntitiesDLL(
    Metamod::IMetamod *api,
    const std::unordered_map<std::string, std::uint32_t> &vOffsets)
{
    if (!gGameLib)
    {
        return;
    }

    gMetamod = api;
    gGameLib = api->getGame();
    gEngineLib = api->getEngine();
    gVOffsets = vOffsets;

    auto getVTable = [](std::string_view className) {
        Metamod::Engine::IEdict *edict = gEngineLib->createEntity(Metamod::FuncCallType::Direct);
        if (!gGameLib->callGameEntity(className, edict->getEntVars()))
        {
            gEngineLib->removeEntity(edict, Metamod::FuncCallType::Direct);
            return 0;
        }

        if (!edict->getPrivateData())
        {
            gEngineLib->removeEntity(edict, Metamod::FuncCallType::Direct);
            return 0;
        }

        auto vTable = *(reinterpret_cast<std::intptr_t *>(edict->getPrivateData()));
        gEngineLib->removeEntity(edict, Metamod::FuncCallType::Direct);

        if (!reinterpret_cast<void *>(vTable))
        {
            return 0;
        }

        return vTable;
    };

    Metamod::Game::Entities::IBasePlayer::VTable = getVTable(Metamod::Game::Entities::Valve::BasePlayer::CLASS_NAME);

    if (Metamod::Game::Entities::IBasePlayer::VTable)
    {
        gBasePlayerHooks = std::make_unique<Metamod::Game::Valve::BasePlayerHooks>(vOffsets);
    }
}

C_DLLEXPORT Metamod::Game::IBasePlayerHooks *GetBasePlayerHooks()
{
    return gBasePlayerHooks.get();
}

C_DLLEXPORT Metamod::Game::Entities::IEntityHolder *GetEntityHolder()
{
    return &gEntityHolder;
}