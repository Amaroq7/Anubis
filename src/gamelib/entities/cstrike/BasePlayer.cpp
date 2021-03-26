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

#include <gamelib/entities/cstrike/BasePlayer.hpp>
#include <enginecallback.h>
#include <tier0/platform.h>
#include <cbase.h>
#include <player.h>
#include <regamedll_interfaces.h>

namespace Metamod::GameLib::Entities::CStrike
{
    BasePlayer::BasePlayer(const Engine::Edict *edict) : BaseMonster(edict)
    {}

    BasePlayer::operator CBasePlayer *() const
    {
        return reinterpret_cast<CBasePlayer *>(GET_PRIVATE(*m_edict));
    }

    void BasePlayer::makeVIP()
    {
        operator CBasePlayer *()->CSPlayer()->MakeVIP();
    }
} // namespace Valve