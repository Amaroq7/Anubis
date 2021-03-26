/*
 *  Copyright (C) 2020-2021 MetamodCPP Development Team
 *
 *  This file is part of MetamodCPP.
 *
 *  MetamodCPP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  MetamodCPP is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with MetamodCPP.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <IMetamod.hpp>

#include <StdFSWrapper.hpp>
#include <array>

namespace Metamod
{
    enum class PathType : std::uint8_t
    {
        Game = 0,
        Metamod = 1,
        Logs = 2,
        Configs = 3
    };

    class Config
    {
    public:
        Config(std::string_view gameDir);

        const fs::path &getPath(PathType path) const;
        void setPath(PathType pathType, fs::path path);
        LogLevel getLogLevel() const;
        void setLogLevel(LogLevel level);

    private:
        fs::path _getMetamodPath();
        void _readConfigFile();

    private:
        std::array<fs::path, 4> m_paths;
        LogLevel m_currentLogLevel = LogLevel::Info;
        std::string m_configFilename = "config.yaml";
    };
}