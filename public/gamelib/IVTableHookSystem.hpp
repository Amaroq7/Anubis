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

#pragma once

#include "StandardHeaders.hpp"
#include "IInterface.hpp"

namespace Metamod::GameLib
{
    enum class vFuncType : std::uint16_t
    {
        Spawn = 0,
        TakeDamage,
        TraceAttack
    };

    class IVTableHook
    {
    public:
        using Callback = std::function<Return(IVTableHook *const hook, bool post)>;
        using Param = std::variant<std::int32_t,
                                   std::uint32_t,
                                   std::int16_t,
                                   float,
                                   float *,
                                   Engine::IEntVars *,
                                   Engine::IEdict *,
                                   Engine::ITraceResult *,
                                   const char *>;

        virtual ~IVTableHook() = default;

        virtual const std::optional<Param> &getReturn() const = 0;
        virtual std::vector<Param> &getParams() = 0;
    };
} // namespace Metamod::GameLib