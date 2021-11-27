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
#include "BasePlayer.hpp"
#include "AnubisExports.hpp"

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

    nstd::observer_ptr<IBaseEntity> EntityHolder::getBaseEntity(nstd::observer_ptr<Engine::IEdict> edict)
    {
        return _getEntity(edict);
    }

    std::unique_ptr<IBasePlayer> EntityHolder::getBasePlayer(nstd::observer_ptr<Engine::IEdict> edict)
    {
        if (_getEntity(edict))
        {
            return std::make_unique<BasePlayer>(edict);
        }

        return {};
    }

    nstd::observer_ptr<IBaseEntity> EntityHolder::allocEntity(nstd::observer_ptr<Engine::IEdict> edict)
    {
        const auto &[it, inserted] = m_entities.try_emplace(edict, std::make_unique<BaseEntity>(edict));

        if (!inserted)
        {
            it->second->updateSerialNumber(edict->getSerialNumber());
        }

        return it->second;
    }

    nstd::observer_ptr<IBaseEntity> EntityHolder::getBaseEntity(CBaseEntity *baseEntity)
    {
        return getBaseEntity(gEngineLib->getEdict(baseEntity->pev));
    }

    std::unique_ptr<IBasePlayer> EntityHolder::getBasePlayer(CBasePlayer *basePlayer)
    {
        return getBasePlayer(gEngineLib->getEdict(basePlayer->pev));
    }

    nstd::observer_ptr<IBaseEntity> EntityHolder::getBaseEntity(entvars_t *entVars)
    {
        return getBaseEntity(gEngineLib->getEdict(entVars));
    }
} // namespace Anubis::Game::Valve
