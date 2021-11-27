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

#include <engine/ICvar.hpp>
#include <observer_ptr.hpp>

#include <memory>
#include <variant>
#include <string>

namespace Anubis::Engine
{
    class ILibrary;

    class Cvar final : public ICvar
    {
    public:
        Cvar(cvar_t *cvar, nstd::observer_ptr<ILibrary> engine);
        Cvar(std::string_view name, std::string_view value, nstd::observer_ptr<ILibrary> engine);
        ~Cvar() override = default;

        [[nodiscard]] std::string_view getName() const override;
        [[nodiscard]] std::string_view getString() const override;
        [[nodiscard]] Flags getFlags() const override;
        [[nodiscard]] float getValue() const override;

        void setString(std::string_view string) override;
        void setFlags(Flags flags) override;
        void setValue(float value) override;

        explicit operator cvar_t *() const final;

    private:
        std::string m_name;
        std::variant<cvar_t *, std::unique_ptr<cvar_t>> m_cvar;
        nstd::observer_ptr<ILibrary> m_engine;
    };
} // namespace Anubis::Engine