/*
 *  Copyright (C) 2020 Metamod++ Development Team
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

#include "Cvar.hpp"
#include <extdll.h>
#include <enginecallback.h>
#include <functional>
#include <string>

namespace Metamod::Engine
{
    Cvar::Cvar(cvar_t *cvar) : m_cvar(cvar) {}
    Cvar::Cvar(std::string_view name, std::string_view value) : m_name(name), m_cvar(std::make_unique<cvar_t>())
    {
        operator ::cvar_t *()->name = m_name.data();
        operator ::cvar_t *()->string = const_cast<char *>(value.data());
    }

    std::string_view Cvar::getName() const
    {
        return operator ::cvar_t *()->name;
    }

    std::string_view Cvar::getString() const
    {
        return operator ::cvar_t *()->string;
    }

    Cvar::Flags Cvar::getFlags() const
    {
        return static_cast<Flags>(operator ::cvar_t *()->flags);
    }

    float Cvar::getValue() const
    {
        return operator ::cvar_t *()->value;
    }

    void Cvar::setString(std::string_view string)
    {
        std::invoke(g_engfuncs.pfnCvar_DirectSet, operator ::cvar_t *(), string.data());
    }

    void Cvar::setFlags(ICvar::Flags flags)
    {
        operator ::cvar_t *()->flags = static_cast<int>(flags);
    }

    void Cvar::setValue(float value)
    {
        std::invoke(g_engfuncs.pfnCvar_DirectSet, operator ::cvar_t *(), std::to_string(value).c_str());
    }

    Cvar::operator ::cvar_t *() const
    {
        return std::visit(
            [](auto &&arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, cvar_t *>)
                {
                    return arg;
                }
                else
                {
                    return arg.get();
                }
            }, m_cvar);
    }
}
