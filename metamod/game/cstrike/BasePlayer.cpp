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
#include <API/CSEntity.h>
#include <API/CSPlayer.h>

namespace Metamod::Game::CStrike
{
    BasePlayer::BasePlayer(Engine::IEdict *edict) : BaseMonster(edict) { }

    BasePlayer::operator CBasePlayer *() const
    {
        return reinterpret_cast<CBasePlayer *>(m_entity);
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
        return operator CBasePlayer *()->TakeDamage(static_cast<entvars_t *>(*pevInflictor),
                                                    static_cast<entvars_t *>(*pevAttacker),
                                                    flDamage, bitsDamageType) == TRUE;
    }

    void BasePlayer::giveShield(bool deploy)
    {
        operator CBasePlayer *()->CSPlayer()->GiveShield(deploy);
    }

    void BasePlayer::removeShield()
    {
        operator CBasePlayer *()->CSPlayer()->RemoveShield();
    }

    void BasePlayer::dropShield(bool deploy)
    {
        operator CBasePlayer *()->CSPlayer()->DropShield(deploy);
    }

    bool BasePlayer::hasShield() const
    {
        return operator CBasePlayer *()->m_bOwnsShield;
    }
} // namespace CStrike
