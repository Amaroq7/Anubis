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
#include "ReGameHeaders.hpp"

namespace Anubis::Game::CStrike
{
    const std::unique_ptr<EntityHolder> &getEntityHolder()
    {
        static auto entHolder = std::make_unique<Game::CStrike::EntityHolder>();
        return entHolder;
    }

    nstd::observer_ptr<IBaseEntity> EntityHolder::getBaseEntity(nstd::observer_ptr<Engine::IEdict> edict)
    {
        return _getEntity(edict);
    }

    nstd::observer_ptr<IBasePlayer> EntityHolder::getBasePlayer(nstd::observer_ptr<Engine::IEdict> edict)
    {
        if (edict->getIndex() != 0u && edict->getIndex() <= gEngineLib->getMaxClients())
        {
            return nstd::dynamic_observer_cast<IBasePlayer>(_getEntity(edict));
        }

        return {};
    }

    nstd::observer_ptr<IBaseEntity> EntityHolder::allocEntity(nstd::observer_ptr<Engine::IEdict> edict)
    {
        if (edict->getIndex() != 0u && edict->getIndex() <= gEngineLib->getMaxClients())
        {
            const auto &[it, inserted] = m_entities.try_emplace(edict, std::make_unique<BasePlayer>(edict));
            if (!inserted)
            {
                it->second->updateSerialNumber(edict->getSerialNumber());
            }

            return it->second;
        }
        else
        {
            const auto &[it, inserted] = m_entities.try_emplace(edict, std::make_unique<BaseEntity>(edict));
            if (!inserted)
            {
                it->second->updateSerialNumber(edict->getSerialNumber());
            }

            return it->second;
        }

        return {};
    }

    nstd::observer_ptr<IBaseEntity> EntityHolder::getBaseEntity(CBaseEntity *baseEntity)
    {
        return getBaseEntity(gEngineLib->getEdict(baseEntity->pev));
    }

    nstd::observer_ptr<IBasePlayer> EntityHolder::getBasePlayer(CBasePlayer *basePlayer)
    {
        return getBasePlayer(gEngineLib->getEdict(basePlayer->pev));
    }

    nstd::observer_ptr<IBaseEntity> EntityHolder::getBaseEntity(entvars_t *entVars)
    {
        return getBaseEntity(gEngineLib->getEdict(entVars));
    }
} // namespace Anubis::Game::CStrike