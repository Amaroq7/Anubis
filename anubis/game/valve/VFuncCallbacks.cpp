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
#include "EntitiesHooks.hpp"
#include "EntityHolder.hpp"
#include "Config.hpp"
#include <engine/ITraceResult.hpp>
#include <game/IBasePlayer.hpp>
#include <extdll.h>

std::intptr_t CWorldVTable;
std::intptr_t CWorldSpawnOrigFn;

class CGameRules;

namespace
{
    using namespace Anubis;

    inline std::intptr_t getVTable(void *instance)
    {
        return *(reinterpret_cast<std::intptr_t *>(instance));
    }

    template<typename t_ret = void, typename... t_args>
    inline t_ret execVFunc(std::intptr_t vFunc, void *instance, t_args... args)
    {
#if defined __linux__
        return reinterpret_cast<t_ret (*)(void *, t_args...)>(vFunc)(instance, args...);
#elif defined _WIN32
        return reinterpret_cast<t_ret(__fastcall *)(void *, int, t_args...)>(vFunc)(instance, 0, args...);
#endif
    }
} // namespace

namespace Anubis::Game::VFunc
{
    std::uint32_t gPevOffset;

    /* virtual void	Spawn( void ) */
    void FASTCALL vSpawnHook(void *instance
#if defined _WIN32
                             ,
                             void *notUsed
#endif
    )
    {
        if (getVTable(instance) == IBasePlayer::VTable)
        {
            static auto hookChain = Valve::getBasePlayerHooks()->spawn();
            hookChain->callChain(
                [instance](nstd::observer_ptr<IBasePlayer>)
                {
                    execVFunc<>(hookChain->getVFuncAddr(), instance);
                },
                Valve::getEntityHolder()->getBasePlayer(static_cast<CBasePlayer *>(instance)));
        }
        else if (getVTable(instance) == CWorldVTable)
        {
            execVFunc<>(CWorldSpawnOrigFn, instance);
            nstd::observer_ptr<CGameRules> gameRules = Valve::gConfig->getAddress<CGameRules>("g_pGameRules");
            gPluginInfo->execHook(SetupHookType::GameRules, gPluginInfo->createGameRules(gameRules));
        }
    }

    /* int TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType); */
    std::int32_t FASTCALL vTakeDamageHook(void *instance,
#if defined _WIN32
                                          void *notUsed,
#endif
                                          entvars_t *pevInflictor,
                                          entvars_t *pevAttacker,
                                          float flDamage,
                                          std::int32_t bitsDamageType)
    {
        if (getVTable(instance) == IBasePlayer::VTable)
        {
            static auto hookChain = Valve::getBasePlayerHooks()->takeDamage();
            return hookChain->callChain(
                [instance](nstd::observer_ptr<IBasePlayer>, nstd::observer_ptr<IBaseEntity> pevInflictor,
                           nstd::observer_ptr<IBaseEntity> pevAttacker, float &flDamage, DmgType bitsDamageType)
                {
                    return execVFunc<int>(hookChain->getVFuncAddr(), instance, static_cast<entvars_t *>(*pevInflictor),
                                          static_cast<entvars_t *>(*pevAttacker), float {flDamage},
                                          static_cast<int>(bitsDamageType));
                },
                Valve::getEntityHolder()->getBasePlayer(static_cast<CBasePlayer *>(instance)),
                Valve::getEntityHolder()->getBaseEntity(pevInflictor),
                Valve::getEntityHolder()->getBaseEntity(pevAttacker), flDamage, static_cast<DmgType>(bitsDamageType));
        }

        return 0;
    }

    /* void TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int
     * bitsDamageType); */
    void FASTCALL vTraceAttack(void *instance,
#if defined _WIN32
                               void *notUsed,
#endif
                               entvars_t *pevAttacker,
                               float flDamage,
                               Vector vecDir,
                               ::TraceResult *ptr,
                               std::int32_t bitsDamageType)
    {
        if (getVTable(instance) == IBasePlayer::VTable)
        {
            std::unique_ptr<Engine::ITraceResult> metaTr = gEngineLib->createTraceResult(ptr);
            static auto hookChain = Valve::getBasePlayerHooks()->traceAttack();

            hookChain->callChain(
                [instance](nstd::observer_ptr<IBasePlayer>, nstd::observer_ptr<IBaseEntity> pevAttacker, float flDamage,
                           float *vec, const std::unique_ptr<Engine::ITraceResult> &metatr, DmgType bitsDamageType)
                {
                    execVFunc<void>(hookChain->getVFuncAddr(), instance, static_cast<entvars_t *>(*pevAttacker),
                                    flDamage, vec, static_cast<TraceResult *>(*metatr),
                                    static_cast<int>(bitsDamageType));
                },
                Valve::getEntityHolder()->getBasePlayer(static_cast<CBasePlayer *>(instance)),
                Valve::getEntityHolder()->getBaseEntity(pevAttacker), flDamage, &vecDir.x, metaTr,
                static_cast<DmgType>(bitsDamageType));
        }
    }

    void FASTCALL vKilled(void *instance,
#if defined _WIN32
                          void *notUsed,
#endif
                          entvars_t *pevAttacker,
                          int iGib)
    {
        if (getVTable(instance) == IBasePlayer::VTable)
        {
            static auto hookChain = Valve::getBasePlayerHooks()->killed();

            hookChain->callChain(
                [instance](nstd::observer_ptr<IBasePlayer>, nstd::observer_ptr<IBaseEntity> pevAttacker, GibType gib)
                {
                    execVFunc<void>(hookChain->getVFuncAddr(), instance, static_cast<entvars_t *>(*pevAttacker),
                                    static_cast<int>(gib));
                },
                Valve::getEntityHolder()->getBasePlayer(static_cast<CBasePlayer *>(instance)),
                Valve::getEntityHolder()->getBaseEntity(pevAttacker), static_cast<GibType>(iGib));
        }
    }
} // namespace Anubis::Game::VFunc