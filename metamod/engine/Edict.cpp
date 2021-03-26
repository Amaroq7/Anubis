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

#include "Edict.hpp"

#include <Metamod.hpp>

namespace Metamod::Engine
{
    Edict::Edict(edict_t *edict)
        : m_edict(edict), m_entVars(std::make_unique<EntVars>(VARS(edict)))
    {}

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
        return !FNullEnt(m_edict) && !m_edict->free;
    }

    EntVars *Edict::getEntVars() const
    {
        return m_entVars.get();
    }

    Edict::operator edict_t *() const
    {
        return m_edict;
    }
} // namespace Metamod::Engine
