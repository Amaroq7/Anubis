/*
 *  Copyright (C) 2020-2021 Metamod-CPP Development Team
 *
 *  This file is part of Metamod-CPP.
 *
 *  Metamod-CPP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Metamod-CPP is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Metamod-CPP.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "MetaCvars.hpp"
#include "Utils.hpp"
#include "Metamod.hpp"

#include <osconfig.h>
#include <extdll.h>
#include <util.h>
#include <usercmd.h>

namespace Metamod::CvarListener
{
    void metaLogLevel(const char *pszNewValue)
    {
        if (bool result = gMetaGlobal->setLogLevel(pszNewValue); !result)
        {
            gMetaGlobal->logMsg(LogLevel::Warning, LogDest::Console, R"("Unknown loglevel "{}". Value has been set to "info".")", pszNewValue);
            CVAR_SET_STRING(gMetaLogLevelCvar.name, "info");
        }
    }
}
