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

#include <gamelib/IGameLib.hpp>
#include <EngineExports.hpp>
#include <Module.hpp>
#include "Callbacks.hpp"
#include "Hooks.hpp"
#include "Funcs.hpp"

namespace Metamod::GameLib
{
    struct ModInfo
    {
        std::string_view name;      // name (the game dir)
        std::string_view libNameLinux;   // filename of shared lib
        std::string_view libNameWin32;   // filename of shared lib
        std::string_view desc;      // our long-name description
    };

    class GameLib : public IGameLib
    {
    public:
        GameLib() = delete;
        explicit GameLib(std::string_view gameDir);

        std::string_view getName() const override;
        std::string_view getDesc() const override;
        const fs::path &getGameDir() const override;
        const fs::path &getPathname() const override;
        Hooks *getHooks() const override;
        Funcs *getFuncs(bool direct) const override;

        const DLL_FUNCTIONS *getDllFuncs() const;
        const NEW_DLL_FUNCTIONS *getNewDllFuncs() const;
        Module::SystemHandle getSystemHandle() const;

    private:
        void _loadGameDLL();
        void _replaceFuncs();

    private:
        constexpr static inline std::size_t knownGamesNum = 5;
        constexpr static inline std::array<ModInfo, knownGamesNum> knownGames = {
            ModInfo{ "cstrike", "cs.so", "mp.dll", "Counter-Strike 1.6" },
            ModInfo{ "czero", "cs.so", "mp.dll", "Counter-Strike:Condition Zero" },
            ModInfo{ "dod", "dod.so", "dod.dll", "Day of Defeat" },
            ModInfo{ "tfc", "tfc.so", "tfc.dll", "Team Fortress Classic" },
            ModInfo{ "valve", "hl.so", "hl.dll", "Half-Life" }
        };

        static constexpr const char *engineEntryFuncName = "GiveFnptrsToDll";

        static constexpr inline std::int32_t entityAPIInterfaceVersion = INTERFACE_VERSION;
        static constexpr inline std::int32_t newDLLInterfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
        static constexpr const char *newDllApiFuncName = "GetNewDLLFunctions";
        static constexpr const char *entityApi2FuncName = "GetEntityAPI2";
        static constexpr const char *entityApiFuncName = "GetEntityAPI";

    private:
        std::unique_ptr<Hooks> m_hooks;
        std::unique_ptr<Funcs> m_funcs;
        std::unique_ptr<Funcs> m_funcsDirect;
        std::size_t m_modPos = 0;
        fs::path m_gameDir;
        fs::path m_pathName;
        std::unique_ptr<Module> m_gameLibrary;

        DLL_FUNCTIONS m_dllApiTable = {};
        NEW_DLL_FUNCTIONS m_newDllApiTable = {};
    };
}