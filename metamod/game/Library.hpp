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

#include <game/ILibrary.hpp>
#include <Module.hpp>
#include "Callbacks.hpp"
#include "Hooks.hpp"

#include <unordered_map>
#include <limits>
#include <array>

namespace Metamod::Engine
{
    class ILibrary;
}

namespace Metamod::Game
{
    class IEntityHolder;

    struct ModInfo
    {
        std::string_view name;      // name (the game dir)
        std::string_view libNameLinux;   // filename of shared lib
        std::string_view libNameWin32;   // filename of shared lib
    };

    class Library final : public ILibrary
    {
    public:
        Library() = delete;
        Library(Engine::ILibrary *engine,
            std::string_view gameDir,
            const fs::path &metaConfigsDir
        );

        std::string_view getName() const override;
        std::string_view getDesc() const override;
        const fs::path &getGameDir() const override;
        const fs::path &getPathname() const override;
        Mod getMod() const final;
        Hooks *getHooks() const override;

        bool callGameEntity(std::string_view name, Engine::IEntVars *pev) override;

        void pfnGameInit(FuncCallType callType) override;
        std::int32_t pfnSpawn(Engine::IEdict *edict, FuncCallType callType) override;
        bool pfnClientConnect(Engine::IEdict *pEntity, std::string_view pszName,
                                      std::string_view pszAddress, std::string &szRejectReason,
                                      FuncCallType callType) override;
        void pfnClientPutInServer(Engine::IEdict *pEntity, FuncCallType callType) override;
        void pfnClientCommand(Engine::IEdict *pEntity, FuncCallType callType) override;
        void pfnClientUserInfoChanged(Engine::IEdict *pEntity, Engine::InfoBuffer infobuffer, FuncCallType callType) override;

        void pfnServerActivate(std::uint32_t edictCount, std::uint32_t clientMax, FuncCallType callType) override;
        void pfnServerDeactivate(FuncCallType callType) override;
        void pfnStartFrame(FuncCallType callType) override;
        void pfnGameShutdown(FuncCallType callType) override;
        void pfnCvarValue(const Engine::IEdict *player,
                          std::uint32_t requestID,
                          std::string_view cvarName,
                          std::string_view value,
                          FuncCallType callType) final;

        IBaseEntity *getBaseEntity(Engine::IEdict *edict) override;
        IBasePlayer *getBasePlayer(Engine::IEdict *edict) override;
        IBasePlayerHooks *getCBasePlayerHooks() override;
        void initVFuncHooks();

        void *getDllFuncs() final;
        void *getNewDllFuncs() final;
        Module::SystemHandle getSystemHandle() const final;
        void setMaxClients(std::uint32_t maxClients);

    private:
        void _loadGameDLL();
        void _replaceFuncs();
        void _loadVOffsets(const fs::path &metaConfigsDir);
        void _initGameEntityDLL(fs::path &&path);

        template <typename t_hookregistry, typename t_origfunc, typename... t_args>
        auto _execGameDLLFunc(t_hookregistry *hookchain, t_origfunc origFunc, FuncCallType callType, t_args... args)
        {
            static_assert(std::is_invocable_v<t_origfunc, t_args...>, "Invalid function passed");
            if (callType == FuncCallType::Direct)
            {
                return std::invoke(origFunc, std::forward<t_args>(args)...);
            }

            return hookchain->callChain(origFunc, std::forward<t_args>(args)...);
        }

    private:
        constexpr static inline std::size_t knownGamesNum = 5;
        constexpr static inline std::array<ModInfo, knownGamesNum> knownGames = {
            ModInfo{ "valve", "hl.so", "hl.dll" },
            ModInfo{ "cstrike", "cs.so", "mp.dll" },
            ModInfo{ "czero", "cs.so", "mp.dll" },
            ModInfo{ "dod", "dod.so", "dod.dll" },
            ModInfo{ "tfc", "tfc.so", "tfc.dll" },

        };

        static constexpr const char *engineEntryFuncName = "GiveFnptrsToDll";

        static constexpr inline std::int32_t entityAPIInterfaceVersion = INTERFACE_VERSION;
        static constexpr inline std::int32_t newDLLInterfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
        static constexpr inline const char *newDllApiFuncName = "GetNewDLLFunctions";
        static constexpr inline const char *entityApi2FuncName = "GetEntityAPI2";
        static constexpr inline const char *entityApiFuncName = "GetEntityAPI";

    private:
        std::unique_ptr<Hooks> m_hooks;
        Engine::ILibrary *m_engine;
        IBasePlayerHooks *m_basePlayerhooks = nullptr;
        IEntityHolder *m_entityHolder = nullptr;
        Mod m_modType;
        fs::path m_gameDir;
        fs::path m_pathName;
        std::unique_ptr<Module> m_gameLibrary;
        std::unique_ptr<Module> m_entityLibrary;
        std::uint32_t m_maxClients = 6;

        DLL_FUNCTIONS m_dllApiTable = {};
        NEW_DLL_FUNCTIONS m_newDllApiTable = {};
        std::unordered_map<std::string, std::uint32_t> m_virtualOffsets;
    };
}
