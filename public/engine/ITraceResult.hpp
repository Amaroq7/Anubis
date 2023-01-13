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

#include "../observer_ptr.hpp"
#include "Common.hpp"

struct TraceResult;

namespace Anubis::Engine
{
    class IEdict;

    class ITraceResult
    {
    public:
        virtual ~ITraceResult() = default;

        [[nodiscard]] virtual bool getAllSolid() const = 0;
        [[nodiscard]] virtual bool getStartSolid() const = 0;
        [[nodiscard]] virtual bool getInOpen() const = 0;
        [[nodiscard]] virtual bool getInWater() const = 0;
        [[nodiscard]] virtual float getFraction() const = 0;
        [[nodiscard]] virtual const float *getEndPos() const = 0;
        [[nodiscard]] virtual float getPlaneDist() const = 0;
        [[nodiscard]] virtual const float *getPlaneNormal() const = 0;
        [[nodiscard]] virtual nstd::observer_ptr<IEdict> getHit() const = 0;
        [[nodiscard]] virtual HitGroup getHitGroup() const = 0;

        virtual void setAllSolid(bool allSolid) = 0;
        virtual void setStartSolid(bool startSolid) = 0;
        virtual void setInOpen(bool inOpen) = 0;
        virtual void setInWater(bool inWater) = 0;
        virtual void setFraction(float fraction) = 0;
        virtual void setEndPos(const float *endPos) = 0;
        virtual void setPlaneDist(float planeDist) = 0;
        virtual void setPlaneNormal(const float *planeNormal) = 0;
        virtual void setHit(nstd::observer_ptr<IEdict> hit) = 0;
        virtual void setHitGroup(HitGroup hitGroup) = 0;

        virtual explicit operator ::TraceResult *() const = 0;
    };
} // namespace Anubis::Engine