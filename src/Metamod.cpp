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

#include <Metamod.hpp>
#include <gamelib/GameLib.hpp>
#include <EngineExports.hpp>
#include <enginecallback.h>

#include <yaml-cpp/yaml.h>

namespace Metamod
{
    std::unique_ptr<Metamod> gMetaGlobal;

    Metamod::Metamod() : m_engineLib(std::make_unique<Engine::Engine>())
    {
        char gameDir[MAX_PATH];
        GET_GAME_DIR(gameDir);
        m_gameLib = std::make_unique<GameLib::GameLib>(gameDir);

        std::string_view gameDirsv(gameDir);
        if (gameDirsv == "valve")
        {
            m_gameType = GameType::Valve;
        }
        else if (gameDirsv == "cstrike")
        {
            m_gameType = GameType::CStrike;
        }
        else if (gameDirsv == "czero")
        {
            m_gameType = GameType::CZero;
        }

        constexpr std::size_t ENGINE_MSG_NUM = 58;
        for (std::size_t i = 0; i <= ENGINE_MSG_NUM; i++)
        {
            m_regMsgs.at(i).id = i;
        }

        m_engineLib->getReHLDSFuncs()->addExtDll(m_gameLib->getSystemHandle());

        _loadPlugins(gameDir);
    }

    std::uint32_t Metamod::getInterfaceVersion() const
    {
        return VERSION;
    }

    Engine::Engine *Metamod::getEngine() const
    {
        return m_engineLib.get();
    }

    GameLib::GameLib *Metamod::getGameLib() const
    {
        return m_gameLib.get();
    }

    GameType Metamod::getGameType() const
    {
        return m_gameType;
    }

    const Metamod::RegMsg *Metamod::getMsgInfo(std::string_view name) const
    {
        for (const auto &msg : m_regMsgs)
        {
            if (msg.name == name)
            {
                return &msg;
            }
        }
        return nullptr;
    }

    bool Metamod::addNewMsg(std::uint8_t id, std::string_view name, std::int16_t size)
    {
        if (_findMessage(id))
        {
            return false;
        }

        RegMsg &msg = m_regMsgs.at(id);

        msg.name = name;
        msg.size = size;
        msg.id = id;

        return true;
    }

    bool Metamod::_findMessage(std::uint8_t id)
    {
        return !m_regMsgs.at(id).name.empty();
    }

    void Metamod::_loadPlugins(std::string_view gameDir)
    {
        fs::path basePath(fs::current_path() / gameDir);
        fs::path metaPluginsFilePath(basePath / "addons" / "metamod" / "configs" / "plugins.yaml");

        YAML::Node rootNode;
#if defined __linux__
        std::string_view osName("linux");
#elif defined _WIN32
        std::string_view osName("windows");
#endif
        try
        {
#if defined __linux__
            rootNode = YAML::LoadFile(metaPluginsFilePath.c_str());
#elif defined _WIN32
            rootNode = YAML::LoadFile(metaPluginsFilePath.string().c_str());
#endif
        }
        catch (const std::exception &e)
        {
            using namespace std::string_literals;
            throw std::runtime_error("Error parsing yaml vtable offsets file: "s + e.what());
        }

        for (auto pluginsIt = rootNode.begin(); pluginsIt != rootNode.end(); ++pluginsIt)
        {
            if (pluginsIt->second["os"].as<std::string>() != osName)
            {
                continue;
            }

            std::string pluginPathStr = pluginsIt->second["path"].as<std::string>();
            fs::path pluginPath(pluginPathStr);
            std::unique_ptr<Module> pluginModule;

            if (pluginPath.is_absolute())
            {
                pluginModule = std::make_unique<Module>(pluginPath);
            }
            else
            {
                pluginModule = std::make_unique<Module>(basePath / pluginPath);
            }

            if (!pluginModule->isLoaded())
            {
                // TODO: Error msg
                continue;
            }

            auto metaQueryFn = pluginModule->getSymbol<fnMetaQuery>("MetaQuery");
            if (!metaQueryFn)
            {
                // TODO: Error msg
                continue;
            }

            IPlugin *pluginInfo = std::invoke(metaQueryFn);
            if (!pluginInfo)
            {
                // TODO: Error msg
                continue;
            }

            std::uint32_t metaInterfaceVersion = getInterfaceVersion();
            std::uint32_t pluginInterfaceVersion = pluginInfo->getInterfaceVersion();

            auto extractMajorVer = [](std::uint32_t version)
            {
                return (version >> 16) & 0xFFFF;
            };

            auto extractMinorVer = [](std::uint32_t version)
            {
                return version & 0xFFFF;
            };

            if (extractMajorVer(metaInterfaceVersion) != extractMajorVer(pluginInterfaceVersion))
            {
                // TODO: Error message
                continue;
            }
            else if (extractMinorVer(metaInterfaceVersion) < extractMinorVer(pluginInterfaceVersion))
            {
                // TODO: Error message
                continue;
            }

            auto metaInitFn = pluginModule->getSymbol<fnMetaInit>("MetaInit");
            if (!metaInitFn || !std::invoke(metaInitFn, gMetaGlobal.get()))
            {
                // TODO: Error msg
                continue;
            }

            m_plugins.emplace_back(std::move(pluginModule));
        }
    }
}