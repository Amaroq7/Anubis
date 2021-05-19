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

    /*int TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType); */
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
                return hook->callNext(player, *pevInflictor, *pevAttacker, flDamage, bitsDamageType);
            }, [player, hook](IBasePlayer *, Engine::IEntVars *pevInflictor,
                              Engine::IEntVars *pevAttacker, float flDamage, int bitsDamageType) {
                return hook->callOriginal(player, *pevInflictor, *pevAttacker, flDamage, bitsDamageType);
            },
            instanceToType<IBasePlayer>(player), gEngineLib->getEntVars(pevInflictor),
            gEngineLib->getEntVars(pevAttacker), flDamage, bitsDamageType);
    }

    /*void TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int
 * bitsDamageType); */
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

        hookchain->callChain([hook, player, &vecDir] (IBasePlayer *, Engine::IEntVars *pevAttacker,
                                            float flDamage, float *, Engine::ITraceResult *metatr,
                                            std::int32_t bitsDamageType) {
            hook->callNext(player, *pevAttacker, flDamage, vecDir, *metatr, bitsDamageType);
        }, [hook, player, &vecDir] (IBasePlayer *, Engine::IEntVars *pevAttacker,
                                    float flDamage, float *, Engine::ITraceResult *metatr,
                                    std::int32_t bitsDamageType) {
            hook->callOriginal(player, *pevAttacker, flDamage, vecDir, *metatr, bitsDamageType);
        },
        instanceToType<IBasePlayer>(player), gEngineLib->getEntVars(pevAttacker),
        flDamage, &vecDir.x, metaTr, bitsDamageType);

        gEngineLib->freeTraceResult(metaTr);
    }
}