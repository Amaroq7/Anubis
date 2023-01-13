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

#include "Config.hpp"

#if defined __linux__
    #include <dlfcn.h>
#else
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <Psapi.h>
#endif

namespace Anubis::Game::Valve
{
    std::unique_ptr<Config> gConfig;

    Config::Config(const std::filesystem::path &configPath, nstd::observer_ptr<void> libHandle) : m_libHandle(libHandle)
    {
        _loadVirtualOffsets(configPath);
        _loadOtherAddr(configPath);
    }

    std::uint32_t Config::getVirtualOffset(std::string_view funcName) const
    {
        return m_virtualOffsets.at(funcName.data());
    }

    void Config::_loadVirtualOffsets(const std::filesystem::path &configPath)
    {
        try
        {
            std::filesystem::path virtualOffsetsPath = configPath / "gamedata" / "valve" / "voffsets.yaml";
#if defined __linux__
            YAML::Node rootNode = YAML::LoadFile(virtualOffsetsPath.c_str());
            std::string_view osName("linux");
#elif defined _WIN32
            YAML::Node rootNode = YAML::LoadFile(virtualOffsetsPath.string().c_str());
            std::string_view osName("windows");
#endif
            for (auto funcIt = rootNode.begin(); funcIt != rootNode.end(); ++funcIt)
            {
                m_virtualOffsets.try_emplace(funcIt->first.as<std::string>(),
                                             funcIt->second[osName.data()].as<std::uint32_t>());
            }
        }
        catch (const YAML::BadFile &e)
        {
            using namespace std::string_literals;
            throw std::runtime_error("Error parsing yaml vtable offsets file: "s + e.what());
        }
    }

    void Config::_loadOtherAddr(const std::filesystem::path &configPath)
    {
        try
        {
            std::filesystem::path varsOffsetsPath = configPath / "gamedata" / "valve" / "other.yaml";
#if defined __linux__
            YAML::Node rootNode = YAML::LoadFile(varsOffsetsPath.c_str());
#elif defined _WIN32
            YAML::Node rootNode = YAML::LoadFile(varsOffsetsPath.string().c_str());
#endif
            for (auto typeIt = rootNode.begin(); typeIt != rootNode.end(); ++typeIt)
            {
                std::unordered_map<std::string, nstd::observer_ptr<void>> addresses;
                for (auto name = typeIt->second.begin(); name != typeIt->second.end(); ++name)
                {
#if defined __linux__
                    addresses.try_emplace(name->first.as<std::string>(),
                                          dlsym(m_libHandle.get(), name->second["linux"].as<std::string>().c_str()));
#else
                    bool var = name->second["var"].IsDefined() && name->second["var"].as<bool>();
                    std::uint8_t mask =
                        name->second["mask"].IsDefined() ? name->second["mask"].as<std::uint8_t>() : 0x3f;
                    addresses.try_emplace(
                        name->first.as<std::string>(),
                        _findPattern(name->second["windows"].as<std::vector<std::uint8_t>>(), mask, var));
#endif
                }
                m_other.try_emplace(typeIt->first.as<std::string>(), std::move(addresses));
            }
        }
        catch (const YAML::BadFile &e)
        {
            using namespace std::string_literals;
            throw std::runtime_error("Error parsing yaml other.yaml file: "s + e.what());
        }
    }

#if defined _WIN32
    nstd::observer_ptr<void> Config::_findPattern(std::vector<std::uint8_t> pattern, std::uint8_t mask, bool var)
    {
        MODULEINFO moduleInfo;
        if (GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(m_libHandle.get()), &moduleInfo,
                                 sizeof(moduleInfo)))
        {
            auto bytesBuffer = std::make_unique<std::uint8_t[]>(moduleInfo.SizeOfImage);
            if (ReadProcessMemory(GetCurrentProcess(), moduleInfo.lpBaseOfDll, bytesBuffer.get(),
                                  moduleInfo.SizeOfImage, nullptr))
            {
                auto res = std::search(bytesBuffer.get(), bytesBuffer.get() + moduleInfo.SizeOfImage, pattern.begin(),
                                       pattern.end(),
                                       [mask](std::uint8_t val1, std::uint8_t val2)
                                       {
                                           return (val1 == val2 || val2 == mask);
                                       });

                if (res >= bytesBuffer.get() + moduleInfo.SizeOfImage)
                {
                    return nullptr;
                }

                std::uintptr_t offset =
                    reinterpret_cast<std::uintptr_t>(res) - reinterpret_cast<std::uintptr_t>(bytesBuffer.get());

                void *addr = nullptr;
                if (var)
                {
                    addr =
                        *reinterpret_cast<void **>(reinterpret_cast<std::uint8_t *>(moduleInfo.lpBaseOfDll) + offset);
                }
                else
                {
                    addr = reinterpret_cast<void *>(reinterpret_cast<std::uint8_t *>(moduleInfo.lpBaseOfDll) + offset);
                }

                return addr;
            }
        }
        return nullptr;
    }
#endif
} // namespace Anubis::Game::Valve