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

#include "Edict.hpp"
#include <engine/ILibrary.hpp>

#include <extdll.h>

namespace Anubis::Engine
{
    Edict::Edict(edict_t *edict, nstd::observer_ptr<ILibrary> engine) : m_edict(edict), m_engine(engine) {}

    std::uint32_t Edict::getIndex() const
    {
        return m_engine->getIndexOfEdict(m_edict);
    }

    std::uint32_t Edict::getSerialNumber() const
    {
        return static_cast<std::uint32_t>(m_edict->serialnumber);
    }

    bool Edict::isFree() const
    {
        return m_edict->free;
    }

    void *Edict::getPrivateData() const
    {
        return m_edict->pvPrivateData;
    }

    EntFlags Edict::getFlags() const
    {
        return static_cast<EntFlags>(m_edict->v.flags);
    }

    void Edict::setFlags(EntFlags flags)
    {
        m_edict->v.flags = static_cast<int>(flags);
    }

    Edict::operator entvars_t *() const
    {
        return &m_edict->v;
    }

    Edict::operator edict_t *() const
    {
        return m_edict;
    }

} // namespace Anubis::Engine
