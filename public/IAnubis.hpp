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

#include "observer_ptr.hpp"
#include "Common.hpp"
#include "IHelpers.hpp"
#include "IPlugin.hpp"
#include "IHookChains.hpp"
#include "IMsg.hpp"
#include "ILogger.hpp"

#include <filesystem>
#include <any>

#if defined ANUBIS_CORE
typedef struct enginefuncs_s enginefuncs_t;
typedef struct globalvars_s globalvars_t;
#endif

namespace Anubis
{
    /**
     * @brief Path types
     *
     * Allows to retrieve a specific path.
     */
    enum class PathType : std::uint8_t
    {
        Game = 0,    /**< Game directory */
        Anubis = 1,  /**< Anubis directory */
        Logs = 2,    /**< Logs directory */
        Configs = 3, /**< Configs directory */
        Plugins = 4  /**< Plugins directory */
    };

    namespace Game
    {
        class ILibrary;
        class IBasePlayerHooks;

        enum class SetupHookType : std::uint8_t
        {
            BasePlayerHooks = 0,
            EntityHolder,
            GameRules
        };

    } // namespace Game

    namespace Engine
    {
        class ILibrary;
    }

    /**
     * @brief Anubis API interface
     *
     * Anubis API interface is provided by Anubis::Init().
     */
    class IAnubis
    {
    public:
        /**
         * @brief Anubis API major version
         */
        static constexpr MajorInterfaceVersion MAJOR_VERSION = MajorInterfaceVersion(2);

        /**
         * @brief Anubis API minor version
         */
        static constexpr MinorInterfaceVersion MINOR_VERSION = MinorInterfaceVersion(0);

        /**
         * @brief Anubis API version
         *
         * Major version is present in the 16 most significant bits.
         * Minor version is present in the 16 least significant bits.
         *
         * @b Examples
         *
         * To extract the major version of Anubis API the following formula can be used
         * @code{cpp}
         * std::uint16_t majorVer = (VERSION >> 16) & 0xFFFF;
         * @endcode
         *
         *
         * To extract the minor version of Anubis API the following formula can be used
         * @code{cpp}
         * std::uint16_t minorVer = VERSION & 0xFFFF;
         * @endcode
         */
        static constexpr InterfaceVersion VERSION = InterfaceVersion(MAJOR_VERSION << 16 | MINOR_VERSION);

        virtual ~IAnubis() = default;

        /**
         * @brief Retrieves Engine::ILibrary instance.
         *
         * Allows to retrieve engine instance to access info about engine, its functions and hooks.
         *
         * @return Engine::ILibrary instance
         */
        [[nodiscard]] virtual nstd::observer_ptr<Engine::ILibrary> getEngine(InterfaceVersion version) const = 0;

        /**
         * @brief Retrieves Game::ILibrary instance.
         *
         * Allows to retrieve game instance to access info about game, its functions, hooks and entities.
         *
         * @return Game::ILibrary instance
         */
        [[nodiscard]] virtual nstd::observer_ptr<Game::ILibrary> getGame(InterfaceVersion version) const = 0;

        /**
         * @brief Retrieves ILogger instance.
         *
         * @return ILogger instance
         */
        [[nodiscard]] virtual std::unique_ptr<ILogger> getLogger(InterfaceVersion version) const = 0;

        /**
         * @brief Retrieves info about registered message.
         *
         * Retrieves info about registered message based on its name.
         *
         * @param name Message name
         *
         * @return Registered message info
         */
        [[nodiscard]] virtual nstd::observer_ptr<IMsg> getMsgInfo(std::string_view name) const = 0;

        /**
         * @brief Retrieves info about registered message.
         *
         * Retrieves info about registered message based on its id.
         *
         * @param msgType Message type
         *
         * @return Registered message info
         */
        [[nodiscard]] virtual nstd::observer_ptr<IMsg> getMsgInfo(Engine::MsgType msgType) const = 0;

        /**
         * @brief Retrieves a path.
         *
         * Retrieves specific path based on its parameter.
         *
         * @param pathType Path type
         *
         * @return Path
         */
        virtual const std::filesystem::path &getPath(PathType pathType) = 0;
    };
#if !defined ANUBIS_CORE
    /**
     * @brief Plugin entry function.
     *
     * First function called in the plugin.
     *
     * @warning This function is mandatory.
     *
     * @return Plugin's data
     */
    ANUBIS_API nstd::observer_ptr<IPlugin> Query();

    /**
     * @brief Plugin initialization function.
     *
     * Called after AnubisQuery().
     *
     * @param api Anubis API interface.
     *
     * @warning This function is mandatory.
     *
     * @return True if plugin initialization succeeded, false otherwise.
     */
    ANUBIS_API bool Init(nstd::observer_ptr<IAnubis> api);

    /**
     * @brief Plugin shutdown function.
     *
     * Called when plugin gets unloaded.
     *
     * @note Plugin should free any resources here.
     * @warning This function is mandatory.
     */
    ANUBIS_API void Shutdown();

    /**
     * @brief Plugin function for installing hooks for virtual functions of entities.
     *
     * Called on the first call of the spawn function.
     *
     * @note This function is only mandatory for entity DLL, otherwise it is optional.
     */
    ANUBIS_API void InstallVHooks();

    #if defined ANUBIS_ENTITY_DLL
    namespace Game
    {
        /**
         * @brief Setup callback for object when it is ready.
         *
         * @warning This function is mandatory if plugin is loaded as entity dll.
         *
         */
        ANUBIS_API void SetupHook(SetupHookType setupHookType, std::function<void(std::any)> hook);
    } // namespace Game
    #endif
#else
    /**
     * @brief Signature of Anubis::Query().
     */
    using fnQuery = nstd::observer_ptr<IPlugin> (*)();

    /**
     * @brief Signature of Anubis::Init().
     */
    using fnInit = bool (*)(nstd::observer_ptr<IAnubis> api);

    /**
     * @brief Signature of Anubis::Shutdown().
     */
    using fnShutdown = void (*)();

    /**
     * @brief Signature of Anubis::InstallVHooks().
     */
    using fnInstallVHooks = void (*)();

    /**
     * @brief Signature of Anubis::Game::SetupHook().
     */
    using fnSetupHook = void (*)(Game::SetupHookType setupHookType, std::function<void(std::any)> hook);
#endif
} // namespace Anubis
