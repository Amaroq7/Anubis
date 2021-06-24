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

#include <StdFSWrapper.hpp>

#include <functional>

#if defined __linux__
    #include <dlfcn.h>
    #include <unistd.h>
#elif defined _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

namespace Metamod
{
    class Module
    {
    public:
        using SystemHandle = void *;

    public:
        explicit Module(const fs::path &path);

        [[nodiscard]] bool isLoaded() const;
        void unload();
        static std::string_view getError();

        template<typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
        T getSymbol(std::string_view name)
        {
            if (!isLoaded())
            {
                return nullptr;
            }
#if defined __linux__
            return reinterpret_cast<T>(dlsym(m_libHandle.get(), name.data()));
#elif defined _WIN32
            return reinterpret_cast<T>(GetProcAddress(reinterpret_cast<HMODULE>(m_libHandle.get()), name.data()));
#endif
        }

        explicit operator SystemHandle();

    private:
        std::unique_ptr<void, std::function<void(SystemHandle)>> m_libHandle;
    };
}