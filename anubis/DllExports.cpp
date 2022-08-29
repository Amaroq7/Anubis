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

#include "DllExports.hpp"
#include <Common.hpp>
#include "Anubis.hpp"

extern "C" ANUBIS_API int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion)
{
    if (!pFunctionTable)
    {
        return 0;
    }

    if (*interfaceVersion != INTERFACE_VERSION)
    {
        *interfaceVersion = INTERFACE_VERSION;
        return 0;
    }

    *pFunctionTable = *Anubis::gAnubisApi->getGame()->getDllFuncs();
    Anubis::gAnubisApi->getEngine()->addExtDll(Anubis::gAnubisApi->getGame()->getSystemHandle());
    return 1;
}

extern "C" ANUBIS_API int GetNewDLLFunctions(NEW_DLL_FUNCTIONS *pNewFunctionTable, int *interfaceVersion)
{
    if (!pNewFunctionTable)
    {
        return 0;
    }
    if (*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION)
    {
        *interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
        return 0;
    }

    *pNewFunctionTable = *Anubis::gAnubisApi->getGame()->getNewDllFuncs();
    return 1;
}
