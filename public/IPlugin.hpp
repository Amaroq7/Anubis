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

#include <filesystem>
#include <string_view>

namespace Anubis
{
    /**
     * @brief Plugin interface
     *
     * This interface should be implemented by the plugin.
     */
    class IPlugin
    {
    public:
        enum class Type : std::uint8_t
        {
            Extension = 0,
            EntityDLL
        };

    public:
#if !defined DOXYGEN_SKIP
        virtual ~IPlugin() = default;
#endif
        /**
         * @brief Anubis API interface.
         *
         * It should provide Anubis API interface version used by the plugin.
         *
         * @note Needs to be implemented.
         *
         * @return Anubis API interface
         */
        [[nodiscard]] virtual InterfaceVersion getInterfaceVersion() const = 0;

        /**
         * @brief Plugin type.
         *
         * It should provide information if plugin should be loaded as an extension or as an entity dll.
         *
         * @note Needs to be implemented.
         *
         * @return Plugin type.
         */
        [[nodiscard]] virtual Type getType() const = 0;

        /**
         * @brief Name of the plugin.
         *
         * It should provide name of the plugin.
         *
         * @note Needs to be implemented.
         *
         * @return Plugin name
         */
        [[nodiscard]] virtual std::string_view getName() const = 0;

        /**
         * @brief Version of the plugin.
         *
         * It should provide version of the plugin.
         *
         * @note Needs to be implemented.
         *
         * @return Plugin version
         */
        [[nodiscard]] virtual std::string_view getVersion() const = 0;

        /**
         * @brief Compilation date of the plugin.
         *
         * It should provide compilation date of the plugin.
         *
         * @note Needs to be implemented.
         *
         * @return Plugin compilation date
         */
        [[nodiscard]] virtual std::string_view getDate() const = 0;

        /**
         * @brief Author of the plugin.
         *
         * It should provide author of the plugin.
         *
         * @note Needs to be implemented.
         *
         * @return Plugin author
         */
        [[nodiscard]] virtual std::string_view getAuthor() const = 0;

        /**
         * @brief Homepage of the plugin.
         *
         * It should provide homepage of the plugin.
         *
         * @note Needs to be implemented.
         *
         * @return Plugin homepage
         */
        [[nodiscard]] virtual std::string_view getUrl() const = 0;

        /**
         * @brief Path of the plugin.
         *
         * Provides path of the plugin.
         *
         * @note The path is provided by anubis. It is already set by the time Init() is called.
         * @note The plugin's path is the same as it appears in the configs.yaml file while loading a plugin.
         *
         * @return Path of the plugin
         */
        [[nodiscard]] const std::filesystem::path &getPath() const
        {
            return m_path;
        }

#if defined ANUBIS_CORE
        void setPath(std::filesystem::path &&pluginPath)
        {
            m_path = std::move(pluginPath);
        }
#endif

    private:
        std::filesystem::path m_path;
    };
} // namespace Anubis