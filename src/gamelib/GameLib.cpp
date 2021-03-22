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

#include <gamelib/GameLib.hpp>
#include <Metamod.hpp>
#include <DllExports.hpp>

namespace Metamod::GameLib
{
    GameLib::GameLib(std::string_view gameDir)
        : m_hooks(std::make_unique<Hooks>()),
          m_funcs(std::make_unique<Funcs>(false)),
          m_funcsDirect(std::make_unique<Funcs>(true))
    {
        for (const ModInfo& modInfo : knownGames)
        {
            if (modInfo.name == gameDir)
            {
                break;
            }
            m_modPos++;
        }

        if (knownGamesNum == m_modPos)
        {
            throw std::runtime_error("GameDLL not recognized");
        }

        m_gameDir = fs::current_path() / knownGames[m_modPos].name;
        m_pathName = fs::current_path() /
                     knownGames[m_modPos].name /
                     "dlls" /
#if defined __linux__
                     knownGames[m_modPos].libNameLinux;
#elif defined _WIN32
                     m_knowGames[m_modPos].libNameWin32;
#endif

        _loadGameDLL();
    }

    std::string_view GameLib::getName() const
    {
        return knownGames[m_modPos].name;
    }

    std::string_view GameLib::getDesc() const
    {
        return knownGames[m_modPos].desc;
    }

    const fs::path &GameLib::getGameDir() const
    {
        return m_gameDir;
    }

    const fs::path &GameLib::getPathname() const
    {
        return m_pathName;
    }

    void GameLib::_loadGameDLL()
    {
        using namespace std::string_literals;

        m_gameLibrary = std::make_unique<Module>(m_pathName);

        if (!m_gameLibrary->isLoaded())
        {
            throw std::runtime_error("Error while loading game dll: "s + Module::getError().data());
        }

        auto fnGiveEngFuncs = m_gameLibrary->getSymbol<Engine::fnGiveFuncs>(engineEntryFuncName);
        if (!fnGiveEngFuncs)
        {
            throw std::runtime_error("Cannot find GiveFnptrsToDll in the game library");
        }
        fnGiveEngFuncs(const_cast<enginefuncs_t *>(gMetaGlobal->getEngine()->getEngineFuncs()), gpGlobals);

        auto fnGetNewDLLFuncs = m_gameLibrary->getSymbol<fnNewDllFunctions>(newDllApiFuncName);
        if (fnGetNewDLLFuncs)
        {
            std::int32_t version = newDLLInterfaceVersion;
            if (!fnGetNewDLLFuncs(&gNewDLLFunctions, &version))
            {
                if (version > newDLLInterfaceVersion)
                {
                    throw std::runtime_error("New DLL API functions not compatible. Metamod outdated.");
                }
                else
                {
                    throw std::runtime_error("New DLL API functions not compatible. GameDLL outdated.");
                }
            }
        }

        auto fnGetEntityAPI2 = m_gameLibrary->getSymbol<fnDllFunctionsv2>(entityApi2FuncName);
        if (fnGetEntityAPI2)
        {
            std::int32_t version = entityAPIInterfaceVersion;
            if (!fnGetEntityAPI2(&gEntityInterface, &version))
            {
                if (version > entityAPIInterfaceVersion)
                {
                    throw std::runtime_error("EntityAPI2 functions not compatible. Metamod outdated.");
                }
                else
                {
                    throw std::runtime_error("EntityAPI2 functions not compatible. GameDLL outdated.");
                }
            }
        }
        else
        {
            auto fnGetEntityAPI = m_gameLibrary->getSymbol<fnDllFunctions>(entityApiFuncName);
            if (fnGetEntityAPI)
            {
                if (!fnGetEntityAPI(&gEntityInterface, entityAPIInterfaceVersion))
                {
                    throw std::runtime_error("Cannot get retrieve EntityAPI functions from GameDLL.");
                }
            }
            else
            {
                throw std::runtime_error("Cannot get retrieve EntityAPI functions from GameDLL.");
            }
        }

        _replaceFuncs();
    }

    void GameLib::_replaceFuncs()
    {
        m_dllApiTable = gEntityInterface;
        m_newDllApiTable = gNewDLLFunctions;

        // Replace only funcs we want to have hooked
#define ASSIGN_ENT_FUNC(func) (m_dllApiTable.func = Callbacks::Engine::func)
        ASSIGN_ENT_FUNC(pfnGameInit);
        ASSIGN_ENT_FUNC(pfnClientConnect);
        ASSIGN_ENT_FUNC(pfnClientPutInServer);
        ASSIGN_ENT_FUNC(pfnClientCommand);
        ASSIGN_ENT_FUNC(pfnClientUserInfoChanged);
        ASSIGN_ENT_FUNC(pfnServerActivate);
        ASSIGN_ENT_FUNC(pfnServerDeactivate);
        ASSIGN_ENT_FUNC(pfnStartFrame);
#undef ASSIGN_ENT_FUNC
#define ASSIGN_NEW_DLL_FUNC(func) (m_newDllApiTable.func = Callbacks::Engine::func)
        ASSIGN_NEW_DLL_FUNC(pfnGameShutdown);
#undef ASSIGN_NEW_DLL_FUNC
    }

    const DLL_FUNCTIONS *GameLib::getDllFuncs() const
    {
        return &m_dllApiTable;
    }

    const NEW_DLL_FUNCTIONS *GameLib::getNewDllFuncs() const
    {
        return &m_newDllApiTable;
    }

    Hooks *GameLib::getHooks() const
    {
        return m_hooks.get();
    }

    Funcs *GameLib::getFuncs(bool direct) const
    {
        return direct ? m_funcsDirect.get() : m_funcs.get();
    }

    Module::SystemHandle GameLib::getSystemHandle() const
    {
        return *m_gameLibrary.get();
    }
}