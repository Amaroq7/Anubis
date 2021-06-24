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

#include "Common.hpp"
#include "IHelpers.hpp"
#include "IPlugin.hpp"
#include "IHookChains.hpp"

#if defined META_CORE
    #include <unordered_map>
    #include <game/IEntityHolder.hpp>
#endif

namespace Metamod
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
        Console = (1 << 0),           /**< Log to console */
        File = (1 << 1),              /**< Log to file */
        ConsoleFile = Console | File  /**< Log to console and file */
    };

    /**
     * @brief Path types
     *
     * Allows to retrieve a specific path.
     */
    enum class PathType : std::uint8_t
    {
        Game = 0,       /**< Game directory */
        Metamod = 1,    /**< Metamod directory */
        Logs = 2,       /**< Logs directory */
        Configs = 3,    /**< Configs directory */
        Plugins = 4     /**< Plugins directory */
    };

    namespace Game
    {
        class ILibrary;
        class IBasePlayerHooks;
    }

    namespace Engine
    {
        class ILibrary;
    }

    /**
     * @brief Metamod API interface
     *
     * Metamod API interface is provided by MetaInit().
     */
    class IMetamod
    {
    public:
        /**
         * @brief Metamod API major version
         *
         * Should be used to check if provided Metamod API is compatible.
         */
        static constexpr std::uint16_t MAJOR_VERSION = 0;

        /**
         * @brief Metamod API minor version
         *
         * Should be used to check if provided Metamod API is compatible.
         */
        static constexpr std::uint16_t MINOR_VERSION = 0;

        /**
         * @brief Metamod API version
         *
         * Major version is present in the 16 most significant bits.
         * Minor version is present in the 16 least significant bits.
         *
         * @b Examples
         *
         * To extract the major version of Metamod API the following formula can be used
         * @code{cpp}
         * std::uint16_t majorVer = (VERSION >> 16) & 0xFFFF;
         * @endcode
         *
         *
         * To extract the minor version of Metamod API the following formula can be used
         * @code{cpp}
         * std::uint16_t minorVer = VERSION & 0xFFFF;
         * @endcode
         */
        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

    public:
        /**
         * @brief Info about registered message.
         *
         * Contains information about registered message.
         * Returned by getMsgInfo().
         *
         */
        struct RegMsg
        {
            /**
             * @brief Name
             *
             * Name of the message
             */
            std::string_view name;

            /**
             * @brief Size
             *
             * Size of the message
             */
            std::int16_t size;

            /**
             * @brief Id
             *
             * Id of the message
             */
            std::uint8_t id = 0;
        };

#if !defined DOXYGEN_SKIP
        virtual ~IMetamod() = default;
#endif

        /**
         * @brief Retrieves Metamod API version.
         * @return Metamod API version
         */
        virtual std::uint32_t getInterfaceVersion() const = 0;

        /**
         * @brief Retrieves Engine::ILibrary instance.
         *
         * Allows to retrieve engine instance to access info about engine, its functions and hooks.
         *
         * @return Engine::ILibrary instance
         */
        virtual Engine::ILibrary *getEngine() const = 0;

        /**
         * @brief Retrieves Game::ILibrary instance.
         *
         * Allows to retrieve game instance to access info about game, its functions, hooks and entities.
         *
         * @return Game instance
         */
        virtual Game::ILibrary *getGame() const = 0;

        /**
         * @brief Retrieves info about registered message.
         *
         * Retrieves info about registered message based on its name.
         *
         * @param name Message name
         *
         * @return Registered message info
         */
        virtual const RegMsg *getMsgInfo(std::string_view name) const = 0;

        /**
         * @brief Retrieves info about registered message.
         *
         * Retrieves info about registered message based on its id.
         *
         * @param id Message id
         *
         * @return Registered message info
         */
        virtual const RegMsg *getMsgInfo(std::uint8_t id) const = 0;

        /**
         * @brief Logs a message.
         *
         * @param plugin Plugin struct
         * @param level Log level
         * @param dest Log dest
         * @param msg Message to log
         *
         * @warning @c plugin parameter must not be a null pointer.
         */
        virtual void logMsg(IPlugin *plugin, LogLevel level, LogDest dest, std::string_view msg) = 0;

        /**
         * @brief Retrieves a path.
         *
         * Retrieves specific path based on its parameter.
         *
         * @param pathType Path type
         *
         * @return Path
         */
        virtual fs::path getPath(PathType pathType) = 0;

#if defined META_CORE
        virtual void logMsg(LogLevel level, LogDest dest, std::string_view msg) = 0;
#endif
    };

    using fnMetaQuery = IPlugin *(*)();

    /**
     * @brief Plugin entry function.
     *
     * First function called in the plugin.
     *
     * @warning This function is mandatory.
     *
     * @return Plugin struct
     */
    C_DLLEXPORT IPlugin *MetaQuery();

    using fnMetaInit = bool (*)(IMetamod *api);

    /**
     * @brief Plugin initialization function.
     *
     * Called after MetaQuery().
     *
     * @param api Metamod API interface.
     *
     * @note Plugin should check if the provided Metamod interface version is compatible.
     * @warning This function is mandatory.
     *
     * @return True if Metamod API version is compatible and plugin initialization succeeded, false otherwise.
     */
    C_DLLEXPORT bool MetaInit(IMetamod *api);

    using fnMetaShutdown = void (*)();

    /**
     * @brief Plugin shutdown function.
     *
     * Called when plugin gets unloaded.
     *
     * @note Plugin should free any resources here.
     * @note This function is optional.
     */
    C_DLLEXPORT void MetaShutdown();

#if defined META_CORE
    using fnInitGameEntityDLL = void (*)(IMetamod *api,
                                         const std::unordered_map<std::string, std::uint32_t> &vOffsets);

    C_DLLEXPORT void InitGameEntitiesDLL(IMetamod *api,
                                         const std::unordered_map<std::string, std::uint32_t> &vOffsets);

    using fnGetBasePlayerHooks = Game::IBasePlayerHooks *(*)();
    C_DLLEXPORT Game::IBasePlayerHooks *GetBasePlayerHooks();

    using fnGetEntityHolder = Game::IEntityHolder *(*)();
    C_DLLEXPORT Metamod::Game::IEntityHolder *GetEntityHolder();
#endif
}
