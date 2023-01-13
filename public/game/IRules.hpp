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

#include "../Common.hpp"
#include "../observer_ptr.hpp"

#include <string_view>

namespace Anubis::Engine
{
    struct InfoBuffer;
}

namespace Anubis::Game
{
    class IBaseEntity;
    class IBasePlayer;
    class IBasePlayerItem;
    class IItem;

    ANUBIS_STRONG_TYPEDEF(std::uint8_t, TeamIndex)

    enum class PlayerRelationship : std::uint8_t
    {
        NotTeamMate = 0,
        TeamMate,
        Enemy,
        Ally,
        Neutral,
    };

    enum class DeadPlayerWeapon : std::uint8_t
    {
        None = 0,
        DropAll = 7,
        DropActive = 8,
        NoDrop = 9
    };

    enum class DeadPlayerAmmo : std::uint8_t
    {
        None = 0,
        DropAll = 10,
        DropActive = 11,
        NoDrop = 12
    };

    class IRules
    {
    public:
        virtual ~IRules() = default;

        virtual void refreshSkillData() = 0;
        virtual void think() = 0;
        virtual bool isAllowedToSpawn(nstd::observer_ptr<IBaseEntity> entity) = 0;

        virtual bool allowFlashlight() = 0;
        // virtual bool shouldSwitchWeapon(nstd::observer_ptr<IBasePlayer> player,
        // nstd::observer_ptr<IBasePlayerItem> pWeapon) = 0;
        // virtual bool getNextBestWeapon(nstd::observer_ptr<IBasePlayer> player,
        //                                nstd::observer_ptr<IBasePlayerItem> currentWeapon) = 0;

        virtual bool isMultiplayer() = 0;
        virtual bool isDeathMatch() = 0;
        virtual bool isTeamPlay() = 0;
        virtual bool isCoOp() = 0;
        virtual std::string_view getGameDescription() = 0;

        virtual bool clientConnected(nstd::observer_ptr<IBaseEntity> entity,
                                     std::string_view name,
                                     std::string_view address,
                                     std::string &rejectReason) = 0;

        virtual void initHUD(nstd::observer_ptr<IBasePlayer> pl) = 0;
        virtual void clientDisconnected(nstd::observer_ptr<IBaseEntity> client) = 0;
        virtual void updateGameMode(nstd::observer_ptr<IBasePlayer> player) = 0;

        virtual float playerFallDamage(nstd::observer_ptr<IBasePlayer> player) = 0;
        virtual bool playerCanTakeDamage(nstd::observer_ptr<IBasePlayer> player,
                                         nstd::observer_ptr<IBaseEntity> attacker) = 0;
        virtual bool shouldAutoAim(nstd::observer_ptr<IBasePlayer> player, nstd::observer_ptr<IBaseEntity> target) = 0;

        virtual void playerSpawn(nstd::observer_ptr<IBasePlayer> player) = 0;
        virtual void playerThink(nstd::observer_ptr<IBasePlayer> player) = 0;
        virtual bool playerCanRespawn(nstd::observer_ptr<IBasePlayer> player) = 0;
        virtual float playerSpawnTime(nstd::observer_ptr<IBasePlayer> player) = 0;
        virtual nstd::observer_ptr<IBaseEntity> getPlayerSpawnSpot(nstd::observer_ptr<IBasePlayer> player) = 0;

        virtual bool allowAutoTargetCrosshair() = 0;
        virtual bool clientCommand(nstd::observer_ptr<IBasePlayer> player, std::string_view cmd) = 0;
        virtual void clientUserInfoChanged(nstd::observer_ptr<IBasePlayer> player, Engine::InfoBuffer infobuffer) = 0;

        virtual std::int32_t pointsForKill(nstd::observer_ptr<IBasePlayer> attacker,
                                           nstd::observer_ptr<IBasePlayer> killed) = 0;
        virtual void playerKilled(nstd::observer_ptr<IBasePlayer> victim,
                                  nstd::observer_ptr<IBaseEntity> killer,
                                  nstd::observer_ptr<IBaseEntity> inflictor) = 0;
        virtual void deathNotice(nstd::observer_ptr<IBasePlayer> victim,
                                 nstd::observer_ptr<IBaseEntity> killer,
                                 nstd::observer_ptr<IBaseEntity> inflictor) = 0;

        // virtual bool canHavePlayerItem(nstd::observer_ptr<IBasePlayer> player,
        //                                nstd::observer_ptr<IBasePlayerItem> pWeapon) = 0;
        // virtual void playerGotWeapon(nstd::observer_ptr<IBasePlayer> player,
        //                              nstd::observer_ptr<IBasePlayerItem> pWeapon) = 0;

        // virtual std::int32_t weaponShouldRespawn(nstd::observer_ptr<IBasePlayerItem> pWeapon) = 0;
        // virtual float weaponRespawnTime(nstd::observer_ptr<IBasePlayerItem> pWeapon) = 0;
        // virtual float weaponTryRespawn(nstd::observer_ptr<IBasePlayerItem> pWeapon) = 0;
        //  virtual Vector weaponRespawnSpot(nstd::observer_ptr<IBasePlayerItem> pWeapon) = 0;

        // virtual bool canHaveItem(CBasePlayer *player, CItem *item) = 0;
        // virtual void playerGotItem(CBasePlayer *player, CItem *item) = 0;

        // virtual int itemShouldRespawn( CItem *item ) = 0;
        // virtual float itemRespawnTime( CItem *item ) = 0;
        // virtual Vector itemRespawnSpot( CItem *item ) = 0;

        virtual bool
            canHaveAmmo(nstd::observer_ptr<IBasePlayer> player, std::string_view ammoName, std::int32_t maxCarry) = 0;
        virtual void
            playerGotAmmo(nstd::observer_ptr<IBasePlayer> player, std::string_view name, std::int32_t count) = 0;

        // virtual int ammoShouldRespawn( CBasePlayerAmmo *pAmmo ) = 0;
        // virtual float ammoRespawnTime( CBasePlayerAmmo *pAmmo ) = 0;
        // virtual Vector ammoRespawnSpot( CBasePlayerAmmo *pAmmo ) = 0;

        virtual float healthChargerRechargeTime() = 0;
        virtual float hevChargerRechargeTime() = 0;

        virtual DeadPlayerWeapon deadPlayerWeapons(nstd::observer_ptr<IBasePlayer> player) = 0;
        virtual DeadPlayerAmmo deadPlayerAmmo(nstd::observer_ptr<IBasePlayer> player) = 0;

        virtual std::string_view getTeamID(nstd::observer_ptr<IBaseEntity> entity) = 0;
        virtual PlayerRelationship playerRelationship(nstd::observer_ptr<IBasePlayer> player,
                                                      nstd::observer_ptr<IBaseEntity> target) = 0;
        virtual TeamIndex getTeamIndex(std::string_view teamName) = 0;
        virtual std::string_view getIndexedTeamName(TeamIndex teamIndex) = 0;
        virtual bool isValidTeam(std::string_view teamName) = 0;
        virtual void changePlayerTeam(nstd::observer_ptr<IBasePlayer> player,
                                      std::string_view teamName,
                                      bool kill,
                                      bool gib) = 0;
        virtual std::string_view setDefaultPlayerTeam(nstd::observer_ptr<IBasePlayer> player) = 0;
        virtual bool playTextureSounds() = 0;
        virtual bool playFootstepSounds(nstd::observer_ptr<IBasePlayer> player, float vol) = 0;

        virtual bool allowMonsters() = 0;

        virtual void endMultiplayerGame() = 0;
    };
} // namespace Anubis::Game