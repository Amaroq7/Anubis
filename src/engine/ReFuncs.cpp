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

#include "engine/ReFuncs.hpp"
#include <extdll.h>
#include <usercmd.h>
#include <osconfig.h>
#include <rehlds_api.h>

namespace Metamod::Engine 
{
    ReFuncs::ReFuncs(const RehldsFuncs_t *reFuncs) : m_funcs(reFuncs) {}

    bool ReFuncs::addExtDll(void *hModule) const
    {
        return m_funcs->AddExtDll(hModule);
    }

    void ReFuncs::removeExtDll(void *hModule) const
    {
        m_funcs->RemoveExtDll(hModule);
    }
}