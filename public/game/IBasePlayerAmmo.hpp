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

#include "IBaseEntity.hpp"
#include <observer_ptr.hpp>

/**
 * @internal A internal representation of a base entity in gamedll
 */
class CBasePlayerAmmo;

namespace Anubis::Game
{
    class IBasePlayerAmmo: public virtual IBaseEntity
    {
        public:
            ~IBasePlayerAmmo() override = default;
            virtual void Spawn() = 0;
	        void DefaultTouch( nstd::observer_ptr<IBaseEntity> pOther ) = 0; // default weapon touch
	        virtual bool AddAmmo( nstd::observer_ptr<IBaseEntity> pOther ) = 0;

	        void Materialize() = 0;


                /**
             * @cond INTERNAL
             */
            /**
             * @internal Allows to get a internal representation of a base entity in the gamedll.
             */
            explicit virtual operator CBasePlayerAmmo *() const = 0;
    };
}