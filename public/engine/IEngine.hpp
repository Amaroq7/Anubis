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

#include <cinttypes>
#include <cstddef>

namespace Metamod::Engine
{
    class IEdict;
    class ITraceResult;
    class IGlobals;
    class IFuncs;
    class IHooks;
    class IReFuncs;

    class IEngine
    {
    public:
        virtual ~IEngine() = default;

        virtual IEdict *getEdict(std::uint32_t index) = 0;
        virtual IGlobals *getGlobals() const = 0;
        virtual IFuncs *getFuncs(bool direct = false) const = 0;
        virtual ITraceResult *createTraceResult() = 0;
        virtual void freeTraceResult(ITraceResult *tr) = 0;
        virtual IHooks *getHooks() = 0;
        virtual IReFuncs *getReHLDSFuncs() = 0;
    };
} // namespace Metamod::Engine