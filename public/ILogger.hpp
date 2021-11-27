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

#include "Common.hpp"

#include <filesystem>

namespace Anubis
{
    /**
     * @brief Log levels
     *
     * Available log levels for logging messages.
     */
    enum class LogLevel : std::uint8_t
    {
        Debug = 0, /**< Debug log level */
        Info,      /**< Info log level */
        Warning,   /**< Warning log level */
        Error      /**< Error log level */
    };

    /**
     * @brief Log destinations
     *
     * Available destinations for logging a message.
     */
    enum class LogDest : std::uint8_t
    {
        Console = (1 << 0),          /**< Log to console */
        File = (1 << 1),             /**< Log to file */
        ConsoleFile = Console | File /**< Log to console and file */
    };

    class ILogger
    {
    public:
        /**
         * @brief Logger API major version
         */
        static constexpr MajorInterfaceVersion MAJOR_VERSION = MajorInterfaceVersion(1);

        /**
         * @brief Logger API minor version
         */
        static constexpr MinorInterfaceVersion MINOR_VERSION = MinorInterfaceVersion(0);

        /**
         * @brief Logger API version
         *
         * Major version is present in the 16 most significant bits.
         * Minor version is present in the 16 least significant bits.
         *
         * @b Examples
         *
         * To extract the major version of Logger API the following formula can be used
         * @code{cpp}
         * std::uint16_t majorVer = (VERSION >> 16) & 0xFFFF;
         * @endcode
         *
         *
         * To extract the minor version of Logger API the following formula can be used
         * @code{cpp}
         * std::uint16_t minorVer = VERSION & 0xFFFF;
         * @endcode
         */
        static constexpr InterfaceVersion VERSION = InterfaceVersion(MAJOR_VERSION << 16 | MINOR_VERSION);

    public:
        virtual ~ILogger() = default;

        /**
         * @brief Sets file where logs will be stored.
         *
         * @note Filename will have appended the following suffix "<year><month><day>.txt"
         * @note If no filename is set then writing to the file will have no effect.
         *
         * @param filename        Filename.
         *
         */
        virtual void setFilename(std::string_view filename) = 0;

        /**
         * @brief Sets log tag.
         *
         * @note If no log tag is set then default value will be "UNKNOWN".
         *
         * @param logTag        Log tag.
         *
         */
        virtual void setLogTag(std::string_view logTag) = 0;

        /**
         * @brief Sets base dir for storing logs.
         *
         * @note If none is set then base directory will be Anubis default logs dir.
         *
         * @param path        Base directory where logs will be stored.
         *
         */
        virtual void setBaseDir(const std::filesystem::path &path) = 0;

        /**
         * @brief Sends log depending on the destination.
         *
         * @param logDest       Logging destination.
         * @param level         Level of the log.
         * @param msg           Message to log.
         *
         */
        virtual void logMsg(LogDest logDest, LogLevel level, std::string_view msg) = 0;

        /**
         * @brief Sets logs level for the logger.
         *
         * @param logLevel      Log level to set.
         *
         */
        virtual void setLogLevel(LogLevel logLevel) = 0;
    };
} // namespace Anubis