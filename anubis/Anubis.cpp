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

#include "Anubis.hpp"
#include "engine/Library.hpp"
#include "game/Library.hpp"

#include <AnubisInfo.hpp>

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace
{
    bool isInterfaceCompatible(Anubis::InterfaceVersion reqVersion, Anubis::InterfaceVersion intVersion)
    {
        auto extractMajorVer = [](std::uint32_t version)
        {
            return (version >> 16) & 0xFFFF;
        };

        auto extractMinorVer = [](std::uint32_t version)
        {
            return version & 0xFFFF;
        };

        return extractMajorVer(reqVersion) == extractMajorVer(intVersion) &&
               extractMinorVer(reqVersion) <= extractMinorVer(intVersion);
    }
} // namespace

namespace Anubis
{
    std::unique_ptr<Anubis> gAnubisApi;

    Anubis::Anubis(std::unique_ptr<Config> &&config) : m_config(std::move(config))
    {
        _initEngineMessages();
    }

    nstd::observer_ptr<Engine::ILibrary> Anubis::getEngine(InterfaceVersion version) const
    {
        return isInterfaceCompatible(version, Engine::ILibrary::VERSION) ? getEngine() : nullptr;
    }

    nstd::observer_ptr<Game::ILibrary> Anubis::getGame(InterfaceVersion version) const
    {
        return isInterfaceCompatible(version, Game::ILibrary::VERSION) ? getGame() : nullptr;
    }

    std::unique_ptr<ILogger> Anubis::getLogger(InterfaceVersion version) const
    {
        static std::unique_ptr<Config> empty;
        return isInterfaceCompatible(version, ILogger::VERSION)
                   ? std::make_unique<Logger>(empty, m_config->getPath(PathType::Logs), m_engineLib)
                   : nullptr;
    }

    nstd::observer_ptr<IMsg> Anubis::getMsgInfo(std::string_view name) const
    {
        for (const auto &msg : m_regMsgs)
        {
            if (msg && msg->getName() == name)
            {
                return msg;
            }
        }
        return m_regMsgs.front();
    }

    nstd::observer_ptr<IMsg> Anubis::getMsgInfo(Engine::MsgType msgType) const
    {
        for (const auto &msg : m_regMsgs)
        {
            if (msg && msg->getType() == msgType)
            {
                return msg;
            }
        }

        return m_regMsgs.front();
    }

    bool Anubis::addNewMsg(Engine::MsgType msgType, std::string_view name, Engine::MsgSize size)
    {
        if (_findMessage(msgType))
        {
            return false;
        }

        m_regMsgs[msgType] = std::make_unique<Msg>(name, msgType, size);
        return true;
    }

    bool Anubis::_findMessage(Engine::MsgType msgType)
    {
        return m_regMsgs.at(msgType) != nullptr;
    }

    void Anubis::loadPlugins()
    {
        std::filesystem::path metaPluginsFilePath(m_config->getPath(PathType::Configs) / "plugins.yaml");

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
            throw std::runtime_error("Error parsing yaml plugins file: "s + e.what());
        }

