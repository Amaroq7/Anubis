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

#include "VFuncCallbacks.hpp"

#include "AnubisExports.hpp"
#include "EntityHolder.hpp"
#include "EntitiesHooks.hpp"

#include <engine/ITraceResult.hpp>

#include <type_traits>

namespace Anubis::Game::VFunc
{
    void vCBasePlayerSpawn(IReGameHook_CBasePlayer_Spawn *hook, CBasePlayer *player)
    {
        static auto hookChain = CStrike::getBasePlayerHooks()->spawn();
        hookChain->callChain(
            [player, hook](const std::unique_ptr<IBasePlayer> &)
            {
                hook->callNext(player);
            },
            [player, hook](const std::unique_ptr<IBasePlayer> &)
            {
                hook->callOriginal(player);
            },
            CStrike::getEntityHolder()->getBasePlayer(player));
    }

    BOOL vCBasePlayerTakeDamage(IReGameHook_CBasePlayer_TakeDamage *hook,
                                CBasePlayer *player,
                                entvars_t *pevInflictor,
                                entvars_t *pevAttacker,
                                float &flDamage,
                                std::int32_t bitsDamageType)
    {
        static auto hookChain = CStrike::getBasePlayerHooks()->takeDamage();

        return hookChain->callChain(
            [player, hook](const std::unique_ptr<IBasePlayer> &, nstd::observer_ptr<IBaseEntity> pevInflictor,
                           nstd::observer_ptr<IBaseEntity> pevAttacker, float &flDamage, DmgType bitsDamageType)
            {
                return hook->callNext(player, static_cast<entvars_t *>(*pevInflictor),
                                      static_cast<entvars_t *>(*pevAttacker), flDamage,
                                      static_cast<int>(bitsDamageType));
            },
            [player, hook](const std::unique_ptr<IBasePlayer> &, nstd::observer_ptr<IBaseEntity> pevInflictor,
                           nstd::observer_ptr<IBaseEntity> pevAttacker, float &flDamage, DmgType bitsDamageType)
            {
                return hook->callOriginal(player, static_cast<entvars_t *>(*pevInflictor),
                                          static_cast<entvars_t *>(*pevAttacker), flDamage,
                                          static_cast<int>(bitsDamageType));
            },
            CStrike::getEntityHolder()->getBasePlayer(player), CStrike::getEntityHolder()->getBaseEntity(pevInflictor),
            CStrike::getEntityHolder()->getBaseEntity(pevAttacker), flDamage, static_cast<DmgType>(bitsDamageType));
    }

    void vCBasePlayerTraceAttack(IReGameHook_CBasePlayer_TraceAttack *hook,
                                 CBasePlayer *player,
                                 entvars_t *pevAttacker,
                                 float flDamage,
                                 Vector &vecDir,
                                 TraceResult *ptr,
                                 std::int32_t bitsDamageType)
    {
        std::unique_ptr<Engine::ITraceResult> metaTr = gEngineLib->createTraceResult(ptr);
        static auto hookChain = CStrike::getBasePlayerHooks()->traceAttack();

        hookChain->callChain(
            [hook, player, &vecDir](const std::unique_ptr<IBasePlayer> &, nstd::observer_ptr<IBaseEntity> pevAttacker,
                                    float flDamage, float *, const std::unique_ptr<Engine::ITraceResult> &metatr,
                                    DmgType bitsDamageType)
            {
                hook->callNext(player, static_cast<entvars_t *>(*pevAttacker), flDamage, vecDir,
                               static_cast<TraceResult *>(*metatr), static_cast<int>(bitsDamageType));
            },
            [hook, player, &vecDir](const std::unique_ptr<IBasePlayer> &, nstd::observer_ptr<IBaseEntity> pevAttacker,
                                    float flDamage, float *, const std::unique_ptr<Engine::ITraceResult> &metatr,
                                    DmgType bitsDamageType)
            {
                hook->callOriginal(player, static_cast<entvars_t *>(*pevAttacker), flDamage, vecDir,
                                   static_cast<TraceResult *>(*metatr), static_cast<int>(bitsDamageType));
            },
            CStrike::getEntityHolder()->getBasePlayer(player), CStrike::getEntityHolder()->getBaseEntity(pevAttacker),
            flDamage, &vecDir.x, metaTr, static_cast<DmgType>(bitsDamageType));
    }

    void vCBasePlayerKilled(IReGameHook_CBasePlayer_Killed *hook, CBasePlayer *player, entvars_t *pevAttacker, int iGib)
    {
        static auto hookChain = CStrike::getBasePlayerHooks()->killed();

        hookChain->callChain(
            [hook, player](const std::unique_ptr<IBasePlayer> &, nstd::observer_ptr<IBaseEntity> pevAttacker,
                           GibType gibType)
            {
                hook->callNext(player, static_cast<entvars_t *>(*pevAttacker), static_cast<int>(gibType));
            },
            [hook, player](const std::unique_ptr<IBasePlayer> &, nstd::observer_ptr<IBaseEntity> pevAttacker,
                           GibType gibType)
            {
                hook->callOriginal(player, static_cast<entvars_t *>(*pevAttacker), static_cast<int>(gibType));
            },
            CStrike::getEntityHolder()->getBasePlayer(player), CStrike::getEntityHolder()->getBaseEntity(pevAttacker),
            static_cast<GibType>(iGib));
    }

    void vCBasePlayerGiveShield(IReGameHook_CBasePlayer_GiveShield *hook, CBasePlayer *player, bool deploy)
    {
        static auto hookChain = CStrike::getBasePlayerHooks()->giveShield();
        hookChain->callChain(
            [hook, player](const std::unique_ptr<IBasePlayer> &, bool deploy)
            {
                hook->callNext(player, deploy);
            },
            [hook, player](const std::unique_ptr<IBasePlayer> &, bool deploy)
            {
                hook->callOriginal(player, deploy);
            },
            CStrike::getEntityHolder()->getBasePlayer(player), deploy);
    }

    CBaseEntity *vCBasePlayerDropShield(IReGameHook_CBasePlayer_DropShield *hook, CBasePlayer *player, bool deploy)
    {
        static auto hookChain = CStrike::getBasePlayerHooks()->dropShield();
        return static_cast<CBaseEntity *>(*hookChain->callChain(
            [hook, player](const std::unique_ptr<IBasePlayer> &, bool deploy)
            {
                return CStrike::getEntityHolder()->getBaseEntity(hook->callNext(player, deploy));
            },
            [hook, player](const std::unique_ptr<IBasePlayer> &, bool deploy)
            {
                return CStrike::getEntityHolder()->getBaseEntity(hook->callOriginal(player, deploy));
            },
            CStrike::getEntityHolder()->getBasePlayer(player), deploy));
    }
} // namespace Anubis::Game::VFunc