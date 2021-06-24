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
#include <game/VFuncHelpers.hpp>
#include "MetaExports.hpp"
#include "EntitiesHooks.hpp"
#include "EntityHolder.hpp"
#include <engine/IEntVars.hpp>
#include <engine/ITraceResult.hpp>
#include <game/IBasePlayer.hpp>
#include <type_traits>
#include <extdll.h>
#include <util.h>

namespace
{
    using namespace Metamod;

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<Game::IBaseEntity, T>>>
    T *instanceToType(void *instance)
    {
        static std::uint32_t pevOffset = gVOffsets.at("pev");
        edict_t *ed =
            ENT(*reinterpret_cast<entvars_t **>(reinterpret_cast<std::intptr_t *>(instance) + pevOffset));
        if constexpr (std::is_same_v<T, Game::IBasePlayer>)
        {
            return gEntityHolder.getBasePlayer(gEngineLib->getEdict(ed));
        }

#if defined __linux__
        return nullptr;
#endif
    }

    inline std::intptr_t getVTable(void *instance)
    {
        return *(reinterpret_cast<std::intptr_t *>(instance));
    }
}

namespace Metamod::Game::VFunc
{
    /* virtual void	Spawn( void ) { return; } */
    void vSpawnHook(
#if defined __linux__
        void *instance
#endif
    )
    {
#if defined _WIN32
        void *instance;
        __asm
        {
            mov [instance], ecx
        }
#endif
        if (getVTable(instance) == Game::IBasePlayer::VTable)
        {
            static Game::Valve::BasePlayerSpawnHookRegistry *hookchain = gBasePlayerHooks->spawn();

            hookchain->callChain([instance](IBasePlayer *) {
                VFuncHelpers::execOriginalFunc<>(hookchain->getVFuncAddr(), instance);
            }, instanceToType<IBasePlayer>(instance));
        }
    }

    /*int TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType); */
    std::int32_t vTakeDamageHook(
#if defined __linux__
        void *instance,
#endif
        entvars_t *pevInflictor,
        entvars_t *pevAttacker,
        float flDamage,
        std::int32_t bitsDamageType [[maybe_unused]])
    {
#if defined _WIN32
        void *instance;
        __asm
        {
            mov [instance], ecx
        }
#endif
        if (getVTable(instance) == IBasePlayer::VTable)
        {
            static Game::Valve::BasePlayerTakeDamageHookRegistry *hookchain = gBasePlayerHooks->takeDamage();

            return hookchain->callChain(
                [instance](IBasePlayer *, Engine::IEntVars *pevInflictor,
                           Engine::IEntVars *pevAttacker, float flDamage, std::int32_t bitsDamageType)
                {
                    return VFuncHelpers::execOriginalFunc<int, entvars_t *, entvars_t *, float, int>(
                        hookchain->getVFuncAddr(), instance,
                        static_cast<entvars_t *>(*pevInflictor), static_cast<entvars_t *>(*pevAttacker),
                        flDamage, bitsDamageType);
                },
                instanceToType<IBasePlayer>(instance), gEngineLib->getEntVars(pevInflictor),
                gEngineLib->getEntVars(pevAttacker), flDamage, bitsDamageType);
        }

        return 0;
    }

    /*void TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int
 * bitsDamageType); */
    void vTraceAttack(
#if defined __linux__
        void *instance,
#endif
        entvars_t *pevAttacker,
        float flDamage,
        Vector vecDir,
        ::TraceResult *ptr,
        std::int32_t bitsDamageType)
    {
#if defined _WIN32
        void *instance;
        __asm
        {
            mov [instance], ecx
        }
#endif
        if (getVTable(instance) == IBasePlayer::VTable)
        {
            static Game::Valve::BasePlayerTraceAttackHookRegistry *hookchain = gBasePlayerHooks->traceAttack();
            Engine::ITraceResult *metaTr = gEngineLib->createTraceResult(ptr);

            hookchain->callChain([instance](IBasePlayer *, Engine::IEntVars *pevAttacker, float flDamage, float *vec, Engine::ITraceResult *metatr, std::int32_t bitsDamageType) {
                VFuncHelpers::execOriginalFunc<void, entvars_t *, float, float *, ::TraceResult *, int>(
                     hookchain->getVFuncAddr(), instance, static_cast<entvars_t *>(*pevAttacker), flDamage, vec, *metatr, bitsDamageType
                );
            },
            instanceToType<IBasePlayer>(instance), gEngineLib->getEntVars(pevAttacker),
            flDamage, &vecDir.x, metaTr, bitsDamageType);

            gEngineLib->freeTraceResult(metaTr);
        }
    }

    void vKilled(
#if defined __linux__
        void *instance,
#endif
        entvars_t *pevAttacker,
        int iGib
    )
    {
#if defined _WIN32
        void *instance;
        __asm
        {
            mov [instance], ecx
        }
#endif

        if (getVTable(instance) == IBasePlayer::VTable)
        {
            static Game::Valve::BasePlayerKilledHookRegistry *hookchain = gBasePlayerHooks->killed();

            hookchain->callChain([instance](IBasePlayer *, Engine::IEntVars *pevAttacker, GibType gibType) {
               VFuncHelpers::execOriginalFunc<void, entvars_t *, int>(
                   hookchain->getVFuncAddr(), instance, static_cast<entvars_t *>(*pevAttacker), static_cast<int>(gibType)
               );
            },
            instanceToType<IBasePlayer>(instance), gEngineLib->getEntVars(pevAttacker), static_cast<GibType>(iGib));
        }
    }
}