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

#include <engine/ICvar.hpp>
#include <cvardef.h>
#include <memory>
#include <variant>
#include <string>

namespace Metamod::Engine
{
    class Cvar final : public ICvar
    {
    public:
        Cvar(cvar_t *cvar);
        Cvar(std::string_view name, std::string_view value);
        ~Cvar() override = default;

        std::string_view getName() const override;
        std::string_view getString() const override;
        Flags getFlags() const override;
        float getValue() const override;

        void setString(std::string_view string) override;
        void setFlags(Flags flags) override;
        void setValue(float value) override;

        operator cvar_t *() const;

    private:
        std::string m_name;
        std::variant<cvar_t *, std::unique_ptr<cvar_t>> m_cvar;
    };
}