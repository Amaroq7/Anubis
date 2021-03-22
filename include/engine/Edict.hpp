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

#include <engine/IEdict.hpp>
#include <entities/IBasePlayer.hpp>
#include <extdll.h>
#include "EntVars.hpp"

#include <cinttypes>
#include <vector>
#include <memory>

namespace Metamod::Engine
{
    class Edict final : public IEdict
    {
        public:
            Edict() = delete;
            Edict(edict_t *edict, const std::uint32_t &maxClients);
            ~Edict() override = default;

            // IEdict
            std::uint32_t getIndex() const override;
            std::uint32_t getSerialNumber() const override;
            bool isValid() const override;
            Entities::IBaseEntity *getBaseEntity() override;
            Entities::IBasePlayer *getBasePlayer() override;

            EntVars *getEntVars() const override;

            // Edict
            operator edict_t *() const;

        private:
            bool _isValid() const;

            template<typename T, typename = std::enable_if_t<std::is_base_of_v<Entities::IBaseEntity, T>>>
            T *getEntity()
            {
                if (!_isValid() || !m_edict->pvPrivateData)
                {
                    return nullptr;
                }

                if (m_serialNumber != static_cast<std::uint32_t>(m_edict->serialnumber))
                {
                    m_entities.clear();
                    m_serialNumber = static_cast<std::uint32_t>(m_edict->serialnumber);
                }
                else
                {
                    for (const auto &entity : m_entities)
                    {
                        if (auto entityBase = dynamic_cast<T *>(entity.get()); entityBase)
                        {
                            return entityBase;
                        }
                    }
                }
                return dynamic_cast<T *>(m_entities.emplace_back(std::make_unique<T>(this)).get());
            }

        private:
            edict_t *m_edict = nullptr;
            std::vector<std::unique_ptr<Entities::IBaseEntity>> m_entities;
            std::unique_ptr<EntVars> m_entVars;
            std::uint32_t m_serialNumber = 0;
            const std::uint32_t &m_maxClients;
    };
} // namespace Metamod::Engine