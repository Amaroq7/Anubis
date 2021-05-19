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

#pragma once

#include <game/IBasePlayer.hpp>
#include <engine/IEdict.hpp>

#include <memory>

namespace Metamod::Game
{
    class IEntityHolder
    {
    public:
        virtual IBaseEntity *getBaseEntity(Engine::IEdict *edict) = 0;
        virtual IBasePlayer *getBasePlayer(Engine::IEdict *edict) = 0;

    protected:
        template<typename T, typename = std::enable_if_t<std::is_base_of_v<IBaseEntity, T>>>
        T *_getEntity(Engine::IEdict *edict)
        {
            if (!edict->isValid() || !edict->getPrivateData())
            {
                return nullptr;
            }

            const auto& [beginIt, endIt] = m_entities.equal_range(edict->getIndex());
            if (beginIt != m_entities.end() && beginIt->second->serialNumber != edict->getSerialNumber())
            {
                m_entities.erase(beginIt, endIt);
            }
            else
            {
                for (auto it = beginIt; it != endIt; ++it)
                {
                    if (auto entityBase = dynamic_cast<T *>(it->second->entity.get()); entityBase)
                    {
                        return entityBase;
                    }
                }
            }

            auto entity = std::make_unique<T>(edict);
            auto result = dynamic_cast<T *>(entity.get());
            m_entities.emplace(edict->getIndex(), std::make_unique<EntityInfo>(std::move(entity), edict->getSerialNumber()));

            return result;
        }

    protected:
        struct EntityInfo
        {
            EntityInfo(std::unique_ptr<IBaseEntity> &&ent, std::uint32_t snumber) :
                entity(std::move(ent)), serialNumber(snumber)
            {}

            std::unique_ptr<IBaseEntity> entity;
            std::uint32_t serialNumber;
        };

    protected:
        std::unordered_multimap<std::uint32_t, std::unique_ptr<EntityInfo>> m_entities;
    };
}