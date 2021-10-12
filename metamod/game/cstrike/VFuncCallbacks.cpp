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

#include "VFuncCallbacks.hpp"

#include "MetaExports.hpp"
#include "EntityHolder.hpp"
#include "EntitiesHooks.hpp"
#include "BasePlayer.hpp"

#include <engine/IEntVars.hpp>
#include <engine/ITraceResult.hpp>

#include <type_traits>

using namespace Metamod;

namespace
{
    template<typename T, typename U, typename = std::enable_if_t<std::conjunction_v<
        std::is_base_of<Game::IBaseEntity, T>,
        std::is_base_of<CBaseEntity, U>>>>
    T *instanceToType(U *instance)
    {
        if constexpr (std::is_same_v<T, Game::IBasePlayer>)
        {
            return gEntityHolder.getBasePlayer(gEngineLib->getEdict(instance->edict()));
        }
        else if constexpr (std::is_same_v<T, Game::IBaseEntity>)
        {
            return gEntityHolder.getBaseEntity(gEngineLib->getEdict(instance->edict()));
        }

        return nullptr;
    }
}

namespace Metamod::Game::VFunc
{
    void vCBasePlayerSpawn(IReGameHook_CBasePlayer_Spawn *hook, CBasePlayer *player)
    {
        static Game::CStrike::BasePlayerSpawnHookRegistry *hookchain = gBasePlayerHooks->spawn();
        hookchain->callChain([player, hook](IBasePlayer *) {
            hook->callNext(player);
        }, [player, hook](IBasePlayer *) {
            hook->callOriginal(player);
        }, instanceToType<IBasePlayer>(player));
    }

    BOOL vCBasePlayerTakeDamage(IReGameHook_CBasePlayer_TakeDamage *hook,
                                    CBasePlayer *player,
                                    entvars_t *pevInflictor,
                                    entvars_t *pevAttacker,
                                    float &flDamage,
                                    std::int32_t bitsDamageType)
    {
        static Game::CStrike::BasePlayerTakeDamageHookRegistry *hookchain = gBasePlayerHooks->takeDamage();
        return hookchain->callChain(
            [player, hook](IBasePlayer *, Engine::IEntVars *pevInflictor,
                           Engine::IEntVars *pevAttacker, float flDamage, int bitsDamageType) {
                return hook->callNext(player, static_cast<entvars_t *>(*pevInflictor),
                                      static_cast<entvars_t *>(*pevAttacker), flDamage, bitsDamageType);
            }, [player, hook](IBasePlayer *, Engine::IEntVars *pevInflictor,
                              Engine::IEntVars *pevAttacker, float flDamage, int bitsDamageType) {
                return hook->callOriginal(player, static_cast<entvars_t *>(*pevInflictor),
                                          static_cast<entvars_t *>(*pevAttacker), flDamage, bitsDamageType);
            },
            instanceToType<IBasePlayer>(player), gEngineLib->getEntVars(pevInflictor),
            gEngineLib->getEntVars(pevAttacker), flDamage, bitsDamageType);
    }

    void vCBasePlayerTraceAttack(IReGameHook_CBasePlayer_TraceAttack *hook,
        CBasePlayer *player,
        entvars_t *pevAttacker,
        float flDamage,
        Vector &vecDir,
        ::TraceResult *ptr,
        std::int32_t bitsDamageType)
    {
        static Game::CStrike::BasePlayerTraceAttackHookRegistry *hookchain = gBasePlayerHooks->traceAttack();
        Engine::ITraceResult *metaTr = gEngineLib->createTraceResult(ptr);

        hookchain->callChain([hook, player, &vecDir](IBasePlayer *, Engine::IEntVars *pevAttacker,
                                            float flDamage, float *, Engine::ITraceResult *metatr,
                                            std::int32_t bitsDamageType) {
            hook->callNext(player, static_cast<entvars_t *>(*pevAttacker), flDamage, vecDir, *metatr, bitsDamageType);
        }, [hook, player, &vecDir](IBasePlayer *, Engine::IEntVars *pevAttacker,
                                    float flDamage, float *, Engine::ITraceResult *metatr,
                                    std::int32_t bitsDamageType) {
            hook->callOriginal(player, static_cast<entvars_t *>(*pevAttacker), flDamage, vecDir, *metatr, bitsDamageType);
        },
        instanceToType<IBasePlayer>(player), gEngineLib->getEntVars(pevAttacker),
        flDamage, &vecDir.x, metaTr, bitsDamageType);

        gEngineLib->freeTraceResult(metaTr);
    }

    void vCBasePlayerKilled(IReGameHook_CBasePlayer_Killed *hook, CBasePlayer *player,
                            entvars_t *pevAttacker, int iGib)
    {
        static Game::CStrike::BasePlayerKilledHookRegistry *hookchain = gBasePlayerHooks->killed();

        hookchain->callChain([hook, player](IBasePlayer *, Engine::IEntVars *pevAttacker, GibType gibType) {
            hook->callNext(player, static_cast<entvars_t *>(*pevAttacker), static_cast<int>(gibType));
        }, [hook, player](IBasePlayer *, Engine::IEntVars *pevAttacker, GibType gibType) {
            hook->callOriginal(player, static_cast<entvars_t *>(*pevAttacker), static_cast<int>(gibType));
        }, instanceToType<IBasePlayer>(player), gEngineLib->getEntVars(pevAttacker), static_cast<GibType>(iGib));
    }

    void vCBasePlayerGiveShield(IReGameHook_CBasePlayer_GiveShield *hook, CBasePlayer *player, bool deploy)
    {
        static Game::CStrike::BasePlayerGiveShieldHookRegistry *hookchain = gBasePlayerHooks->giveShield();

        hookchain->callChain([hook, player](IBasePlayer *, bool deploy) {
            hook->callNext(player, deploy);
        }, [hook, player](IBasePlayer *, bool deploy) {
            hook->callOriginal(player, deploy);
        }, instanceToType<IBasePlayer>(player), deploy);
    }

    CBaseEntity *vCBasePlayerDropShield(IReGameHook_CBasePlayer_DropShield *hook, CBasePlayer *player, bool deploy)
    {
        static Game::CStrike::BasePlayerDropShieldHookRegistry *hookchain = gBasePlayerHooks->dropShield();

        return static_cast<CBaseEntity *>(*dynamic_cast<CStrike::BaseEntity *>(hookchain->callChain([hook, player](IBasePlayer *, bool deploy) {
            return instanceToType<IBaseEntity>(hook->callNext(player, deploy));
        }, [hook, player](IBasePlayer *, bool deploy) {
            return instanceToType<IBaseEntity>(hook->callOriginal(player, deploy));
        }, instanceToType<IBasePlayer>(player), deploy)));
    }
}