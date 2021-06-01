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

#include <engine/IEdict.hpp>

#include "EntVars.hpp"

#include <cinttypes>
#include <vector>
#include <memory>

namespace Metamod::Engine
{
    class EntVars;

    class Edict final : public IEdict
    {
        public:
            Edict() = delete;
            explicit Edict(edict_t *edict);
            ~Edict() override = default;

            // IEdict
            [[nodiscard]] std::uint32_t getIndex() const final;
            [[nodiscard]] std::uint32_t getSerialNumber() const final;
            [[nodiscard]] bool isValid() const final;

            [[nodiscard]] EntVars *getEntVars() const final;

            // Edict
            explicit operator edict_t *() const final;

            [[nodiscard]] void *getPrivateData() const final;

        private:
            edict_t *m_edict = nullptr;
            std::unique_ptr<EntVars> m_entVars;
    };
} // namespace Metamod::Engine