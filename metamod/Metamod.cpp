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

#include "Metamod.hpp"
#include "EngineExports.hpp"
#include "engine/Library.hpp"
#include "game/Library.hpp"
#include <MetaInfo.hpp>
#include <Utils.hpp>
#include <MetaCvars.hpp>

#include <yaml-cpp/yaml.h>
#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/ostream.h>

#include <fstream>

namespace
{
    fmt::text_style getTextStyleForLog(Metamod::LogLevel logLevel)
    {
        switch (logLevel)
        {
        case Metamod::LogLevel::Info:
            return fg(fmt::terminal_color::cyan);
        case Metamod::LogLevel::Warning:
            return fg(fmt::terminal_color::yellow);
        case Metamod::LogLevel::Error:
            return fg(fmt::terminal_color::red);
        default:
            return {};
        }
    }

    std::string_view getPrefixForLog(Metamod::LogLevel logLevel)
    {
        switch (logLevel)
        {
        case Metamod::LogLevel::Info:
            return "INFO";
        case Metamod::LogLevel::Warning:
            return "WARNING";
        case Metamod::LogLevel::Error:
            return "ERROR";
        default:
            return "DEBUG";
        }
    }
}

namespace Metamod
{
    std::unique_ptr<Metamod> gMetaGlobal;

    Metamod::Metamod(std::unique_ptr<Config> &&config)
    : m_config(std::move(config)),
      m_engineLib(std::make_unique<Engine::Library>())
      
    {
        char gameDir[MAX_PATH];
        GET_GAME_DIR(gameDir);
        m_gameLib = std::make_unique<Game::Library>(m_engineLib.get(), gameDir, m_config->getPath(PathType::Configs));

        constexpr std::size_t ENGINE_MSG_NUM = 58;
        for (std::size_t i = 0; i <= ENGINE_MSG_NUM; i++)
        {
            m_regMsgs.at(i).id = static_cast<std::uint8_t>(i);
        }
    }

    std::uint32_t Metamod::getInterfaceVersion() const
    {
        return VERSION;
    }

    Engine::ILibrary *Metamod::getEngine() const
    {
        return m_engineLib.get();
    }

