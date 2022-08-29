/*
 *  Copyright (C) 2021 Anubis Development Team
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

#include "Logger.hpp"
#include <utility>

namespace
{
    fmt::text_style getTextStyleForLog(Anubis::LogLevel logLevel)
    {
        switch (logLevel)
        {
            case Anubis::LogLevel::Info:
                return fg(fmt::terminal_color::cyan);
            case Anubis::LogLevel::Warning:
                return fg(fmt::terminal_color::yellow);
            case Anubis::LogLevel::Error:
                return fg(fmt::terminal_color::red);
            default:
                return {};
        }
    }

    std::string_view getPrefixForLog(Anubis::LogLevel logLevel)
    {
        switch (logLevel)
        {
            case Anubis::LogLevel::Info:
                return "INFO";
            case Anubis::LogLevel::Warning:
                return "WARNING";
            case Anubis::LogLevel::Error:
                return "ERROR";
            default:
                return "DEBUG";
        }
    }
} // namespace

namespace Anubis
{
    Logger::Logger(const std::unique_ptr<Anubis::Config> &config, nstd::observer_ptr<Engine::ILibrary> engineLib)
        : m_engineLib(engineLib),
          m_config(config),
          m_basePath(config->getPath(PathType::Logs))
    {
    }

    Logger::Logger(const std::unique_ptr<Anubis::Config> &config,
                   std::filesystem::path basePath,
                   nstd::observer_ptr<Engine::ILibrary> engineLib)
        : m_engineLib(engineLib),
          m_config(config),
          m_basePath(std::move(basePath))
    {
    }

    void Logger::setFilename(std::string_view filename)
    {
        m_filename = filename;
    }

    void Logger::setLogTag(std::string_view logTag)
    {
        m_logTag = logTag;
    }

    void Logger::setBaseDir(const std::filesystem::path &path)
    {
        m_basePath = path;
        if (!std::filesystem::exists(path))
        {
            std::error_code ec;
            std::filesystem::create_directories(path, ec);
            if (ec != std::errc()) {}
        }
    }

    void Logger::setLogLevel(LogLevel logLevel)
    {
        if (!m_config)
        {
            m_logLevel = logLevel;
        }
    }

    void Logger::logMsg(LogDest logDest, LogLevel level, std::string_view msg)
    {
        if ((m_config && level < m_config->getLogLevel()) || (!m_config && level < m_logLevel))
        {
            return;
        }

        if ((logDest & LogDest::Console) == LogDest::Console)
        {
            std::string fmtMsg {
                fmt::format(getTextStyleForLog(level), "[{}] {}: {}\n", m_logTag, getPrefixForLog(level), msg)};
            _sendToConsole(fmtMsg);
        }
        if ((logDest & LogDest::File) == LogDest::File && !m_filename.empty())
        {
            std::string fmtMsg {fmt::format("[{}] {}: {}\n", m_logTag, getPrefixForLog(level), msg)};
            _sendToFile(fmtMsg);
        }
    }

    void Logger::_sendToConsole(std::string_view msg)
    {
        m_engineLib->print(msg, FuncCallType::Direct);
    }

    void Logger::_sendToFile(std::string_view msg)
    {
        time_t currentTime;
        time(&currentTime);
        tm convertedTime = {};

#if defined __linux__
        convertedTime = *std::localtime(&currentTime);
#elif defined _WIN32
        localtime_s(&convertedTime, &currentTime);
#endif
        static char logDateTime[64], fileSuffix[256];
        std::strftime(logDateTime, sizeof(logDateTime), "%Y/%m/%d - %H:%M:%S:", &convertedTime);

        if (!m_logFile.is_open())
        {
            std::strftime(fileSuffix, sizeof(fileSuffix), "_%Y%m%d.txt", &convertedTime);
            m_logFile = std::ofstream(m_basePath / (m_filename + fileSuffix), std::ios_base::app | std::ios_base::ate);

            if (!m_logFile.is_open())
            {
                return;
            }
        }

        fmt::print(m_logFile, "{} {}\n", logDateTime, msg);
        m_logFile.flush();
    }
} // namespace Anubis