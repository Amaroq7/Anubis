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

#include "AnubisExports.hpp"
#include "EntitiesHooks.hpp"
#include "EntityHolder.hpp"
#include "VFuncCallbacks.hpp"

#include <engine/IEdict.hpp>
#include <game/valve/BasePlayer.hpp>

#include <yaml-cpp/yaml.h>

nstd::observer_ptr<Anubis::Game::ILibrary> gGameLib;
nstd::observer_ptr<Anubis::Engine::ILibrary> gEngineLib;
nstd::observer_ptr<Anubis::IAnubis> gAnubisAPI;

namespace
{
    std::unordered_map<std::string, std::uint32_t> loadVOffsets()
    {
        std::unordered_map<std::string, std::uint32_t> offsets;
        try
        {
            std::filesystem::path virtualOffsetsPath =
                gAnubisAPI->getPath(Anubis::PathType::Configs) / "gamedata" / "virtual" / "valve" / "offsets.yml";
#if defined __linux__
            YAML::Node rootNode = YAML::LoadFile(virtualOffsetsPath.c_str());
            std::string_view osName("linux");
#elif defined _WIN32
            YAML::Node rootNode = YAML::LoadFile(virtualOffsetsPath.string().c_str());
            std::string_view osName("windows");
#endif
            for (auto funcIt = rootNode.begin(); funcIt != rootNode.end(); ++funcIt)
            {
                offsets.try_emplace(funcIt->first.as<std::string>(), funcIt->second[osName.data()].as<std::uint32_t>());
            }
        }
        catch (const YAML::BadFile &e)
        {
            using namespace std::string_literals;
            throw std::runtime_error("Error parsing yaml vtable offsets file: "s + e.what());
        }

        return offsets;
    }
} // namespace

namespace Anubis
{
    nstd::observer_ptr<IPlugin> Query()
    {
        static std::unique_ptr<IPlugin> pluginInfo = std::make_unique<EntityLib::Valve::Plugin>();
        return pluginInfo;
    }

    bool Init(nstd::observer_ptr<IAnubis> api)
    {
        gAnubisAPI = api;
        gGameLib = api->getGame(Game::ILibrary::VERSION);
        gEngineLib = api->getEngine(Engine::ILibrary::VERSION);

        return true;
    }

    void InstallVHooks()
    {
        auto getVTable = [](std::string_view className)
        {
            const auto &edict = gEngineLib->createEntity(Anubis::FuncCallType::Direct);
            if (!gGameLib->callGameEntity(className, edict))
            {
                gEngineLib->removeEntity(edict, Anubis::FuncCallType::Direct);
                return 0;
            }

            if (!edict->getPrivateData())
            {
                gEngineLib->removeEntity(edict, Anubis::FuncCallType::Direct);
                return 0;
            }

            auto vTable = *(reinterpret_cast<std::intptr_t *>(edict->getPrivateData()));
            gEngineLib->removeEntity(edict, Anubis::FuncCallType::Direct);

            if (!reinterpret_cast<void *>(vTable))
            {
                return 0;
            }

            return vTable;
        };

        Game::IBasePlayer::VTable = getVTable(Game::Valve::BasePlayer::CLASS_NAME);
        if (!Game::IBasePlayer::VTable)
        {
            return;
        }

        auto vOffsets = loadVOffsets();
        Game::VFunc::gPevOffset = vOffsets.at("pev");

        Game::Valve::getEntityHolder();
        Game::Valve::getBasePlayerHooks(std::move(vOffsets));
    }

    void Shutdown() {}

    namespace Game
    {
        nstd::observer_ptr<IBasePlayerHooks> BasePlayerHooks()
        {
            return Valve::getBasePlayerHooks();
        }

        nstd::observer_ptr<IEntityHolder> EntityHolder()
        {
            return Valve::getEntityHolder();
        }
    } // namespace Game
} // namespace Anubis