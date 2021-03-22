#pragma once

#include <extdll.h>
#include <enginecallback.h>
#include <util.h>

namespace Metamod::Engine
{
    using fnGiveFuncs = void (*)(enginefuncs_t *pengfuncsFromEngine, globalvars_t *pGlobals);
}