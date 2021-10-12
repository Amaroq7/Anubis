/*
 *  Copyright (C) 2020 Metamod++ Development Team
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

#include "MetaExports.hpp"
#include "EntitiesHooks.hpp"
#include "EntityHolder.hpp"

#include <engine/IEdict.hpp>
#include <game/cstrike/BasePlayer.hpp>

#include "ReGameHeaders.hpp"

Metamod::Game::ILibrary *gGameLib;
Metamod::Engine::ILibrary *gEngineLib;
Metamod::IMetamod *gMetamod;
IReGameApi *gReGameAPI;
CSysModule *gGameModule;
//std::unordered_map<std::string, std::uint32_t> gVOffsets;

static bool initReGameDLL_API()
{
    fs::path regameDllPath = gMetamod->getPath(Metamod::PathType::Game) / "dlls";
#if defined __linux__
    regameDllPath /= "cs.so";
#elif defined _WIN32
    regameDllPath /= "mp.dll";
#endif

#if defined __linux__
    gGameModule = Sys_LoadModule(regameDllPath.c_str());
#elif defined _WIN32
    gGameModule = Sys_LoadModule(regameDllPath.string().c_str());
#endif

    CreateInterfaceFn ifaceFactory = Sys_GetFactory(gGameModule);
    if (!ifaceFactory)
    {
        gMetamod->logMsg(Metamod::LogLevel::Error, Metamod::LogDest::File | Metamod::LogDest::Console, "Failed to locate engine module");
        return false;
    }

    auto reGameAPI = reinterpret_cast<IReGameApi *>(ifaceFactory(VRE_GAMEDLL_API_VERSION, nullptr));
    if (!reGameAPI)
    {
        gMetamod->logMsg(Metamod::LogLevel::Error, Metamod::LogDest::File | Metamod::LogDest::Console,"Failed to locate interface factory in regamedll");
        return false;
    }

    std::uint32_t majorVersion = reGameAPI->GetMajorVersion();
    std::uint32_t minorVersion = reGameAPI->GetMinorVersion();

    if (majorVersion != REGAMEDLL_API_VERSION_MAJOR)
    {
        using namespace std::string_literals;
        gMetamod->logMsg(Metamod::LogLevel::Error, Metamod::LogDest::File | Metamod::LogDest::Console,
            "ReGameDLL API major version mismatch. "s +
            "Expected " + std::to_string(REGAMEDLL_API_VERSION_MAJOR) +
            " got " + std::to_string(majorVersion));
        return false;
    }

    if (minorVersion < REGAMEDLL_API_VERSION_MINOR)
    {
        using namespace std::string_literals;
        gMetamod->logMsg(Metamod::LogLevel::Error, Metamod::LogDest::File | Metamod::LogDest::Console,
            "ReGameDLL API minor version mismatch. "s +
            "Expected at least " + std::to_string(REGAMEDLL_API_VERSION_MINOR) +
            " got " + std::to_string(minorVersion));
        return false;
    }

    if (!reGameAPI->BGetICSEntity(CSENTITY_API_INTERFACE_VERSION))
    {
        using namespace std::string_literals;
        gMetamod->logMsg(Metamod::LogLevel::Error, Metamod::LogDest::ConsoleFile,
                         "Interface CCSEntity API version '"s + CSENTITY_API_INTERFACE_VERSION + "' not found");
        return false;
    }

    gReGameAPI = reGameAPI;
    return true;
}

C_DLLEXPORT void InitGameEntitiesDLL(
    Metamod::IMetamod *api,
    const std::unordered_map<std::string, std::uint32_t> &vOffsets [[maybe_unused]])
{
    if (gMetamod)
    {
        return;
    }

    gMetamod = api;
    gGameLib = api->getGame();
    gEngineLib = api->getEngine();
    //gVOffsets = vOffsets;

    /*auto getVTable = [](std::string_view className) {
        Metamod::Engine::IEdict *edict = gEngineLib->createEntity(Metamod::FuncCallType::Direct);
        if (!gGameLib->callGameEntity(className, edict->getEntVars()))
        {
            gEngineLib->removeEntity(edict, Metamod::FuncCallType::Direct);
            return 0;
        }

        if (!edict->getPrivateData())
        {
            gEngineLib->removeEntity(edict, Metamod::FuncCallType::Direct);
            return 0;
        }

        auto vTable = *(reinterpret_cast<std::intptr_t *>(edict->getPrivateData()));
        gEngineLib->removeEntity(edict, Metamod::FuncCallType::Direct);

        if (!reinterpret_cast<void *>(vTable))
        {
            return 0;
        }

        return vTable;
    };*/

    if (!initReGameDLL_API())
    {
        return;
    }

    gBasePlayerHooks = std::make_unique<Metamod::Game::CStrike::BasePlayerHooks>(gReGameAPI->GetHookchains());
}

C_DLLEXPORT Metamod::Game::IBasePlayerHooks *GetBasePlayerHooks()
{
    return gBasePlayerHooks.get();
}

C_DLLEXPORT Metamod::Game::IEntityHolder *GetEntityHolder()
{
    return &gEntityHolder;
}

C_DLLEXPORT void FreeGameEntitiesDLL()
{
    Sys_UnloadModule(gGameModule);
}