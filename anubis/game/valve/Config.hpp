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

#include <observer_ptr.hpp>
#include <game/ILibrary.hpp>

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <unordered_map>
#include <cinttypes>
#include <functional>
#include <optional>

namespace Anubis::Game::Valve
{
    class Config
    {
    public:
        Config(const std::filesystem::path &configPath, nstd::observer_ptr<void> libHandle);

    public:
        std::uint32_t getVirtualOffset(std::string_view funcName) const;
        template<typename T>
        nstd::observer_ptr<T> getAddress(std::string_view name,
                                         std::optional<std::string_view> className = std::nullopt) const
        {
            if (className)
            {
                return nstd::static_observer_cast<T>(m_other.at((*className).data()).at(name.data()));
            }
            return nstd::static_observer_cast<T>(m_other.at("global").at(name.data()));
        }
        template<typename t_ret, typename... t_args>
        std::function<t_ret(t_args...)> getAddressFn(std::string_view name,
                                                     std::optional<std::string_view> className = std::nullopt) const
        {
            if (className)
            {
#if defined __linux__
                return reinterpret_cast<t_ret (*)(t_args...)>(m_other.at((*className).data()).at(name.data()).get());
#else
                return reinterpret_cast<t_ret(__fastcall *)(t_args...)>(
                    m_other.at((*className).data()).at(name.data()).get());
#endif
            }

            return reinterpret_cast<t_ret (*)(t_args...)>(m_other.at("global").at(name.data()).get());
        }

    private:
        void _loadVirtualOffsets(const std::filesystem::path &configPath);
        void _loadOtherAddr(const std::filesystem::path &configPath);
#if defined _WIN32
        nstd::observer_ptr<void> _findPattern(std::vector<std::uint8_t> pattern, std::uint8_t mask, bool var);
#endif

    private:
        nstd::observer_ptr<void> m_libHandle;
        std::unordered_map<std::string, std::uint32_t> m_virtualOffsets;
        std::unordered_map<std::string, std::unordered_map<std::string, nstd::observer_ptr<void>>> m_other;
    };

    extern std::unique_ptr<Config> gConfig;
} // namespace Anubis::Game::Valve