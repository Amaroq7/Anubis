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

#include <extdll.h>

namespace Metamod::Game
{
    using fnNewDllFunctions = int(*)(NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion);
    using fnDllFunctionsv2 = int(*)(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion);
    using fnDllFunctions = int(*)(DLL_FUNCTIONS *pFunctionTable, int interfaceVersion);
}