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

#include <observer_ptr.hpp>
#include <engine/IEdict.hpp>

#include <cinttypes>
#include <vector>
#include <memory>
#include <array>

namespace Anubis::Engine
{
    class ILibrary;

    class Edict final : public IEdict
    {
    public:
        Edict(edict_t *edict, nstd::observer_ptr<ILibrary> engine);
        ~Edict() final = default;

        // IEdict
        [[nodiscard]] std::uint32_t getIndex() const final;
        [[nodiscard]] std::uint32_t getSerialNumber() const final;
        [[nodiscard]] bool isFree() const final;

        [[nodiscard]] Flag getFlags() const final;

        [[nodiscard]] std::array<float, 3> getVecProperty(VecProperty property) const final;
        [[nodiscard]] StringOffset getStrProperty(StrProperty property) const final;
        [[nodiscard]] float getFlProperty(FlProperty property) const final;
        [[nodiscard]] std::int32_t getIntProperty(IntProperty property) const final;
        [[nodiscard]] std::int16_t getShortProperty(ShortProperty property) const final;
        [[nodiscard]] std::uint16_t getUShortProperty(UShortProperty property) const final;
        [[nodiscard]] std::byte getByteProperty(ByteProperty property) const final;
        [[nodiscard]] nstd::observer_ptr<IEdict> getEdictProperty(EdictProperty property) const final;

        [[nodiscard]] FixAngle getFixAngle() const final;
        [[nodiscard]] PrecacheId getModelIndex() const final;
        [[nodiscard]] MoveType getMoveType() const final;
        [[nodiscard]] SolidType getSolidType() const final;

        [[nodiscard]] Effects getEffects() const final;
        [[nodiscard]] std::array<std::byte, 4> getController() const final;
        [[nodiscard]] std::array<std::byte, 2> getBlending() const final;

        [[nodiscard]] RenderMode getRenderMode() const final;
        [[nodiscard]] DeadFlag getDeadFlag() const final;
        [[nodiscard]] SpawnFlag getSpawnFlag() const final;

        void setFlags(Flag flags) final;

        void setVecProperty(VecProperty property, std::array<float, 3> value) final;
        void setStrProperty(StrProperty property, StringOffset value) final;
        void setFlProperty(FlProperty property, float value) final;
        void setIntProperty(IntProperty property, std::int32_t value) final;
        void setShortProperty(ShortProperty property, std::int16_t value) final;
        void setUShortProperty(UShortProperty property, std::uint16_t value) final;
        void setByteProperty(ByteProperty property, std::byte value) final;
        void setEdictProperty(EdictProperty property, nstd::observer_ptr<IEdict> value) final;

        void setFixAngle(FixAngle fixAngle) final;
        void setModelIndex(PrecacheId modelIndex) final;
        void setMoveType(MoveType moveType) final;
        void setSolidType(SolidType solidType) final;

        void setEffects(Effects effects) final;
        void setController(std::array<std::byte, 4> controller) final;
        void setBlending(std::array<std::byte, 2> blending) final;

        void setRenderMode(RenderMode renderMode) final;
        void setDeadFlag(DeadFlag deadFlag) final;
        void setSpawnFlag(SpawnFlag spawnFlag) final;

        [[nodiscard]] void *getPrivateData() const final;

        explicit operator edict_t *() const final;
        explicit operator entvars_t *() const final;

    private:
        edict_t *m_edict;
        nstd::observer_ptr<ILibrary> m_engine;
    };
} // namespace Anubis::Engine