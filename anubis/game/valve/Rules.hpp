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

#pragma once

#include <game/IRules.hpp>
#include <game/ILibrary.hpp>

class CGameRules;

namespace Anubis::Game::Valve
{
    class Rules final : public IRules
    {
    public:
        ~Rules() final = default;
        Rules(nstd::observer_ptr<CGameRules> gameRules, nstd::observer_ptr<ILibrary> gameLib);

        void refreshSkillData() final;
        void think() final;
        bool isAllowedToSpawn(nstd::observer_ptr<IBaseEntity> entity) final;

        bool allowFlashlight() final;

        bool isMultiplayer() final;
        bool isDeathMatch() final;
        bool isTeamPlay() final;
        bool isCoOp() final;
        std::string_view getGameDescription() final;

        bool clientConnected(nstd::observer_ptr<IBaseEntity> entity,
                             std::string_view name,
                             std::string_view address,
                             std::string &rejectReason) final;

        void initHUD(nstd::observer_ptr<IBasePlayer> player) final;
        void clientDisconnected(nstd::observer_ptr<IBaseEntity> client) final;
        void updateGameMode(nstd::observer_ptr<IBasePlayer> player) final;

        float playerFallDamage(nstd::observer_ptr<IBasePlayer> player) final;
        bool playerCanTakeDamage(nstd::observer_ptr<IBasePlayer> player,
                                 nstd::observer_ptr<IBaseEntity> attacker) final;
        bool shouldAutoAim(nstd::observer_ptr<IBasePlayer> player, nstd::observer_ptr<IBaseEntity> target) final;

        void playerSpawn(nstd::observer_ptr<IBasePlayer> player) final;
        void playerThink(nstd::observer_ptr<IBasePlayer> player) final;
        bool playerCanRespawn(nstd::observer_ptr<IBasePlayer> player) final;
        float playerSpawnTime(nstd::observer_ptr<IBasePlayer> player) final;
        nstd::observer_ptr<IBaseEntity> getPlayerSpawnSpot(nstd::observer_ptr<IBasePlayer> player) final;

        bool allowAutoTargetCrosshair() final;
        bool clientCommand(nstd::observer_ptr<IBasePlayer> player, std::string_view cmd) final;
        void clientUserInfoChanged(nstd::observer_ptr<IBasePlayer> player, Engine::InfoBuffer infobuffer) final;

        std::int32_t pointsForKill(nstd::observer_ptr<IBasePlayer> attacker,
                                   nstd::observer_ptr<IBasePlayer> killed) final;
        void playerKilled(nstd::observer_ptr<IBasePlayer> victim,
                          nstd::observer_ptr<IBaseEntity> killer,
                          nstd::observer_ptr<IBaseEntity> inflictor) final;
        void deathNotice(nstd::observer_ptr<IBasePlayer> victim,
                         nstd::observer_ptr<IBaseEntity> killer,
                         nstd::observer_ptr<IBaseEntity> inflictor) final;

        bool
            canHaveAmmo(nstd::observer_ptr<IBasePlayer> player, std::string_view ammoName, std::int32_t maxCarry) final;
        void playerGotAmmo(nstd::observer_ptr<IBasePlayer> player, std::string_view name, std::int32_t count) final;

        float healthChargerRechargeTime() final;
        float hevChargerRechargeTime() final;

        DeadPlayerWeapon deadPlayerWeapons(nstd::observer_ptr<IBasePlayer> player) final;
        DeadPlayerAmmo deadPlayerAmmo(nstd::observer_ptr<IBasePlayer> player) final;

        std::string_view getTeamID(nstd::observer_ptr<IBaseEntity> entity) final;
        PlayerRelationship playerRelationship(nstd::observer_ptr<IBasePlayer> player,
                                              nstd::observer_ptr<IBaseEntity> target) final;
        TeamIndex getTeamIndex(std::string_view teamName) final;
        std::string_view getIndexedTeamName(TeamIndex teamIndex) final;
        bool isValidTeam(std::string_view teamName) final;
        void changePlayerTeam(nstd::observer_ptr<IBasePlayer> player,
                              std::string_view teamName,
                              bool kill,
                              bool gib) final;
        std::string_view setDefaultPlayerTeam(nstd::observer_ptr<IBasePlayer> player) final;
        bool playTextureSounds() final;
        bool playFootstepSounds(nstd::observer_ptr<IBasePlayer> player, float vol) final;

        bool allowMonsters() final;

        void endMultiplayerGame() final;

    private:
        nstd::observer_ptr<CGameRules> m_gameRules;
        nstd::observer_ptr<ILibrary> m_gameLib;
    };
} // namespace Anubis::Game::Valve