        for (auto pluginsIt = rootNode.begin(); pluginsIt != rootNode.end(); ++pluginsIt)
        {
            if (pluginsIt->second["os"].as<std::string>() != osName)
            {
                continue;
            }

            auto pluginPathStr = pluginsIt->second["path"].as<std::string>();
            std::filesystem::path pluginPath(pluginPathStr);

            if (!pluginPath.is_absolute())
            {
                pluginPath = m_config->getPath(PathType::Game) / pluginPath;
            }

            std::unique_ptr<Module> pluginModule;
            try
            {
                pluginModule = std::make_unique<Module>(IPlugin::Type::Extension, pluginPath);
            }
            catch (const std::runtime_error &e)
            {
                m_logger->logMsg(LogDest::ConsoleFile, LogLevel::Error,
                                 fmt::format("Failed to load {}. Reason: {}", pluginPath.string(), e.what()));
                continue;
            }

            pluginModule->setPath(std::move(pluginPath));
            if (!pluginModule->initPlugin(this))
            {
                m_logger->logMsg(LogLevel::Error, LogDest::ConsoleFile, "Cannot initialize {}.",
                                 pluginModule->getName());
                continue;
            }

            m_logger->logMsg(LogLevel::Info, LogDest::ConsoleFile, "Plugin {} v{} has been loaded",
                             pluginModule->getName(), pluginModule->getVersion());

            m_plugins.emplace_back(std::move(pluginModule));
        }
    }

    bool Anubis::setLogLevel(std::string_view logLevel)
    {
        return m_config->setLogLevel(logLevel);
    }

    const std::filesystem::path &Anubis::getPath(PathType pathType)
    {
        return m_config->getPath(pathType);
    }

    void Anubis::freePluginsResources()
    {
        for (const auto &plugin : m_plugins)
        {
            plugin->deinitPlugin();
        }
    }

    void Anubis::_initEngineMessages()
    {
        static constexpr std::size_t ENGINE_MSG_NUM = 58;
        static const std::array<std::string_view, ENGINE_MSG_NUM + 1> msgNames = {"svc_bad",
                                                                                  "svc_nop",
                                                                                  "svc_disconnect",
                                                                                  "svc_event",
                                                                                  "svc_version",
                                                                                  "svc_setview",
                                                                                  "svc_sound",
                                                                                  "svc_time",
                                                                                  "svc_print",
                                                                                  "svc_stufftext",
                                                                                  "svc_setangle",
                                                                                  "svc_serverinfo",
                                                                                  "svc_lightstyle",
                                                                                  "svc_updateuserinfo",
                                                                                  "svc_deltadescription",
                                                                                  "svc_clientdata",
                                                                                  "svc_stopsound",
                                                                                  "svc_pings",
                                                                                  "svc_particle",
                                                                                  "svc_damage",
                                                                                  "svc_spawnstatic",
                                                                                  "svc_event_reliable",
                                                                                  "svc_spawnbaseline",
                                                                                  "svc_temp_entity",
                                                                                  "svc_setpause",
                                                                                  "svc_signonnum",
                                                                                  "svc_centerprint",
                                                                                  "svc_killedmonster",
                                                                                  "svc_foundsecret",
                                                                                  "svc_spawnstaticsound",
                                                                                  "svc_intermission",
                                                                                  "svc_finale",
                                                                                  "svc_cdtrack",
                                                                                  "svc_restore",
                                                                                  "svc_cutscene",
                                                                                  "svc_weaponanim",
                                                                                  "svc_decalname",
                                                                                  "svc_roomtype",
                                                                                  "svc_addangle",
                                                                                  "svc_newusermsg",
                                                                                  "svc_packetentities",
                                                                                  "svc_deltapacketentities",
                                                                                  "svc_choke",
                                                                                  "svc_resourcelist",
                                                                                  "svc_newmovevars",
                                                                                  "svc_resourcerequest",
                                                                                  "svc_customization",
                                                                                  "svc_crosshairangle",
                                                                                  "svc_soundfade",
                                                                                  "svc_filetxferfailed",
                                                                                  "svc_hltv",
                                                                                  "svc_director",
                                                                                  "svc_voiceinit",
                                                                                  "svc_voicedata",
                                                                                  "svc_sendextrainfo",
                                                                                  "svc_timescale",
                                                                                  "svc_resourcelocation",
                                                                                  "svc_sendcvarvalue",
                                                                                  "svc_sendcvarvalue2"};

        for (std::size_t i = 0; i <= ENGINE_MSG_NUM; i++)
        {
            m_regMsgs[i] =
                std::make_unique<Msg>(msgNames[i], Engine::MsgType(static_cast<Engine::MsgType::BaseType>(i)));
        }
    }

    nstd::observer_ptr<Engine::ILibrary> Anubis::getEngine() const
    {
        return m_engineLib;
    }

    nstd::observer_ptr<Game::ILibrary> Anubis::getGame() const
    {
        return m_gameLib;
    }

    void Anubis::initEngine(std::unique_ptr<enginefuncs_t> &&engineFuncs, nstd::observer_ptr<globalvars_t> globals)
    {
        m_engineLib = std::make_unique<Engine::Library>(std::move(engineFuncs), globals);
    }

    void Anubis::initLogger()
    {
        m_logger = std::make_unique<Logger>(m_config, m_engineLib);
        m_logger->setLogTag("ANUBIS");
        m_logger->setFilename("anubis");
    }

    void Anubis::initGameDLL()
    {
        std::string gameDir = m_engineLib->getGameDir(FuncCallType::Direct);
        m_gameLib = std::make_unique<Game::Library>(m_engineLib, gameDir, m_logger);
    }

    void Anubis::installVFHooksForPlugins() const
    {
        for (const auto &plugin : m_plugins)
        {
            plugin->installVFHooks();
        }
    }

    const std::unique_ptr<Logger> &Anubis::getLogger() const
    {
        return m_logger;
    }

    void Anubis::printInfo() const
    {
        static auto general = fmt::format("{} v{}  {} {}\n", ANUBIS_NAME, ANUBIS_VERSION, __DATE__, __TIME__);
        static auto author = fmt::format("by {}\n", ANUBIS_AUTHOR);
        static auto homepage = fmt::format("   {}\n", ANUBIS_HOMEPAGE);
        static auto api = fmt::format("API: v{}.{}\n", Anubis::IAnubis::MAJOR_VERSION, Anubis::IAnubis::MINOR_VERSION);
        static auto engineApi =
            fmt::format("Engine API: v{}.{}\n", Engine::ILibrary::MAJOR_VERSION, Engine::ILibrary::MINOR_VERSION);
        static auto gameApi =
            fmt::format("GameDll API: v{}.{}\n", Game::ILibrary::MAJOR_VERSION, Game::ILibrary::MINOR_VERSION);

        m_engineLib->print(general, FuncCallType::Direct);
        m_engineLib->print(author, FuncCallType::Direct);
        m_engineLib->print(homepage, FuncCallType::Direct);
        m_engineLib->print(api, FuncCallType::Direct);
        m_engineLib->print(engineApi, FuncCallType::Direct);
        m_engineLib->print(gameApi, FuncCallType::Direct);
    }

    void Anubis::printPluginList() const
    {
        std::size_t i = 0;
        m_engineLib->print("Currently loaded plugins:\n", FuncCallType::Direct);

        for (const auto &plugin : m_plugins)
        {
            auto pluginInfo = fmt::format(fmt::emphasis::bold, "  [{}] Name: {} Version: {}\n", i++, plugin->getName(),
                                          plugin->getVersion());
            m_engineLib->print(pluginInfo, FuncCallType::Direct);
        }
        if (m_plugins.empty())
        {
            fmt::text_style textStyle(fmt::emphasis::bold | fg(fmt::terminal_color::cyan));
            static auto noPluginsMsg = fmt::format(textStyle, "No plugins loaded\n");
            m_engineLib->print(noPluginsMsg, FuncCallType::Direct);
        }
        else
        {
            fmt::text_style textStyle(fmt::emphasis::bold | fg(fmt::terminal_color::cyan));
            static auto pluginsCountMsg = fmt::format(textStyle, "{} plugin{} loaded\n", i, (i > 1) ? "s" : "");
            m_engineLib->print(pluginsCountMsg, FuncCallType::Direct);
        }
    }
} // namespace Anubis
