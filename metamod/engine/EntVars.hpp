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

#include <engine/IEntVars.hpp>

namespace Metamod::Engine
{
    class EntVars final : public IEntVars
    {
        public:
            EntVars() = delete;
            explicit EntVars(entvars_t *entvars);
            ~EntVars() final = default;

            [[nodiscard]] EntFlags getFlags() const final;
            void setFlags(EntFlags flags) final;

            [[nodiscard]] std::uint32_t getIndex() const final;

            // EntVars
            explicit operator entvars_t *() const final;

        private:
            entvars_t *m_entVars = nullptr;
    };
}