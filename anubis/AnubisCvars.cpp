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

#include "AnubisCvars.hpp"
#include "Anubis.hpp"

namespace Anubis::CvarListener
{
    void anubisLogLevel(const char *pszNewValue)
    {
        if (bool result = gAnubisApi->setLogLevel(pszNewValue); !result)
        {
            gAnubisApi->getLogger()->logMsg(LogLevel::Warning, LogDest::Console,
                                            R"("Unknown loglevel "{}". Value has been set to "info".")", pszNewValue);
            gAnubisApi->getEngine()->setCvarString(gAnubisLogLevelCvar.name, "info", FuncCallType::Direct);
        }
    }
} // namespace Anubis::CvarListener
