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

#include <engine/Edict.hpp>
#include <entities/valve/BaseEntity.hpp>
#include <entities/valve/BasePlayer.hpp>
#include <entities/cstrike/BaseEntity.hpp>
#include <entities/cstrike/BasePlayer.hpp>

#include <Metamod.hpp>

namespace Metamod::Engine
{
    Edict::Edict(edict_t *edict, const std::uint32_t &maxClients)
        : m_edict(edict), m_entVars(std::make_unique<EntVars>(VARS(edict))),
          m_serialNumber(static_cast<std::uint32_t>(edict->serialnumber)),
          m_maxClients(maxClients)
    {
    }

    std::uint32_t Edict::getIndex() const
    {
        return ENTINDEX(m_edict);
    }

    std::uint32_t Edict::getSerialNumber() const
    {
        return static_cast<std::uint32_t>(m_edict->serialnumber);
    }

    bool Edict::isValid() const
    {
        return _isValid() && m_serialNumber == static_cast<std::uint32_t>(m_edict->serialnumber);
    }

    Entities::IBaseEntity *Edict::getBaseEntity()
    {
        switch (gMetaGlobal->getGameType())
        {
            case GameType::Valve:
                return getEntity<Entities::Valve::BaseEntity>();
            case GameType::CStrike:
            case GameType::CZero:
                return getEntity<Entities::CStrike::BaseEntity>();
            default:
                return nullptr;
        }
    }

    Entities::IBasePlayer *Edict::getBasePlayer()
    {
        std::uint32_t idx = getIndex();
        if (idx > m_maxClients || !idx)
        {
            return nullptr;
        }

        switch (gMetaGlobal->getGameType())
        {
            case GameType::Valve:
                return getEntity<Entities::Valve::BasePlayer>();
            case GameType::CStrike:
            case GameType::CZero:
                return getEntity<Entities::CStrike::BasePlayer>();
            default:
                return nullptr;
        }
    }

    EntVars *Edict::getEntVars() const
    {
        return m_entVars.get();
    }

    Edict::operator edict_t *() const
    {
        return m_edict;
    }

    bool Edict::_isValid() const
    {
        return !FNullEnt(m_edict) && !m_edict->free;
    }
} // namespace Metamod::Engine