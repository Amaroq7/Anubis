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
#include <engine/IGameClient.hpp>

namespace Anubis::Engine
{
    class ILibrary;
    
    class GameClient final : public IGameClient
    {
    public:
        GameClient(::IGameClient *gameClient, nstd::observer_ptr<ILibrary> engine);
        ~GameClient() final = default;

        [[nodiscard]] nstd::observer_ptr<IEdict> getEdict() const final;

        explicit operator ::IGameClient *() const final;

    private:
        ::IGameClient *m_gameClient;
        nstd::observer_ptr<ILibrary> m_engine;
    };
} // namespace Anubis::Engine