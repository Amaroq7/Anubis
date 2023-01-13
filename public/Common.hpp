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

#include <cinttypes>

#if defined _WIN32
    #define ANUBIS_API __declspec(dllexport)
#elif defined __linux__
    #define ANUBIS_API __attribute__((visibility("default")))
#endif

#define ANUBIS_STRONG_TYPEDEF(_base, _type)                                                                            \
    struct _type                                                                                                       \
    {                                                                                                                  \
        _type() = default;                                                                                             \
        constexpr explicit _type(const _base v) : value(v) {}                                                          \
        _type(const _type &v) = default;                                                                               \
        _type &operator=(const _type &rhs) = default;                                                                  \
        _type &operator=(const _base &rhs)                                                                             \
        {                                                                                                              \
            value = rhs;                                                                                               \
            return *this;                                                                                              \
        }                                                                                                              \
        constexpr operator const _base &() const                                                                       \
        {                                                                                                              \
            return value;                                                                                              \
        }                                                                                                              \
        bool operator==(const _type &rhs) const                                                                        \
        {                                                                                                              \
            return value == rhs.value;                                                                                 \
        }                                                                                                              \
        bool operator==(const _base &rhs) const                                                                        \
        {                                                                                                              \
            return value == rhs;                                                                                       \
        }                                                                                                              \
        bool operator<(const _type &rhs) const                                                                         \
        {                                                                                                              \
            return value < rhs.value;                                                                                  \
        }                                                                                                              \
        _base value;                                                                                                   \
        using BaseType = _base;                                                                                        \
    };

#define ANUBIS_STRONG_TYPEDEF_PTR(_base, _type)                                                                        \
    struct _type                                                                                                       \
    {                                                                                                                  \
        _type() = default;                                                                                             \
        constexpr explicit _type(_base *v) : value(v) {}                                                               \
        _type(const _type &v) = default;                                                                               \
        _type &operator=(const _type &rhs) = default;                                                                  \
        _type &operator=(_base *rhs)                                                                                   \
        {                                                                                                              \
            value = rhs;                                                                                               \
            return *this;                                                                                              \
        }                                                                                                              \
        operator _base *() const                                                                                       \
        {                                                                                                              \
            return value;                                                                                              \
        }                                                                                                              \
        bool operator==(const _type &rhs) const                                                                        \
        {                                                                                                              \
            return value == rhs.value;                                                                                 \
        }                                                                                                              \
        bool operator==(const _base *rhs) const                                                                        \
        {                                                                                                              \
            return value == rhs;                                                                                       \
        }                                                                                                              \
        bool operator<(const _type &rhs) const                                                                         \
        {                                                                                                              \
            return value < rhs.value;                                                                                  \
        }                                                                                                              \
        _base *value;                                                                                                  \
        using BaseType = _base *;                                                                                      \
    };

namespace Anubis
{
    enum class FuncCallType : std::uint8_t
    {
        Direct = 0, /**< Call engine func or gamedll directly (without calling other plugins hooks) */
        Hooks       /**< Calls other plugins hooks first */
    };

    ANUBIS_STRONG_TYPEDEF(std::uint32_t, InterfaceVersion)
    ANUBIS_STRONG_TYPEDEF(std::uint16_t, MajorInterfaceVersion)
    ANUBIS_STRONG_TYPEDEF(std::uint16_t, MinorInterfaceVersion)
} // namespace Anubis
