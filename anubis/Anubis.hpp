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
#include <game/ILibrary.hpp>
#include <engine/Common.hpp>
#include "AnubisConfig.hpp"
#include "Module.hpp"
#include "Msg.hpp"
#include "Logger.hpp"

#include <fmt/format.h>

#include <memory>
#include <vector>

typedef struct globalvars_s globalvars_t;

namespace Anubis
{
    class Anubis final : public IAnubis
    {
    public:
        static constexpr std::string_view LOG_TAG = "ANUBIS";

    public:
        explicit Anubis(std::unique_ptr<Config> &&config);
        ~Anubis() final = default;

        [[nodiscard]] nstd::observer_ptr<Engine::ILibrary> getEngine(InterfaceVersion version) const final;
        [[nodiscard]] nstd::observer_ptr<Game::ILibrary> getGame(InterfaceVersion version) const final;
        [[nodiscard]] std::unique_ptr<ILogger> getLogger(InterfaceVersion version) const final;
        [[nodiscard]] nstd::observer_ptr<IMsg> getMsgInfo(std::string_view name) const final;
        [[nodiscard]] nstd::observer_ptr<IMsg> getMsgInfo(Engine::MsgType msgType) const final;

        [[nodiscard]] nstd::observer_ptr<Engine::ILibrary> getEngine() const;
        [[nodiscard]] nstd::observer_ptr<Game::ILibrary> getGame() const;

        bool addNewMsg(Engine::MsgType id, std::string_view name, Engine::MsgSize size);
        bool setLogLevel(std::string_view logLevel);
        const std::filesystem::path &getPath(PathType pathType) final;
        void loadPlugins();

        void freePluginsResources();

        void initEngine(std::unique_ptr<enginefuncs_t> &&engineFuncs, nstd::observer_ptr<globalvars_t> globals);
        void initLogger();
        void initGameDLL();
        void installVFHooksForPlugins() const;
        [[nodiscard]] const std::unique_ptr<Logger> &getLogger() const;
        void printInfo() const;
        void printPluginList() const;

    private:
        void _initEngineMessages();
        bool _findMessage(Engine::MsgType id);

    private:
        std::unique_ptr<Config> m_config;
        std::unique_ptr<Engine::ILibrary> m_engineLib;
        std::unique_ptr<Logger> m_logger;
        std::unique_ptr<Game::ILibrary> m_gameLib;
        std::vector<std::unique_ptr<Module>> m_plugins;
        std::array<std::unique_ptr<IMsg>, 256> m_regMsgs;
    };
    extern std::unique_ptr<Anubis> gAnubisApi;
} // namespace Anubis
