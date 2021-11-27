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

#include "AnubisConfig.hpp"
#include "Utils.hpp"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace Anubis
{
    Config::Config(std::string_view gameDir)
    {
        setPath(PathType::Game, std::filesystem::current_path() / gameDir);
        std::filesystem::path anubisPath = _getAnubisPath().parent_path().parent_path();

        if (anubisPath.empty())
        {
            throw std::runtime_error("Cannot determine anubis path");
        }

        setPath(PathType::Anubis, anubisPath);
        setPath(PathType::Configs, anubisPath / "configs");
        setPath(PathType::Logs, anubisPath / "logs");
        setPath(PathType::Plugins, anubisPath.parent_path());

        // Create dir for logs in case it doesnt exist
        std::filesystem::create_directory(getPath(PathType::Logs));

        _readConfigFile();
    }

    const std::filesystem::path &Config::getPath(PathType path) const
    {
        return m_paths.at(static_cast<std::underlying_type_t<PathType>>(path));
    }

    void Config::setPath(PathType pathType, std::filesystem::path path)
    {
        m_paths.at(static_cast<std::underlying_type_t<PathType>>(pathType)) = std::move(path);
    }

    LogLevel Config::getLogLevel() const
    {
        return m_currentLogLevel;
    }

    void Config::setLogLevel(LogLevel level)
    {
        m_currentLogLevel = level;
    }

    bool Config::setLogLevel(std::string_view logLevel)
    {
        std::string logLevelLowerCase = Utils::toLowerCopy(logLevel);

        if (logLevelLowerCase == "debug")
        {
            setLogLevel(LogLevel::Debug);
            return true;
        }
        else if (logLevelLowerCase == "info")
        {
            setLogLevel(LogLevel::Info);
            return true;
        }
        else if (logLevelLowerCase == "warning")
        {
            setLogLevel(LogLevel::Warning);
            return true;
        }
        else if (logLevelLowerCase == "error")
        {
            setLogLevel(LogLevel::Error);
            return true;
        }

        return false;
    }

    std::string_view Config::getInitLogLevel() const
    {
        return m_initialLogLevel;
    }

    std::filesystem::path Config::_getAnubisPath() const
    {
        constexpr const char *liblistEntry = "gamedll"
#if defined __linux__
                                             "_linux"
#endif
            ;

        std::ifstream liblistgamFile(getPath(PathType::Game) / "liblist.gam");

        if (!liblistgamFile.is_open())
        {
            throw std::runtime_error("Cannot open liblist.gam file");
        }

        auto trimWhitespaces = [](std::string &str)
        {
            str.erase(str.begin(), std::find_if(str.begin(), str.end(),
                                                [](unsigned char ch)
                                                {
                                                    return !std::isspace(ch);
                                                }));
            str.erase(std::find_if(str.rbegin(), str.rend(),
                                   [](unsigned char ch)
                                   {
                                       return !std::isspace(ch);
                                   })
                          .base(),
                      str.end());
        };

        auto splitLine = [](std::string_view str)
        {
            std::string::size_type spacePos = str.find(' ');
            std::string_view firstPart = str.substr(0ul, spacePos);
            std::string_view secondPart = str.substr(spacePos + 1);

            return std::array<std::string_view, 2>({firstPart, secondPart});
        };

        while (!liblistgamFile.eof())
        {
            std::string line;
            std::getline(liblistgamFile, line);

            trimWhitespaces(line);

            if (line.at(0) == '/' && line.at(1) == '/')
            {
                continue;
            }

            const auto &[firstPart, secondPart] = splitLine(line);

            if (firstPart == liblistEntry)
            {
                // strip quotes
                return getPath(PathType::Game) / secondPart.substr(1, secondPart.length() - 2);
            }
        }

        return {};
    }

    void Config::_readConfigFile()
    {
        YAML::Node rootNode;
        try
        {
#if defined __linux__
            rootNode = YAML::LoadFile((getPath(PathType::Configs) / m_configFilename).c_str());
#elif defined _WIN32
            rootNode = YAML::LoadFile((getPath(PathType::Configs) / m_configFilename).string().c_str());
#endif
        }
        catch (const std::exception &e [[maybe_unused]])
        {
            using namespace std::string_literals;
            throw std::runtime_error("Cannot parse "s + m_configFilename + " file");
        }

        for (auto it = rootNode.begin(); it != rootNode.end(); ++it)
        {
            auto nodeName = it->first.as<std::string>();
            if (nodeName == "logging")
            {
                auto level = it->second["level"].as<std::string>();
                std::transform(level.begin(), level.end(), level.begin(),
                               [](unsigned char c)
                               {
                                   return std::tolower(c);
                               });

                if (level == "debug")
                {
                    setLogLevel(LogLevel::Debug);
                }
                else if (level == "info")
                {
                    setLogLevel(LogLevel::Info);
                }
                else if (level == "warning")
                {
                    setLogLevel(LogLevel::Warning);
                }
                else if (level == "error")
                {
                    setLogLevel(LogLevel::Error);
                }

                m_initialLogLevel = std::move(level);
            }
        }
    }
} // namespace Anubis
