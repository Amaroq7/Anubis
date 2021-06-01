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

#include "BasePlayer.hpp"

#include <engine/IEntVars.hpp>
#include <extdll.h>
#include <tier0/platform.h>
#include <util.h>
#include <cbase.h>
#include <player.h>

namespace Metamod::Game::Valve
{
    BasePlayer::BasePlayer(Engine::IEdict *edict)
        : BaseMonster(edict)
    {}

    BasePlayer::operator CBasePlayer *() const
    {
        return reinterpret_cast<CBasePlayer *>(m_edict->getPrivateData());
    }

    void BasePlayer::makeVIP() { /* CStrike only */ }

    bool BasePlayer::takeDamage(Engine::IEntVars *pevInflictor,
                               Engine::IEntVars *pevAttacker,
                               float flDamage,
                               std::int32_t bitsDamageType)
    {
        return operator CBasePlayer *()->TakeDamage(static_cast<entvars_t *>(*pevInflictor),
                                                    static_cast<entvars_t *>(*pevAttacker),
                                                    flDamage, bitsDamageType) == TRUE;
    }
} // namespace Valve
