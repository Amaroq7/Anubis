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

#pragma once

#include <game/ILibrary.hpp>
#include "Module.hpp"
#include "Callbacks.hpp"
#include "Hooks.hpp"
#include "Logger.hpp"
#include "IEntityHolder.hpp"

class CBaseEntity;

#include <limits>
#include <array>

namespace Anubis::Game
{
    struct ModInfo
    {
        std::string_view name;         // name (the game dir)
        std::string_view libNameLinux; // filename of shared lib
        std::string_view libNameWin32; // filename of shared lib
        std::string_view libEntityNameLinux;
        std::string_view libEntityNameWin32;
    };

    class Library final : public ILibrary
    {
    public:
        Library() = delete;
        Library(nstd::observer_ptr<Engine::ILibrary> engine,
                std::string_view gameDir,
                const std::unique_ptr<Logger> &logger);

        [[nodiscard]] std::string_view getName() const final;
        [[nodiscard]] std::string_view getDesc() const final;
        [[nodiscard]] const std::filesystem::path &getGameDir() const final;
        [[nodiscard]] const std::filesystem::path &getPathname() const final;
        [[nodiscard]] Mod getMod() const final;
        [[nodiscard]] nstd::observer_ptr<IHooks> getHooks() const final;

        bool callGameEntity(std::string_view name, nstd::observer_ptr<Engine::IEdict> pEntity) final;

        void pfnGameInit(FuncCallType callType) final;
        std::int32_t pfnSpawn(nstd::observer_ptr<Engine::IEdict> pEntity, FuncCallType callType) final;
        bool pfnClientConnect(nstd::observer_ptr<Engine::IEdict> pEntity,
                              std::string_view pszName,
                              std::string_view pszAddress,
                              nstd::observer_ptr<std::string> szRejectReason,
                              FuncCallType callType) final;
        void pfnClientPutInServer(nstd::observer_ptr<Engine::IEdict> pEntity, FuncCallType callType) final;
        void pfnClientCommand(nstd::observer_ptr<Engine::IEdict> pEntity, FuncCallType callType) final;
        void pfnClientUserInfoChanged(nstd::observer_ptr<Engine::IEdict> pEntity,
                                      Engine::InfoBuffer infobuffer,
                                      FuncCallType callType) final;

        void pfnServerActivate(std::uint32_t edictCount, std::uint32_t clientMax, FuncCallType callType) final;
        void pfnServerDeactivate(FuncCallType callType) final;
        void pfnStartFrame(FuncCallType callType) final;
        void pfnGameShutdown(FuncCallType callType) final;
        void pfnCvarValue(nstd::observer_ptr<Engine::IEdict> player,
                          std::string_view value,
                          FuncCallType callType) final;
        void pfnCvarValue2(nstd::observer_ptr<Engine::IEdict> player,
                           std::uint32_t requestID,
                           std::string_view cvarName,
                           std::string_view value,
                           FuncCallType callType) final;
        void pfnClientDisconnect(nstd::observer_ptr<Engine::IEdict> pEntity, FuncCallType callType) final;

        std::unique_ptr<IBaseEntity> getBaseEntity(nstd::observer_ptr<Engine::IEdict> edict) final;
        std::unique_ptr<IBasePlayer> getBasePlayer(nstd::observer_ptr<Engine::IEdict> edict) final;
        nstd::observer_ptr<IBasePlayerHooks> getCBasePlayerHooks() final;
        nstd::observer_ptr<IRules> getRules() const final;
        void initVFuncHooks() final;
        std::unique_ptr<IBaseEntity> getBaseEntity(edict_t *entity) const final;

        const std::unique_ptr<DLL_FUNCTIONS> &getDllFuncs() final;
        const std::unique_ptr<NEW_DLL_FUNCTIONS> &getNewDllFuncs() final;
        [[nodiscard]] Module::SystemHandle getSystemHandle() const final;
        void setMaxClients(std::uint32_t maxClients);
        void setEdictList(edict_t *edictList);
        void freeEntitiesDLL();

    private:
        void _loadGameDLL();
        void _replaceFuncs();
        void _initGameEntityDLL(std::filesystem::path &&path);

    private:
        constexpr static inline std::size_t knownGamesNum = 6;
        constexpr static inline std::array<ModInfo, knownGamesNum> knownGames = {
            ModInfo {       "",       "",        "",                  "",                ""},
            ModInfo {  "valve",  "hl.so",  "hl.dll",   "libvalve_api.so",   "valve_api.dll"},
            ModInfo {"cstrike",  "cs.so",  "mp.dll", "libcstrike_api.so", "cstrike_api.dll"},
            ModInfo {  "czero",  "cs.so",  "mp.dll", "libcstrike_api.so", "cstrike_api.dll"},
            ModInfo {    "dod", "dod.so", "dod.dll",     "libdod_api.so",     "dod_api.dll"},
            ModInfo {    "tfc", "tfc.so", "tfc.dll",     "libtfc_api.so",     "tfc_api.dll"}
        };

        static constexpr const char *engineEntryFuncName = "GiveFnptrsToDll";

        static constexpr inline std::int32_t entityAPIInterfaceVersion = INTERFACE_VERSION;
        static constexpr inline std::int32_t newDLLInterfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
        static constexpr inline const char *newDllApiFuncName = "GetNewDLLFunctions";
        static constexpr inline const char *entityApi2FuncName = "GetEntityAPI2";
        static constexpr inline const char *entityApiFuncName = "GetEntityAPI";

    private:
        std::unique_ptr<Hooks> m_hooks;
        nstd::observer_ptr<Engine::ILibrary> m_engine;
        const std::unique_ptr<Logger> &m_logger;
        std::unique_ptr<DLL_FUNCTIONS> m_dllFunctions;
        std::unique_ptr<NEW_DLL_FUNCTIONS> m_newDllFunctions;
        std::unique_ptr<DLL_FUNCTIONS> m_gameLibDllFunctions;
        std::unique_ptr<NEW_DLL_FUNCTIONS> m_gameLibNewDllFunctions;
        nstd::observer_ptr<IBasePlayerHooks> m_basePlayerHooks;
        nstd::observer_ptr<IEntityHolder> m_entityHolder;
        Mod m_modType = Mod::Unknown;
        std::filesystem::path m_gameDir;
        std::filesystem::path m_pathName;
        std::unique_ptr<Module> m_gameLibrary;
        std::unique_ptr<Module> m_entityLibrary;
        std::uint32_t m_maxClients = 6;
        edict_t *m_edictList = nullptr;
        nstd::observer_ptr<IRules> m_rules;
    };
} // namespace Anubis::Game
