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

#if defined _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif defined __linux__
    #include <sys/mman.h>
    #include <unistd.h>
#endif

#include <type_traits>

namespace Metamod::Game
{
    enum class vFuncType : std::uint16_t
    {
        Spawn = 0,
        TakeDamage,
        TraceAttack
    };
}

namespace Metamod::Game::VFuncHelpers
{
    template<typename t_ret = void, typename... t_args>
    t_ret execOriginalFunc(std::intptr_t origFunc, void *instance, t_args... args)
    {
        if constexpr (std::is_same_v<t_ret, void>)
        {
#if defined __linux__
            reinterpret_cast<t_ret (*)(void *, t_args...)>(origFunc)(instance, args...);
#elif defined _WIN32
            reinterpret_cast<t_ret(__fastcall *)(void *, int, t_args...)>(origFunc)(instance, 0, args...);
#endif
        }
        else
        {
#if defined __linux__
            return reinterpret_cast<t_ret (*)(void *, t_args...)>(origFunc)(instance, args...);
#elif defined _WIN32
            return reinterpret_cast<t_ret(__fastcall *)(void *, int, t_args...)>(origFunc)(instance, 0, args...);
#endif
        }
    }
}