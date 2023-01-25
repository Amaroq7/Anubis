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

#pragma once

#include <observer_ptr.hpp>
#include <game/IEntityHolder.hpp>
#include "ReGameHeaders.hpp"
#include "BaseEntity.hpp"
#include "BasePlayerItem.hpp"
#include "BasePlayerWeapon.hpp"
#include "BasePlayerAmmo.hpp"
#include "BasePlayer.hpp"
#include "AnubisExports.hpp"

class CBaseEntity;
class CBasePlayer;

namespace Anubis::Game::CStrike
{
    class EntityHolder final : public IEntityHolder
    {
    public:
        std::unique_ptr<IBaseEntity> getBaseEntity(nstd::observer_ptr<Engine::IEdict> edict) final;
        std::unique_ptr<IBasePlayer> getBasePlayer(nstd::observer_ptr<Engine::IEdict> edict) final;
        std::unique_ptr<IBasePlayerItem> getBasePlayerItem(nstd::observer_ptr<Engine::IEdict> edict) final;
        std::unique_ptr<IBasePlayerWeapon> getBasePlayerWeapon(nstd::observer_ptr<Engine::IEdict> edict) final;
        std::unique_ptr<IBasePlayerAmmo> getBasePlayerAmmo(nstd::observer_ptr<Engine::IEdict> edict) final;

        std::unique_ptr<IBaseEntity> getBaseEntity(CBaseEntity *baseEntity);
        std::unique_ptr<IBasePlayer> getBasePlayer(CBasePlayer *basePlayer);
        std::unique_ptr<IBaseEntity> getBaseEntity(edict_t *edict) final;
        std::unique_ptr<IBaseEntity> getBaseEntity(entvars_t *entVars) final;

    private:
        template<typename T,
                 typename U,
                 typename = std::enable_if_t<
                     std::conjunction_v<std::is_base_of<IBaseEntity, T>, std::is_base_of<BaseEntity, U>>>>
        std::unique_ptr<T> _getEntity(nstd::observer_ptr<Engine::IEdict> edict) const
        {
            if constexpr (std::is_same_v<IBaseEntity, T>)
            {
                return std::make_unique<U>(edict);
            }
            else if constexpr (std::is_same_v<IBasePlayer, T>)
            {
                if (edict->getIndex() != 0u && edict->getIndex() <= gEngineLib->getMaxClients())
                {
                    return std::make_unique<BasePlayer>(edict);
                }

                return {};
            }
            else
            {
                if (edict->getIndex() > gEngineLib->getMaxClients())
                {
                    return std::make_unique<U>(edict);
                }
            }

            return nullptr;
        }
    };

    const std::unique_ptr<EntityHolder> &getEntityHolder();
} // namespace Anubis::Game::CStrike