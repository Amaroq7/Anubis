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

#pragma once

#include <engine/IEngine.hpp>

#include "Edict.hpp"
#include "Funcs.hpp"
#include "Globals.hpp"
#include "TraceResult.hpp"
#include "Hooks.hpp"
#include <osconfig.h>
#include <usercmd.h>
#include <rehlds_api.h>
#include <engine_hlds_api.h>
#include "ReFuncs.hpp"

namespace Metamod::Engine
{
    class ReHLDSAPIException : public std::runtime_error
    {
    public:
        ReHLDSAPIException(std::string_view msg) : std::runtime_error(msg.data()) {}
    };

    class ReHLDSAPINotFoundException : public ReHLDSAPIException
    {
    public:
        ReHLDSAPINotFoundException(std::string_view msg)
            : ReHLDSAPIException(msg)
        {}
    };

    class ReHLDSAPICouldNotRetrieveException : public ReHLDSAPIException
    {
    public:
        ReHLDSAPICouldNotRetrieveException(std::string_view msg, std::int32_t code)
        : ReHLDSAPIException(msg), m_code(code)
        {}

        std::int32_t getCode() const
        {
            return m_code;
        }

    private:
        std::int32_t m_code = 0;
    };

    class ReHLDSAPIExceptionVerMismatch : public ReHLDSAPIException
    {
    public:
        ReHLDSAPIExceptionVerMismatch(
            std::string_view msg,
            std::uint32_t majorVerExpected,
            std::uint32_t majorVerGot,
            std::uint32_t minorVerExpected,
            std::uint32_t minorVerGot
        ) : ReHLDSAPIException(msg), m_majorVerExpected(majorVerExpected),
            m_majorVerGot(majorVerGot), m_minorVerExpected(minorVerExpected),
            m_minorVerGot(minorVerGot)
        {}

        std::uint32_t getMajorExpected() const
        {
            return m_majorVerExpected;
        }

        std::uint32_t getMajorGot() const
        {
            return m_majorVerGot;
        }

        std::uint32_t getMinorExpected() const
        {
            return m_minorVerExpected;
        }

        std::uint32_t getMinorVerGot() const
        {
            return m_minorVerGot;
        }
    private:
        std::uint32_t m_majorVerExpected = 0;
        std::uint32_t m_majorVerGot = 0;
        std::uint32_t m_minorVerExpected = 0;
        std::uint32_t m_minorVerGot = 0;
    };

    class Engine final : public IEngine
    {
    public:
        Engine();
        Engine(const Engine &other) = delete;
        Engine(Engine &&other) = delete;
        ~Engine() override = default;

        // IEngine
        Edict *getEdict(std::uint32_t index) override;
        Globals *getGlobals() const override;
        Funcs *getFuncs(bool direct) const override;
        TraceResult *createTraceResult() override;
        void freeTraceResult(ITraceResult *tr) override;
        Hooks *getHooks() override;
        Edict *getEdict(edict_t *edict);
        EntVars *getEntVars(entvars_t *vars);
        TraceResult *createTraceResult(::TraceResult *tr);
        ReFuncs *getReHLDSFuncs() override;

        const enginefuncs_t *getEngineFuncs() const;
        void clear(bool uninstallHooks = false);
        void initializeEdicts(edict_t *pEdictList, std::uint32_t edictCount, std::uint32_t clientMax);
        edict_t *getEngineEdictList() const;

    private:
        void _replaceFuncs();
        void _initReHLDSAPI();
        void _installHooks();
        void _uninstallHooks();
        static void SV_DropClientHook(IRehldsHook_SV_DropClient *chain, IGameClient *client, bool crash, const char *string);
        static void Cvar_DirectSetHook(IRehldsHook_Cvar_DirectSet *chain, cvar_t *cvar, const char *value);

    private:
        std::unique_ptr<Globals> m_globals;
        std::unique_ptr<Funcs> m_funcs;
        std::unique_ptr<Funcs> m_funcsDirect;
        std::unique_ptr<Hooks> m_hooks;
        std::unique_ptr<ReFuncs> m_reFuncs;
        IRehldsHookchains *m_reHookchains;
        std::array<std::unique_ptr<Edict>, MAX_EDICTS> m_edicts;
        std::forward_list<std::unique_ptr<TraceResult>> m_traceResults;
        enginefuncs_t m_engineFuncs;
        std::uint32_t m_maxClients = 1;
        edict_t *m_edictList;
    };
} // namespace Metamod::Engine