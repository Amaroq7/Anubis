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

#pragma once

#include <ILogger.hpp>
#include <engine/ILibrary.hpp>
#include <IHelpers.hpp>

#include "AnubisConfig.hpp"

#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/ostream.h>

#include <fstream>

namespace Anubis
{
    class Logger final : public ILogger
    {
    public:
        Logger(const std::unique_ptr<Anubis::Config> &config, nstd::observer_ptr<Engine::ILibrary> engineLib);
        Logger(const std::unique_ptr<Anubis::Config> &config,
               std::filesystem::path basePath,
               nstd::observer_ptr<Engine::ILibrary> engineLib);
        ~Logger() final = default;

        void setFilename(std::string_view filename) final;
        void setLogTag(std::string_view logTag) final;
        void setBaseDir(const std::filesystem::path &path) final;
        void logMsg(LogDest logDest, LogLevel level, std::string_view msg) final;
        void setLogLevel(LogLevel logLevel) final;

        template<typename... t_args>
        void logMsg(LogLevel level, LogDest dest, std::string_view format, t_args &&...args)
        {
            if (m_config && level < m_config->getLogLevel())
            {
                return;
            }

            logMsg(dest, level, fmt::vformat(format, fmt::make_format_args(format, args...)));
        }

    private:
        void _sendToConsole(std::string_view msg);
        void _sendToFile(std::string_view msg);

    private:
        nstd::observer_ptr<Engine::ILibrary> m_engineLib;
        const std::unique_ptr<Config> &m_config;
        LogLevel m_logLevel {LogLevel::Info};
        std::string m_filename;
        std::string m_logTag {"UNKNOWN"};
        std::ofstream m_logFile;
        std::filesystem::path m_basePath;
    };
} // namespace Anubis