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
#pragma once

#include <engine/Common.hpp>
#include <engine/IEdict.hpp>
#include <extdll.h>
#include <cinttypes>
#include <string_view>
#include <functional>

/*
 * These will be called by GameDLL directly.
 * They will make any necessary conversions needed from hl engine's structs to metamod's ones like:
 * edict_t * -> IEdict *
 * TraceResult * -> ITraceResult *
 * and so on.
 * Then they will execute engine funcs (Engine::Funcs) which will call their hookchains for plugins.
 * In the end executing their original counterpart in the hl engine.
 */
namespace Metamod::Engine::Callbacks::GameDLL
{
    int	pfnPrecacheModel(const char *s);
    int	pfnPrecacheSound(const char *s);
    void pfnChangeLevel(const char *s1, const char *s2);
    void pfnServerCommand(const char *str);
    void pfnServerExecute();
    void pfnAddServerCommand(const char *cmd_name, void (*function)());
    void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
    void pfnMessageEnd();
    void pfnWriteByte(int iValue);
    void pfnWriteChar(int iValue);
    void pfnWriteShort(int iValue);
    void pfnWriteLong(int iValue);
    void pfnWriteAngle(float flValue);
    void pfnWriteCoord(float flValue);
    void pfnWriteString(const char *sz);
    void pfnWriteEntity(int iValue);
    int pfnRegUserMsg(const char *pszName, int iSize);
    int pfnGetPlayerUserId(edict_t *e);
    const char *pfnGetPlayerAuthId(edict_t *e);
    char *pfnInfoKeyValue(char *infobuffer, const char *key);
    const char *pfnCmd_Args();
	const char *pfnCmd_Argv(int argc);
	int	pfnCmd_Argc();
    void pfnCVarRegister(cvar_t *pCvar);
    cvar_t *pfnCVarGetPointer(const char *szVarName);
    void pfnSetModel(edict_t *e, const char *m);
    edict_t *pfnCreateEntity();
    void pfnRemoveEntity(edict_t *e);
}