    Game::ILibrary *Metamod::getGame() const
    {
        return m_gameLib.get();
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

    const IMetamod::RegMsg *Metamod::getMsgInfo(std::uint8_t id) const
    {
        for (const auto &msg : m_regMsgs)
        {
            if (msg.id == id)
            {
                return &msg;
            }
        }
        return nullptr;
    }

    bool Metamod::addNewMsg(Engine::MsgType id, std::string_view name, std::int16_t size)
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

    bool Metamod::_findMessage(Engine::MsgType id)
    {
        return !m_regMsgs.at(id).name.empty();
    }

    void Metamod::loadPlugins()
    {
        fs::path metaPluginsFilePath(m_config->getPath(PathType::Configs) / "plugins.yaml");

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

            if (!pluginPath.is_absolute())
            {
                pluginPath = m_config->getPath(PathType::Game) / pluginPath;
            }

            if (!fs::exists(pluginPath))
            {
                logMsg(LogLevel::Error,
                       LogDest::Console | LogDest::File,
                       "Plugin {} not found in {}", pluginsIt->first.as<std::string>(), pluginPath);
                continue;
            }

            auto pluginModule = std::make_unique<Module>(pluginPath);
            if (!pluginModule->isLoaded())
            {
                logMsg(LogLevel::Error, LogDest::Console | LogDest::File, Module::getError());
                continue;
            }

            auto metaQueryFn = pluginModule->getSymbol<fnMetaQuery>("MetaQuery");
            if (!metaQueryFn)
            {
                logMsg(LogLevel::Error,
                      LogDest::Console | LogDest::File,
                      "MetaQuery symbol not found in {}", pluginsIt->first.as<std::string>());
                continue;
            }

            IPlugin *pluginInfo = std::invoke(metaQueryFn);
            if (!pluginInfo)
            {
                logMsg(LogLevel::Error,
                       LogDest::Console | LogDest::File,
                       "No plugin info returned from MetaQuery in {}", pluginsIt->first.as<std::string>());
                continue;
            }

            pluginInfo->setPath(std::move(pluginPath));

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
                logMsg(LogLevel::Error, LogDest::Console | LogDest::File,
                       "Major version mismatch. {} is not compatible with this version of {}"
                       "\n    Requested {} got {}", pluginInfo->getName(), META_NAME,
                       extractMajorVer(pluginInterfaceVersion), extractMajorVer(metaInterfaceVersion));
                continue;
            }
            else if (extractMinorVer(metaInterfaceVersion) < extractMinorVer(pluginInterfaceVersion))
            {
                logMsg(LogLevel::Error,
                       LogDest::Console | LogDest::File,
                       "Minor version mismatch. {} is not compatible with this version of {}"
                       "\n    Requested at least {} got {}", pluginInfo->getName(), META_NAME,
                       extractMinorVer(pluginInterfaceVersion), extractMinorVer(metaInterfaceVersion));
                continue;
            }

            auto metaInitFn = pluginModule->getSymbol<fnMetaInit>("MetaInit");
            if (!metaInitFn || !std::invoke(metaInitFn, this))
            {
                logMsg(LogLevel::Error, LogDest::Console | LogDest::File, "MetaInit symbol not found in {}",
                       pluginInfo->getName());
                continue;
            }

            logMsg(LogLevel::Info, LogDest::Console | LogDest::File, "Plugin {} v{} has been loaded",
                   pluginInfo->getName(), pluginInfo->getVersion());

            m_plugins.emplace_back(std::move(pluginModule));
        }
    }

    void Metamod::logMsg(IPlugin *plugin, LogLevel level, LogDest dest,
                         std::string_view msg)
    {
        if (level < m_config->getLogLevel())
        {
            return;
        }

        auto sendToFileFn = [this](std::string_view logTag, LogLevel level, std::string_view msg) {
            _sendToFile(m_config->getPath(PathType::Logs), logTag, level, msg);
        };

        if ((dest & LogDest::Console) == LogDest::Console)
        {
            _sendToConsole(plugin->getLogTag(), level, msg);
        }
        if ((dest & LogDest::File) == LogDest::File)
        {
            sendToFileFn(plugin->getLogTag(), level, msg);
        }
    }

    void Metamod::_sendToConsole(std::string_view logTag, LogLevel level, std::string_view msg)
    {
        std::invoke(
            g_engfuncs.pfnServerPrint,
            fmt::format(getTextStyleForLog(level), "[{}] {}: {}\n", logTag, getPrefixForLog(level), msg).c_str()
        );
    }

    void Metamod::_sendToFile(const fs::path &logsPath, std::string_view logTag, LogLevel level, std::string_view msg)
    {
        time_t currentTime;
        time(&currentTime);
        tm convertedTime;

        static std::int8_t lastOpenedFileDay = -1;
        static std::ofstream logFile;

#if defined __linux__
        convertedTime = *std::localtime(&currentTime);
#elif defined _WIN32
        localtime_s(&convertedTime, &currentTime);
#endif

        char logDateTime[64], fileName[256];
        std::strftime(logDateTime, sizeof(logDateTime), "%Y/%m/%d - %H:%M:%S: ", &convertedTime);

        // Metamod or new day has started
        if (lastOpenedFileDay == -1 || lastOpenedFileDay != static_cast<int8_t>(convertedTime.tm_mday))
        {
            std::strftime(fileName, sizeof(fileName), "logs_%Y%m%d.log", &convertedTime);
            lastOpenedFileDay = static_cast<std::int8_t>(convertedTime.tm_mday);
            logFile = std::ofstream(logsPath / fileName, std::ios_base::app | std::ios_base::ate);
        }

        fmt::print(logFile, "{} [{}] {}: {}\n", logDateTime, logTag, getPrefixForLog(level), msg);
        logFile.flush();
    }

    bool Metamod::setLogLevel(std::string_view logLevel)
    {
        std::string logLevelLowerCase = Utils::toLowerCopy(logLevel);

        if (logLevelLowerCase == "debug")
        {
            m_config->setLogLevel(LogLevel::Debug);
            return true;
        }
        else if (logLevelLowerCase == "info")
        {
            m_config->setLogLevel(LogLevel::Info);
            return true;
        }
        else if (logLevelLowerCase == "warning")
        {
            m_config->setLogLevel(LogLevel::Warning);
            return true;
        }
        else if (logLevelLowerCase == "error")
        {
            m_config->setLogLevel(LogLevel::Error);
            return true;
        }

        return false;
    }

    fs::path Metamod::getPath(PathType pathType)
    {
        return m_config->getPath(pathType);
    }

    void Metamod::logMsg(LogLevel level, LogDest dest, std::string_view msg)
    {
        logMsg(level, dest, msg, 0);
    }

    void Metamod::freePluginsResources()
    {
        for (const auto &plugin : m_plugins)
        {
            if (auto metaShutdownFn = plugin->getSymbol<fnMetaShutdown>("MetaShutdown"); metaShutdownFn)
            {
                std::invoke(metaShutdownFn);
            }
        }
    }
}
