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

#pragma once

#include "StdFSWrapper.hpp"
#include <string_view>

namespace Metamod 
{
    /**
     * @brief Plugin interface
     *
     * This interface should be implemented by the plugin.
     */
    class IPlugin 
    {
        public:
#if !defined DOXYGEN_SKIP
            virtual ~IPlugin() = default;
#endif
            /**
             * @brief Metamod API interface.
             *
             * It should provide Metamod API interface version used by the plugin.
             *
             * @note Needs to be implemented.
             *
             * @return Metamod API interface
             */
            virtual std::uint32_t getInterfaceVersion() const = 0;

            /**
             * @brief Name of the plugin.
             *
             * It should provide name of the plugin.
             *
             * @note Needs to be implemented.
             *
             * @return Plugin name
             */
            virtual std::string_view getName() const = 0;

            /**
             * @brief Version of the plugin.
             *
             * It should provide version of the plugin.
             *
             * @note Needs to be implemented.
             *
             * @return Plugin version
             */
            virtual std::string_view getVersion() const = 0;

            /**
             * @brief Compilation date of the plugin.
             *
             * It should provide compilation date of the plugin.
             *
             * @note Needs to be implemented.
             *
             * @return Plugin compilation date
             */
            virtual std::string_view getDate() const = 0;

            /**
             * @brief Author of the plugin.
             *
             * It should provide author of the plugin.
             *
             * @note Needs to be implemented.
             *
             * @return Plugin author
             */
            virtual std::string_view getAuthor() const = 0;

            /**
             * @brief Homepage of the plugin.
             *
             * It should provide homepage of the plugin.
             *
             * @note Needs to be implemented.
             *
             * @return Plugin homepage
             */
            virtual std::string_view getUrl() const = 0;

            /**
             * @brief Log tag of the plugin.
             *
             * It should provide logging tag of the plugin.
             *
             * @note Needs to be implemented.
             *
             * @return Plugin log tag
             */
            virtual std::string_view getLogTag() const = 0;

            /**
             * @brief Path of the plugin.
             *
             * Provides path of the plugin.
             *
             * @note The path is provided by metamod. It is already set by the time MetaInit() is called.
             * @note The plugin's path is the same as it appears in the configs.yml file while loading a plugin.
             *
             * @return Path of the plugin
             */
            [[nodiscard]] const fs::path &getPath() const
            {
                return m_path;
            }

#if defined META_CORE
            void setPath(fs::path &&pluginPath)
            {
                m_path = std::move(pluginPath);
            }
#endif

        private:
            fs::path m_path;
    };
}