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

#define GET_INFOKEYBUFFER           (*g_engfuncs.pfnGetInfoKeyBuffer)
#define INFOKEY_VALUE               (*g_engfuncs.pfnInfoKeyValue)
#define SET_CLIENT_KEYVALUE         (*g_engfuncs.pfnSetClientKeyValue)
#define REG_SVR_COMMAND             (*g_engfuncs.pfnAddServerCommand)
#define SERVER_PRINT                (*g_engfuncs.pfnServerPrint)
#define SET_SERVER_KEYVALUE         (*g_engfuncs.pfnSetKeyValue)
#define QUERY_CLIENT_CVAR_VALUE     (*g_engfuncs.pfnQueryClientCvarValue)
#define QUERY_CLIENT_CVAR_VALUE2    (*g_engfuncs.pfnQueryClientCvarValue2)

/*
 * These will be called by metamod's plugins directly.
 * These functions contain hookchains, so metamod's plugins
 * Then they will call their upper counterparts (those are intended to be called by metamod plugins) which contain hookchains.
 */
namespace Metamod::Engine::Callbacks::Direct
{
    void pfnChangeLevel(std::string_view level1, std::string_view level2);
    void pfnMessageBegin(MsgDest msgDest, std::uint32_t msgType, const float *pOrigin, IEdict *edict);
    void pfnMessageEnd();
    void pfnWriteByte(std::byte byteArg);
    void pfnWriteChar(char charArg);
    void pfnWriteShort(std::int16_t shortArg);
    void pfnWriteLong(std::int32_t longArg);
    void pfnWriteAngle(float angleArg);
    void pfnWriteCoord(float coordArg);
    void pfnWriteString(std::string_view strArg);
    void pfnWriteEntity(std::int16_t entArg);
    std::uint8_t pfnRegUserMsg(std::string_view name, std::uint8_t size);
}

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
    void pfnChangeLevel(const char* s1, const char* s2);
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
}