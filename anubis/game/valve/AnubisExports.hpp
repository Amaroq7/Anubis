/*
 *  Copyright (C) 2020 Anubis Development Team
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
#include <AnubisInfo.hpp>
#include <game/ILibrary.hpp>
#include <engine/ILibrary.hpp>
#include <map>

extern nstd::observer_ptr<Anubis::Game::ILibrary> gGameLib;
extern nstd::observer_ptr<Anubis::Engine::ILibrary> gEngineLib;
extern nstd::observer_ptr<Anubis::IAnubis> gAnubisAPI;

namespace EntityLib::Valve
{
    class Plugin : public Anubis::IPlugin
    {
    public:
        ~Plugin() override = default;

        [[nodiscard]] Anubis::InterfaceVersion getInterfaceVersion() const override
        {
            return Anubis::IAnubis::VERSION;
        }

        [[nodiscard]] Type getType() const override
        {
            return Anubis::IPlugin::Type::EntityDLL;
        }

        [[nodiscard]] std::string_view getName() const override
        {
            return "Entity Library Valve";
        }

        [[nodiscard]] std::string_view getVersion() const override
        {
            return ANUBIS_VERSION;
        }

        [[nodiscard]] std::string_view getDate() const override
        {
            return __DATE__;
        }

        [[nodiscard]] std::string_view getAuthor() const override
        {
            return "Anubis Development Team";
        }

        [[nodiscard]] std::string_view getUrl() const override
        {
            return "https://github.com/Amaroq7/anubis";
        }

        void execHook(SetupHookType setupHookType, std::any obj)
        {
            m_hooks.at(setupHookType)(obj);
        }

        void addHook(SetupHookType setupHookType, std::function<void(std::any)> &&hook)
        {
            m_hooks.try_emplace(setupHookType, std::move(hook));
        }

    private:
        std::map<SetupHookType, std::function<void(std::any)>> m_hooks;
    };

    extern std::unique_ptr<Plugin> gPluginInfo;
} // namespace Anubis::Game::Valve