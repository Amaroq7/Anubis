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

#include <engine/ITraceResult.hpp>
#include <engine/ILibrary.hpp>
#include <observer_ptr.hpp>

#include <memory>
#include <variant>

typedef struct TraceResult TraceResult;

namespace Anubis::Engine
{
    class TraceResult final : public ITraceResult
    {
    public:
        TraceResult() = delete;
        explicit TraceResult(nstd::observer_ptr<ILibrary> engine);
        TraceResult(const TraceResult &other) = delete;
        TraceResult(TraceResult &&other) = delete;
        TraceResult(::TraceResult *traceResult, nstd::observer_ptr<ILibrary> engine);
        ~TraceResult() final = default;

        [[nodiscard]] bool getAllSolid() const final;
        [[nodiscard]] bool getStartSolid() const final;
        [[nodiscard]] bool getInOpen() const final;
        [[nodiscard]] bool getInWater() const final;
        [[nodiscard]] float getFraction() const final;
        [[nodiscard]] const float *getEndPos() const final;
        [[nodiscard]] float getPlaneDist() const final;
        [[nodiscard]] const float *getPlaneNormal() const final;
        [[nodiscard]] nstd::observer_ptr<IEdict> getHit() const final;
        [[nodiscard]] HitGroup getHitGroup() const final;

        void setAllSolid(bool allSolid) final;
        void setStartSolid(bool startSolid) final;
        void setInOpen(bool inOpen) final;
        void setInWater(bool inWater) final;
        void setFraction(float fraction) final;
        void setEndPos(const float *endPos) final;
        void setPlaneDist(float planeDist) final;
        void setPlaneNormal(const float *planeNormal) final;
        void setHit(nstd::observer_ptr<IEdict> hit) final;
        void setHitGroup(HitGroup hitGroup) final;

        explicit operator ::TraceResult *() const final;

    private:
        std::variant<::TraceResult *, std::unique_ptr<::TraceResult>> m_traceResult;
        nstd::observer_ptr<ILibrary> m_engine;
    };
} // namespace Anubis::Engine