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

#include <engine/Engine.hpp>
#include <engine/Callbacks.hpp>

#include <extdll.h>
#include <util.h>
#include <memory>

using namespace std::string_literals;

namespace Metamod::Engine
{
    Engine::Engine()
        : m_globals(std::make_unique<Globals>()), m_funcs(std::make_unique<Funcs>()),
          m_funcsDirect(std::make_unique<Funcs>(true)), m_hooks(std::make_unique<Hooks>())
    {
        _replaceFuncs();
        _initReHLDSAPI();
    }

    Edict *Engine::getEdict(std::uint32_t index)
    {
        try
        {
            return m_edicts.at(index).get();
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            // check if edict is valid and register it
            if (edict_t *edict = INDEXENT(index); !FNullEnt(edict) || !index)
            {
                return (m_edicts.at(index) = std::make_unique<Edict>(edict, m_maxClients)).get();
            }
            return nullptr;
        }
    }

    Edict *Engine::getEdict(edict_t *edict)
    {
        try
        {
            return m_edicts.at(ENTINDEX(edict)).get();
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            // check if edict is valid and register it
            if (!FNullEnt(edict) || !ENTINDEX(edict))
            {
                return (m_edicts.at(ENTINDEX(edict)) = std::make_unique<Edict>(edict, m_maxClients)).get();
            }
            return nullptr;
        }
    }

    Globals *Engine::getGlobals() const
    {
        return m_globals.get();
    }

    Funcs *Engine::getFuncs(bool direct) const
    {
        return direct ? m_funcsDirect.get() : m_funcs.get();
    }

    TraceResult *Engine::createTraceResult()
    {
        return m_traceResults.emplace_front().get();
    }

    EntVars *Engine::getEntVars(entvars_t *vars)
    {
        try
        {
            return m_edicts.at(ENTINDEX(ENT(vars)))->getEntVars();
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            // check if edict is valid and register it
            if (!FNullEnt(vars) || !OFFSET(vars))
            {
                edict_t *edict = ENT(vars);
                return (m_edicts.at(ENTINDEX(edict)) = std::make_unique<Edict>(edict, m_maxClients))->getEntVars();
            }
            return nullptr;
        }
    }

    TraceResult *Engine::createTraceResult(::TraceResult *tr)
    {
        return m_traceResults.emplace_front(std::make_unique<TraceResult>(tr)).get();
    }

    void Engine::freeTraceResult(ITraceResult *tr)
    {
        m_traceResults.remove_if([tr](const std::unique_ptr<TraceResult> &ptr) {
            return ptr.get() == tr;
        });
    }

    void Engine::clear(bool uninstallHooks)
    {
        m_traceResults.clear();

        if (uninstallHooks)
        {
            _uninstallHooks();
        }
    }

    void Engine::_installHooks()
    {
        m_reHookchains->SV_DropClient()->registerHook(SV_DropClientHook);
        m_reHookchains->Cvar_DirectSet()->registerHook(Cvar_DirectSetHook);
    }

    void Engine::_uninstallHooks()
    {
        m_reHookchains->SV_DropClient()->unregisterHook(SV_DropClientHook);
        m_reHookchains->Cvar_DirectSet()->unregisterHook(Cvar_DirectSetHook);
    }

    void Engine::SV_DropClientHook(IRehldsHook_SV_DropClient *chain, IGameClient *client, bool crash, const char *string)
    {
        chain->callNext(client, crash, string);
    }

    void Engine::Cvar_DirectSetHook(IRehldsHook_Cvar_DirectSet *chain, cvar_t *cvar, const char *value)
    {
        chain->callNext(cvar, value);
    }

    void Engine::_initReHLDSAPI()
    {
        CSysModule *engineModule = Sys_LoadModule(ENGINE_LIB);
        if (!engineModule)
        {
            throw ReHLDSAPINotFoundException("Failed to locate engine module");
        }

        CreateInterfaceFn ifaceFactory = Sys_GetFactory(engineModule);
        if (!ifaceFactory)
        {
            throw ReHLDSAPINotFoundException("Failed to locate interface factory in engine module");
        }

        std::int32_t retCode = 0;
        auto reHLDSApi = reinterpret_cast<IRehldsApi *>(ifaceFactory(VREHLDS_HLDS_API_VERSION, &retCode));
        if (!reHLDSApi)
        {
            throw ReHLDSAPICouldNotRetrieveException(
                "Failed to retrieve ReHLDS API interface",
                retCode
            );
        }

        auto majorVersion = reHLDSApi->GetMajorVersion();
        auto minorVersion = reHLDSApi->GetMinorVersion();

        if (majorVersion != REHLDS_API_VERSION_MAJOR)
        {
            throw ReHLDSAPIExceptionVerMismatch(
                "ReHLDS API major version mismatch",
                REHLDS_API_VERSION_MAJOR,
                majorVersion,
                REHLDS_API_VERSION_MINOR,
                minorVersion
            );
        }

        if (minorVersion < REHLDS_API_VERSION_MINOR)
        {
            throw ReHLDSAPIExceptionVerMismatch(
                "ReHLDS API minor version mismatch",
                REHLDS_API_VERSION_MAJOR,
                majorVersion,
                REHLDS_API_VERSION_MINOR,
                minorVersion
            );
        }

        _installHooks();
        m_reFuncs = std::make_unique<ReFuncs>(reHLDSApi->GetFuncs());
        m_reHookchains = reHLDSApi->GetHookchains();
    }

    Hooks *Engine::getHooks()
    {
        return m_hooks.get();
    }

    void Engine::_replaceFuncs()
    {
        m_engineFuncs = g_engfuncs;

        // Replace only funcs we want to have hooked
#define ASSIGN_ENG_FUNCS(func) (m_engineFuncs.func = Callbacks::GameDLL::func)
        ASSIGN_ENG_FUNCS(pfnChangeLevel);
        ASSIGN_ENG_FUNCS(pfnMessageBegin);
        ASSIGN_ENG_FUNCS(pfnMessageEnd);
        ASSIGN_ENG_FUNCS(pfnWriteByte);
        ASSIGN_ENG_FUNCS(pfnWriteChar);
        ASSIGN_ENG_FUNCS(pfnWriteShort);
        ASSIGN_ENG_FUNCS(pfnWriteLong);
        ASSIGN_ENG_FUNCS(pfnWriteAngle);
        ASSIGN_ENG_FUNCS(pfnWriteCoord);
        ASSIGN_ENG_FUNCS(pfnWriteString);
        ASSIGN_ENG_FUNCS(pfnWriteEntity);
        ASSIGN_ENG_FUNCS(pfnRegUserMsg);
#undef ASSIGN_ENG_FUNCS
    }

    const enginefuncs_t *Engine::getEngineFuncs() const
    {
        return &m_engineFuncs;
    }

    void Engine::initializeEdicts(edict_t *pEdictList, std::uint32_t edictsNum, std::uint32_t clientMax)
    {
        m_maxClients = clientMax;
        m_edictList = pEdictList;
        for (std::size_t i = 0; i < edictsNum; i++)
        {
            edict_t *edict = pEdictList + i;

            if (edict->free || !edict->pvPrivateData)
            {
                continue;
            }

            m_edicts.at(i) = std::make_unique<Edict>(edict, m_maxClients);
        }
    }

    edict_t *Engine::getEngineEdictList() const
    {
        return m_edictList;
    }

    ReFuncs *Engine::getReHLDSFuncs()
    {
        return m_reFuncs.get();
    }

} // namespace Metamod::Engine