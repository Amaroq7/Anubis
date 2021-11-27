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

#pragma once

#include <IAnubis.hpp>

#include <filesystem>
#include <array>

namespace Anubis
{
    class Config
    {
    public:
        explicit Config(std::string_view gameDir);

        [[nodiscard]] const std::filesystem::path &getPath(PathType path) const;
        void setPath(PathType pathType, std::filesystem::path path);
        [[nodiscard]] LogLevel getLogLevel() const;
        void setLogLevel(LogLevel level);
        bool setLogLevel(std::string_view level);
        std::string_view getInitLogLevel() const;

    private:
        [[nodiscard]] std::filesystem::path _getAnubisPath() const;
        void _readConfigFile();

    private:
        std::array<std::filesystem::path, 5> m_paths;
        LogLevel m_currentLogLevel {LogLevel::Info};
        std::string m_configFilename = "config.yaml";
        std::string m_initialLogLevel;
    };
} // namespace Anubis