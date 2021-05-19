/*
 *  Copyright (C) 2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "BasePlayer.hpp"

#include <engine/IEntVars.hpp>

#include <extdll.h>
#include <tier0/platform.h>
#include <enginecallback.h>
#include <util.h>
#include <cbase.h>
#include <player.h>
#include <regamedll_interfaces.h>

namespace Metamod::Game::Entities::CStrike
{
    BasePlayer::BasePlayer(Engine::IEdict *edict) : BaseMonster(edict) {}

    BasePlayer::operator CBasePlayer *() const
    {
        return reinterpret_cast<CBasePlayer *>(m_edict->getPrivateData());
    }

    void BasePlayer::makeVIP()
    {
        operator CBasePlayer *()->CSPlayer()->MakeVIP();
    }

    bool BasePlayer::takeDamage(Engine::IEntVars *pevInflictor,
                                Engine::IEntVars *pevAttacker,
                                float flDamage,
                                std::int32_t bitsDamageType)
    {
        return operator CBasePlayer *()->TakeDamage(*pevInflictor, *pevAttacker, flDamage, bitsDamageType) == TRUE;
    }
} // namespace Valve
