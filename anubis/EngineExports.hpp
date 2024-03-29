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

typedef struct enginefuncs_s enginefuncs_t;
typedef struct globalvars_s globalvars_t;

namespace Anubis::Engine
{
#if defined __linux__
    using fnGiveFuncs = void (*)(enginefuncs_t *pengfuncsFromEngine, globalvars_t *pGlobals);
#elif defined _WIN32
    using fnGiveFuncs = void(__stdcall *)(enginefuncs_t *pengfuncsFromEngine, globalvars_t *pGlobals);
#endif
} // namespace Anubis::Engine