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

#include "EntityHolder.hpp"

#include <extdll.h>
#include <osconfig.h>
#include <util.h>
#include <cbase.h>
#include <player.h>

namespace Anubis::Game::Valve
{
    const std::unique_ptr<EntityHolder> &getEntityHolder()
    {
        static auto entHolder = std::make_unique<Game::Valve::EntityHolder>();
        return entHolder;
    }

    std::unique_ptr<IBaseEntity> EntityHolder::getBaseEntity(nstd::observer_ptr<Engine::IEdict> edict)
    {
        return _getEntity<IBaseEntity, BaseEntity>(edict);
    }

    std::unique_ptr<IBasePlayer> EntityHolder::getBasePlayer(nstd::observer_ptr<Engine::IEdict> edict)
    {
        return _getEntity<IBasePlayer, BasePlayer>(edict);
    }

    std::unique_ptr<IBasePlayerItem> EntityHolder::getBasePlayerItem(nstd::observer_ptr<Engine::IEdict> edict)
    {
        return _getEntity<IBasePlayerItem, BasePlayerItem>(edict);
    }

    std::unique_ptr<IBasePlayerWeapon> EntityHolder::getBasePlayerWeapon(nstd::observer_ptr<Engine::IEdict> edict)
    {
        return _getEntity<IBasePlayerWeapon, BasePlayerWeapon>(edict);
    }

    std::unique_ptr<IBasePlayerAmmo> EntityHolder::getBasePlayerAmmo(nstd::observer_ptr<Engine::IEdict> edict)
    {
        return _getEntity<IBasePlayerAmmo, BasePlayerAmmo>(edict);
    }

    std::unique_ptr<IBaseEntity> EntityHolder::getBaseEntity(CBaseEntity *baseEntity)
    {
        return getBaseEntity(gEngineLib->getEdict(baseEntity->pev));
    }

    std::unique_ptr<IBasePlayer> EntityHolder::getBasePlayer(CBasePlayer *basePlayer)
    {
        return getBasePlayer(gEngineLib->getEdict(basePlayer->pev));
    }

    std::unique_ptr<IBaseEntity> EntityHolder::getBaseEntity(entvars_t *entVars)
    {
        return getBaseEntity(gEngineLib->getEdict(entVars));
    }

    std::unique_ptr<IBaseEntity> EntityHolder::getBaseEntity(edict_t *edict)
    {
        return getBaseEntity(gEngineLib->getEdict(edict));
    }
} // namespace Anubis::Game::Valve
