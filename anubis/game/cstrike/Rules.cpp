/*
*  Copyright (C) 2022 Anubis Development Team
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

#include "Rules.hpp"

#include <engine/IEdict.hpp>
#include <game/IBaseEntity.hpp>
#include <game/IBasePlayer.hpp>

#include "ReGameHeaders.hpp"

namespace Anubis::Game::CStrike
{
    Rules::Rules(nstd::observer_ptr<CGameRules> gameRules, nstd::observer_ptr<ILibrary> gameLib)
        : m_gameRules(gameRules),
          m_gameLib(gameLib)
    {
    }

    void Rules::refreshSkillData()
    {
        m_gameRules->RefreshSkillData();
    }

    void Rules::think()
    {
        m_gameRules->Think();
    }

    bool Rules::isAllowedToSpawn(nstd::observer_ptr<IBaseEntity> entity)
    {
        return m_gameRules->IsAllowedToSpawn(static_cast<CBaseEntity *>(*entity));
    }

    bool Rules::allowFlashlight()
    {
        return m_gameRules->FAllowFlashlight() != FALSE;
    }

    bool Rules::isMultiplayer()
    {
        return m_gameRules->IsMultiplayer() != FALSE;
    }

    bool Rules::isDeathMatch()
    {
        return m_gameRules->IsDeathmatch() != FALSE;
    }

    bool Rules::isTeamPlay()
    {
        return m_gameRules->IsTeamplay() != FALSE;
    }

    bool Rules::isCoOp()
    {
        return m_gameRules->IsCoOp() != FALSE;
    }

    std::string_view Rules::getGameDescription()
    {
        return m_gameRules->GetGameDescription();
    }

    bool Rules::clientConnected(nstd::observer_ptr<IBaseEntity> entity,
                                std::string_view name,
                                std::string_view address,
                                std::string &rejectReason)
    {
        static constexpr std::size_t MAX_REJECT_REASON = 128;
        static char szRejectReason[128];

        if (rejectReason.capacity() < MAX_REJECT_REASON)
        {
            rejectReason.resize(MAX_REJECT_REASON);
        }

        bool result = m_gameRules->ClientConnected(static_cast<edict_t *>(*entity->edict()), name.data(),
                                                   address.data(), szRejectReason) != FALSE;

        rejectReason = szRejectReason;

        return result;
    }

    void Rules::initHUD(nstd::observer_ptr<IBasePlayer> player)
    {
        m_gameRules->InitHUD(static_cast<CBasePlayer *>(*player));
    }

    void Rules::clientDisconnected(nstd::observer_ptr<IBaseEntity> client)
    {
        m_gameRules->ClientDisconnected(static_cast<edict_t *>(*client->edict()));
    }

    void Rules::updateGameMode(nstd::observer_ptr<IBasePlayer> player)
    {
        m_gameRules->UpdateGameMode(static_cast<CBasePlayer *>(*player));
    }

    float Rules::playerFallDamage(nstd::observer_ptr<IBasePlayer> player)
    {
        return m_gameRules->FlPlayerFallDamage(static_cast<CBasePlayer *>(*player));
    }

    bool Rules::playerCanTakeDamage(nstd::observer_ptr<IBasePlayer> player, nstd::observer_ptr<IBaseEntity> attacker)
    {
        return m_gameRules->FPlayerCanTakeDamage(static_cast<CBasePlayer *>(*player),
                                                 static_cast<CBaseEntity *>(*attacker)) != FALSE;
    }

    bool Rules::shouldAutoAim(nstd::observer_ptr<IBasePlayer> player, nstd::observer_ptr<IBaseEntity> target)
    {
        return m_gameRules->ShouldAutoAim(static_cast<CBasePlayer *>(*player),
                                          static_cast<edict_t *>(*target->edict())) != FALSE;
    }

    void Rules::playerSpawn(nstd::observer_ptr<IBasePlayer> player)
    {
        m_gameRules->PlayerSpawn(static_cast<CBasePlayer *>(*player));
    }

    void Rules::playerThink(nstd::observer_ptr<IBasePlayer> player)
    {
        m_gameRules->PlayerThink(static_cast<CBasePlayer *>(*player));
    }

    bool Rules::playerCanRespawn(nstd::observer_ptr<IBasePlayer> player)
    {
        return m_gameRules->FPlayerCanRespawn(static_cast<CBasePlayer *>(*player)) != FALSE;
    }

    float Rules::playerSpawnTime(nstd::observer_ptr<IBasePlayer> player)
    {
        return m_gameRules->FlPlayerSpawnTime(static_cast<CBasePlayer *>(*player));
    }

    nstd::observer_ptr<IBaseEntity> Rules::getPlayerSpawnSpot(nstd::observer_ptr<IBasePlayer> player)
    {
        return m_gameLib->getBaseEntity(m_gameRules->GetPlayerSpawnSpot(static_cast<CBasePlayer *>(*player)));
    }

    bool Rules::allowAutoTargetCrosshair()
    {
        return m_gameRules->AllowAutoTargetCrosshair() != FALSE;
    }

    bool Rules::clientCommand(nstd::observer_ptr<IBasePlayer> player, std::string_view cmd)
    {
        return m_gameRules->ClientCommand(static_cast<CBasePlayer *>(*player), cmd.data()) != FALSE;
    }

    void Rules::clientUserInfoChanged(nstd::observer_ptr<IBasePlayer> player, Engine::InfoBuffer infobuffer)
    {
        m_gameRules->ClientUserInfoChanged(static_cast<CBasePlayer *>(*player), infobuffer.value);
    }

    std::int32_t Rules::pointsForKill(nstd::observer_ptr<IBasePlayer> attacker, nstd::observer_ptr<IBasePlayer> killed)
    {
        return m_gameRules->IPointsForKill(static_cast<CBasePlayer *>(*attacker), static_cast<CBasePlayer *>(*killed));
    }

    void Rules::playerKilled(nstd::observer_ptr<IBasePlayer> victim,
                             nstd::observer_ptr<IBaseEntity> killer,
                             nstd::observer_ptr<IBaseEntity> inflictor)
    {
        m_gameRules->PlayerKilled(static_cast<CBasePlayer *>(*victim), static_cast<entvars_t *>(*killer),
                                  static_cast<entvars_t *>(*inflictor));
    }

    void Rules::deathNotice(nstd::observer_ptr<IBasePlayer> victim,
                            nstd::observer_ptr<IBaseEntity> killer,
                            nstd::observer_ptr<IBaseEntity> inflictor)
    {
        m_gameRules->DeathNotice(static_cast<CBasePlayer *>(*victim), static_cast<entvars_t *>(*killer),
                                 static_cast<entvars_t *>(*inflictor));
    }

    bool Rules::canHaveAmmo(nstd::observer_ptr<IBasePlayer> player, std::string_view ammoName, std::int32_t maxCarry)
    {
        return m_gameRules->CanHaveAmmo(static_cast<CBasePlayer *>(*player), ammoName.data(), maxCarry) != FALSE;
    }

    void Rules::playerGotAmmo(nstd::observer_ptr<IBasePlayer> player, std::string_view name, std::int32_t count)
    {
        m_gameRules->PlayerGotAmmo(static_cast<CBasePlayer *>(*player), const_cast<char *>(name.data()), count);
    }

    float Rules::healthChargerRechargeTime()
    {
        return m_gameRules->FlHealthChargerRechargeTime();
    }

    float Rules::hevChargerRechargeTime()
    {
        return m_gameRules->FlHEVChargerRechargeTime();
    }

    DeadPlayerWeapon Rules::deadPlayerWeapons(nstd::observer_ptr<IBasePlayer> player)
    {
        return static_cast<DeadPlayerWeapon>(m_gameRules->DeadPlayerWeapons(static_cast<CBasePlayer *>(*player)));
    }

    DeadPlayerAmmo Rules::deadPlayerAmmo(nstd::observer_ptr<IBasePlayer> player)
    {
        return static_cast<DeadPlayerAmmo>(m_gameRules->DeadPlayerAmmo(static_cast<CBasePlayer *>(*player)));
    }

    std::string_view Rules::getTeamID(nstd::observer_ptr<IBaseEntity> entity)
    {
        return m_gameRules->GetTeamID(static_cast<CBaseEntity *>(*entity));
    }

    PlayerRelationship Rules::playerRelationship(nstd::observer_ptr<IBasePlayer> player,
                                                 nstd::observer_ptr<IBaseEntity> target)
    {
        return static_cast<PlayerRelationship>(
            m_gameRules->PlayerRelationship(static_cast<CBasePlayer *>(*player), static_cast<CBaseEntity *>(*target)));
    }

    TeamIndex Rules::getTeamIndex(std::string_view teamName)
    {
        return TeamIndex(static_cast<TeamIndex::BaseType>(m_gameRules->GetTeamIndex(teamName.data())));
    }

    std::string_view Rules::getIndexedTeamName(TeamIndex teamIndex)
    {
        return m_gameRules->GetIndexedTeamName(static_cast<int>(teamIndex.value));
    }

    bool Rules::isValidTeam(std::string_view teamName)
    {
        return m_gameRules->IsValidTeam(teamName.data()) != FALSE;
    }

    void Rules::changePlayerTeam(nstd::observer_ptr<IBasePlayer> player, std::string_view teamName, bool kill, bool gib)
    {
        m_gameRules->ChangePlayerTeam(static_cast<CBasePlayer *>(*player), teamName.data(), static_cast<int>(kill),
                                      static_cast<int>(gib));
    }

    std::string_view Rules::setDefaultPlayerTeam(nstd::observer_ptr<IBasePlayer> player)
    {
        return m_gameRules->SetDefaultPlayerTeam(static_cast<CBasePlayer *>(*player));
    }

    bool Rules::playTextureSounds()
    {
        return m_gameRules->PlayTextureSounds() != FALSE;
    }

    bool Rules::playFootstepSounds(nstd::observer_ptr<IBasePlayer> player [[maybe_unused]], float vol [[maybe_unused]])
    {
        return false;
    }

    bool Rules::allowMonsters()
    {
        return m_gameRules->FAllowMonsters() != FALSE;
    }

    void Rules::endMultiplayerGame()
    {
        m_gameRules->EndMultiplayerGame();
    }

} // namespace Anubis::Game::CStrike
