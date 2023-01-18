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

#include <game/IBasePlayerAmmo.hpp>
#include <observer_ptr.hpp>
#include <extdll.h>
#include <tier0/platform.h>
#include <util.h>
#include <cbase.h>
#include <weapons.h>

namespace Anubis::Game::cstrike
{
    class BasePlayerAmmo: public BaseEntity, public virtual IBasePlayerAmmo
    {
        public:
            BasePlayerAmmo(nstd::observer_ptr<Engine::IEdict> edict);
            ~BasePlayerAmmo() override = default;
            void Spawn() final;
	        void DefaultTouch( nstd::observer_ptr<IBaseEntity> pOther ) final; // default weapon touch
	        bool AddAmmo( nstd::observer_ptr<IBaseEntity> pOther ) final;

	        void Materialize() final;

            explicit operator CBasePlayerAmmo *() const final;

            template<typename t_ret = void, typename... t_args>
        t_ret execFunc(std::string_view fnName, t_args... args) const
        {
    #if defined __linux__
                static auto fn = gConfig->getAddressFn<t_ret, void *, t_args...>(fnName, "CBasePlayerAmmo");
                return fn(operator CBasePlayerAmmo *(), args...);
    #else
                static auto fn = gConfig->getAddressFn<t_ret, void *, int, t_args...>(fnName, "CBasePlayerAmmo");
                return fn(operator CBasePlayerAmmo *(), 0, args...);
    #endif
        }
            
    };
}