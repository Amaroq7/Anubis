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

#include "EntityHolder.hpp"
#include "BasePlayer.hpp"

Metamod::Game::EntityHolder gEntityHolder;

namespace Metamod::Game
{
    IBaseEntity *EntityHolder::getBaseEntity(Engine::IEdict *edict)
    {
        return _getEntity<CStrike::BaseEntity>(edict);
    }

    IBasePlayer *EntityHolder::getBasePlayer(Engine::IEdict *edict)
    {
        return _getEntity<CStrike::BasePlayer>(edict);
    }
}