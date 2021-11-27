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

namespace Anubis::Engine
{
    class ILibrary;

    class Edict final : public IEdict
    {
    public:
        explicit Edict(edict_t *edict, nstd::observer_ptr<ILibrary> engine);
        ~Edict() final = default;

        // IEdict
        [[nodiscard]] std::uint32_t getIndex() const final;
        [[nodiscard]] std::uint32_t getSerialNumber() const final;
        [[nodiscard]] bool isFree() const final;

        [[nodiscard]] EntFlags getFlags() const final;
        void setFlags(EntFlags flags) final;

        [[nodiscard]] void *getPrivateData() const final;

        explicit operator edict_t *() const final;
        explicit operator entvars_t *() const final;

    private:
        edict_t *m_edict;
        nstd::observer_ptr<ILibrary> m_engine;
    };
} // namespace Anubis::Engine