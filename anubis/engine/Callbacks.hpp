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

#include <observer_ptr.hpp>
#include <engine/Common.hpp>

#include <extdll.h>

typedef struct edict_s edict_t;
typedef struct cvar_s cvar_t;
typedef struct TraceResult TraceResult;

namespace Anubis
{
    namespace Engine
    {
        class Library;
    }

    namespace Game
    {
        class ILibrary;
    }
} // namespace Anubis

/*
 * These will be called by GameDLL directly.
 * They will make any necessary conversions needed from hl engine's structs to metamod's ones like:
 * edict_t * -> IEdict *
 * TraceResult * -> ITraceResult *
 * and so on.
 * Then they will execute engine funcs (Engine::Funcs) which will call their hookchains for plugins.
 * In the end executing their original counterpart in the hl engine.
 */

namespace Anubis::Engine::Callbacks::GameDLL
{
    nstd::observer_ptr<Game::ILibrary> getGame(nstd::observer_ptr<Game::ILibrary> game = {});
    nstd::observer_ptr<Library> getEngine(nstd::observer_ptr<Library> engine = {});

    int pfnPrecacheModel(const char *s);
    int pfnPrecacheSound(const char *s);
    int pfnPrecacheGeneric(const char *s);
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
    int pfnCmd_Argc();
    void pfnCVarRegister(cvar_t *pCvar);
    cvar_t *pfnCVarGetPointer(const char *szVarName);
    void pfnSetModel(edict_t *e, const char *m);
    edict_t *pfnCreateEntity();
    void pfnRemoveEntity(edict_t *e);
    void pfnServerPrint(const char *szMsg);
    int pfnIsDedicatedServer();
    int pfnEngCheckParm(const char *pchCmdLineToken, char **ppnext);
    void pfnQueryClientCvarValue(const edict_t *player, const char *cvarName);
    void pfnQueryClientCvarValue2(const edict_t *player, const char *cvarName, int requestID);
    void pfnCvar_DirectSet(cvar_t *var, const char *value);
    int pfnIndexOfEdict(const edict_t *pEdict);
    void pfnGetGameDir(char *szGetGameDir);
    float pfnCVarGetFloat(const char *szVarName);
    const char *pfnCVarGetString(const char *szVarName);
    void pfnCVarSetFloat(const char *szVarName, float flValue);
    void pfnCVarSetString(const char *szVarName, const char *szValue);
    edict_t *pfnPEntityOfEntOffset(int iEntOffset);
    int pfnEntOffsetOfPEntity(const edict_t *pEdict);
    edict_t *pfnPEntityOfEntIndex(int iEntIndex);
    void *pfnPvAllocEntPrivateData(edict_t *pEdict, std::int32_t cb);
    const char *pfnSzFromIndex(int iString);
    int pfnAllocString(const char *szValue);
    int pfnModelIndex(const char *m);
    int pfnRandomLong(int lLow, int lHigh);
    float pfnRandomFloat(float flLow, float flHigh);
    void pfnClientPrintf(edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg);
    int pfnEntIsOnFloor(edict_t *e);
    int pfnDropToFloor(edict_t *e);
    void pfnEmitSound(edict_t *entity,
                      int channel,
                      const char *sample,
                      /*int*/ float volume,
                      float attenuation,
                      int fFlags,
                      int pitch);
    void pfnEmitAmbientSound(edict_t *entity,
                             float *pos,
                             const char *samp,
                             float vol,
                             float attenuation,
                             int fFlags,
                             int pitch);
    void pfnTraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
    void pfnTraceToss(edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr);
    int pfnTraceMonsterHull(edict_t *pEdict,
                            const float *v1,
                            const float *v2,
                            int fNoMonsters,
                            edict_t *pentToSkip,
                            TraceResult *ptr);
    void pfnTraceHull(const float *v1,
                      const float *v2,
                      int fNoMonsters,
                      int hullNumber,
                      edict_t *pentToSkip,
                      TraceResult *ptr);
    void pfnTraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr);
    const char *pfnTraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2);
    void pfnTraceSphere(const float *v1,
                        const float *v2,
                        int fNoMonsters,
                        float radius,
                        edict_t *pentToSkip,
                        TraceResult *ptr);

    void pfnSetOrigin(edict_t *e, const float *rgflOrigin);
    void pfnSetSize(edict_t *e, const float *rgflMin, const float *rgflMax);
    edict_t *pfnCreateNamedEntity(int className);
} // namespace Anubis::Engine::Callbacks::GameDLL
