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

#include <engine/ITraceResult.hpp>
#include <engine/ILibrary.hpp>

#include <memory>
#include <variant>

typedef struct TraceResult TraceResult;

namespace Metamod::Engine
{
    class TraceResult final : public ITraceResult
    {
    public:
        TraceResult() = delete;
        explicit TraceResult(ILibrary *engine);
        TraceResult(const TraceResult &other) = delete;
        TraceResult(TraceResult &&other) = delete;
        TraceResult(::TraceResult *traceResult, ILibrary *engine);
        ~TraceResult() final = default;

        [[nodiscard]] bool getAllSolid() const final;
        [[nodiscard]] bool getStartSolid() const final;
        [[nodiscard]] bool getInOpen() const final;
        [[nodiscard]] bool getInWater() const final;
        [[nodiscard]] float getFraction() const final;
        [[nodiscard]] const float *getEndPos() const final;
        [[nodiscard]] float getPlaneDist() const final;
        [[nodiscard]] const float *getPlaneNormal() const final;
        [[nodiscard]] IEdict *getHit() const final;
        [[nodiscard]] HitGroup getHitGroup() const final;

        void setAllSolid(bool allSolid) final;
        void setStartSolid(bool startSolid) final;
        void setInOpen(bool inOpen) final;
        void setInWater(bool inWater) final;
        void setFraction(float fraction) final;
        void setEndPos(const float *endPos) final;
        void setPlaneDist(float planeDist) final;
        void setPlaneNormal(const float *planeNormal) final;
        void setHit(IEdict *hit) final;
        void setHitGroup(HitGroup hitGroup) final;

        explicit operator ::TraceResult *() const final;

    private:
        std::variant<::TraceResult *, std::unique_ptr<::TraceResult>> m_traceResult;
        ILibrary *m_engine;
    };
} // namespace SPMod::Engine