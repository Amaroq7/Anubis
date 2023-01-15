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

#include "Callbacks.hpp"
#include "Library.hpp"
#include <engine/ICvar.hpp>
#include <engine/IEdict.hpp>
#include <game/IBaseEntity.hpp>
#include <game/ILibrary.hpp>

namespace Anubis::Engine::Callbacks::GameDLL
{
    nstd::observer_ptr<Game::ILibrary> getGame(nstd::observer_ptr<Game::ILibrary> game)
    {
        static nstd::observer_ptr<Game::ILibrary> gameInstance = game;
        return gameInstance;
    }

    nstd::observer_ptr<Library> getEngine(nstd::observer_ptr<Library> engine)
    {
        static nstd::observer_ptr<Library> engineInstance = engine;
        return engineInstance;
    }

    int pfnPrecacheModel(const char *s)
    {
        return static_cast<int>(getEngine()->precacheModel(s, FuncCallType::Hooks));
    }

    int pfnPrecacheSound(const char *s)
    {
        return static_cast<int>(getEngine()->precacheSound(s, FuncCallType::Hooks));
    }

    int pfnPrecacheGeneric(const char *s)
    {
        return static_cast<int>(getEngine()->precacheGeneric(s, FuncCallType::Hooks));
    }

    void pfnChangeLevel(const char *s1, const char *s2)
    {
        getEngine()->changeLevel(s1, s2 ? s2 : std::string_view(), FuncCallType::Hooks);
    }

    void pfnServerCommand(const char *str)
    {
        getEngine()->serverCommand(str, FuncCallType::Hooks);
    }

    void pfnServerExecute()
    {
        getEngine()->serverExecute(FuncCallType::Hooks);
    }

    void pfnAddServerCommand(const char *cmd_name, void (*function)())
    {
        getEngine()->registerSrvCommand(cmd_name, function, FuncCallType::Hooks);
    }

    void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
    {
        std::optional<std::array<float, 3>> msgOrigin;
        if (pOrigin)
        {
            msgOrigin = {pOrigin[0], pOrigin[1], pOrigin[2]};
        }
        getEngine()->messageBegin(static_cast<MsgDest>(msg_dest), MsgType(static_cast<std::uint32_t>(msg_type)),
                                  pOrigin ? msgOrigin : std::nullopt,
                                  (ed) ? getEngine()->getEdict(ed) : nstd::make_observer<IEdict>(),
                                  FuncCallType::Hooks);
    }

    void pfnMessageEnd()
    {
        getEngine()->messageEnd(FuncCallType::Hooks);
    }

    void pfnWriteByte(int iValue)
    {
        getEngine()->writeByte(static_cast<std::byte>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteChar(int iValue)
    {
        getEngine()->writeChar(static_cast<char>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteShort(int iValue)
    {
        getEngine()->writeShort(static_cast<std::int16_t>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteLong(int iValue)
    {
        getEngine()->writeLong(static_cast<std::int32_t>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteAngle(float flValue)
    {
        getEngine()->writeAngle(MsgAngle(flValue), FuncCallType::Hooks);
    }

    void pfnWriteCoord(float flValue)
    {
        getEngine()->writeCoord(MsgCoord(flValue), FuncCallType::Hooks);
    }

    void pfnWriteString(const char *sz)
    {
        getEngine()->writeString(sz, FuncCallType::Hooks);
    }

    void pfnWriteEntity(int iValue)
    {
        getEngine()->writeEntity(MsgEntity(static_cast<std::int16_t>(iValue)), FuncCallType::Hooks);
    }

    int pfnRegUserMsg(const char *pszName, int iSize)
    {
        return getEngine()->regUserMsg(pszName, MsgSize(static_cast<MsgSize::BaseType>(iSize)), FuncCallType::Hooks);
    }

    int pfnGetPlayerUserId(edict_t *e)
    {
        return getEngine()->getPlayerUserID(getEngine()->getEdict(e), FuncCallType::Hooks);
    }

    const char *pfnGetPlayerAuthId(edict_t *e)
    {
        return getEngine()->getPlayerAuthID(getEngine()->getEdict(e), FuncCallType::Hooks).data();
    }

    char *pfnInfoKeyValue(char *infobuffer, const char *key)
    {
        return const_cast<char *>(getEngine()->infoKeyValue(InfoBuffer(infobuffer), key, FuncCallType::Hooks).data());
    }

    const char *pfnCmd_Args()
    {
        return getEngine()->cmdArgs(FuncCallType::Hooks).data();
    }

    const char *pfnCmd_Argv(int argc)
    {
        return getEngine()->cmdArgv(static_cast<std::uint8_t>(argc), FuncCallType::Hooks).data();
    }

    int pfnCmd_Argc()
    {
        return static_cast<int>(getEngine()->cmdArgc(FuncCallType::Hooks));
    }

    void pfnCVarRegister(cvar_t *pCvar)
    {
        getEngine()->registerCvar(pCvar);
    }

    cvar_t *pfnCVarGetPointer(const char *szVarName)
    {
        if (nstd::observer_ptr<ICvar> cvar = getEngine()->getCvar(szVarName, FuncCallType::Hooks); cvar)
        {
            return static_cast<cvar_t *>(*cvar);
        }

        return nullptr;
    }

    void pfnSetModel(edict_t *e, const char *m)
    {
        getEngine()->setModel(getEngine()->getEdict(e), m, FuncCallType::Hooks);
    }

    edict_t *pfnCreateEntity()
    {
        return static_cast<edict_t *>(*getEngine()->createEntity(FuncCallType::Hooks));
    }

    void pfnRemoveEntity(edict_t *e)
    {
        getEngine()->removeEntity(getEngine()->getEdict(e), FuncCallType::Hooks);
    }

    void pfnServerPrint(const char *szMsg)
    {
        getEngine()->print(szMsg, FuncCallType::Hooks);
    }

    int pfnIsDedicatedServer()
    {
        return static_cast<int>(getEngine()->isDedicatedServer(FuncCallType::Hooks));
    }

    int pfnEngCheckParm(const char *pchCmdLineToken, char **ppnext)
    {
        const auto &[pos, nextParam] = getEngine()->checkEngParm(pchCmdLineToken, FuncCallType::Hooks);

        if (ppnext)
        {
            *ppnext = nextParam.empty() ? nullptr : const_cast<char *>(nextParam.data());
        }

        return static_cast<int>(pos);
    }

    void pfnQueryClientCvarValue(const edict_t *player, const char *cvarName)
    {
        getEngine()->queryClientCvarValue(getEngine()->getEdict(player), cvarName, FuncCallType::Hooks);
    }

    void pfnQueryClientCvarValue2(const edict_t *player, const char *cvarName, int requestID)
    {
        getEngine()->queryClientCvarValue2(getEngine()->getEdict(player), cvarName,
                                           static_cast<std::uint32_t>(requestID), FuncCallType::Hooks);
    }

    void pfnCvar_DirectSet(cvar_t *var, const char *value)
    {
        nstd::observer_ptr<ICvar> cvar = getEngine()->addToCache(var);
        getEngine()->cvarDirectSet(cvar, value, FuncCallType::Hooks);
    }

    int pfnIndexOfEdict(const edict_t *pEdict)
    {
        nstd::observer_ptr<IEdict> edict = getEngine()->getEdict(pEdict);
        return static_cast<int>(getEngine()->getIndexOfEdict(edict, FuncCallType::Hooks));
    }

    void pfnGetGameDir(char *szGetGameDir)
    {
        std::string gameDir = getEngine()->getGameDir(FuncCallType::Hooks);

#if defined __linux__
        strncpy(szGetGameDir, gameDir.c_str(), MAX_PATH);
#elif defined _WIN32
        strncpy_s(szGetGameDir, MAX_PATH, gameDir.c_str(), _TRUNCATE);
#endif
    }

    float pfnCVarGetFloat(const char *szVarName)
    {
        return getEngine()->getCvarFloat(szVarName, FuncCallType::Hooks);
    }

    const char *pfnCVarGetString(const char *szVarName)
    {
        return getEngine()->getCvarString(szVarName, FuncCallType::Hooks).data();
    }

    void pfnCVarSetFloat(const char *szVarName, float flValue)
    {
        getEngine()->setCvarFloat(szVarName, flValue, FuncCallType::Hooks);
    }

    void pfnCVarSetString(const char *szVarName, const char *szValue)
    {
        getEngine()->setCvarString(szVarName, szValue, FuncCallType::Hooks);
    }

    edict_t *pfnPEntityOfEntOffset(int iEntOffset)
    {
        return static_cast<edict_t *>(
            *getEngine()->getEntityOfEntOffset(EntityOffset(iEntOffset), FuncCallType::Hooks));
    }

    int pfnEntOffsetOfPEntity(const edict_t *pEdict)
    {
        return static_cast<int>(getEngine()->getEntityOffset(getEngine()->getEdict(pEdict), FuncCallType::Hooks));
    }

    edict_t *pfnPEntityOfEntIndex(int iEntIndex)
    {
        nstd::observer_ptr<IEdict> edict =
            getEngine()->getEdict(static_cast<std::uint32_t>(iEntIndex), FuncCallType::Hooks);

        return edict ? static_cast<edict_t *>(*edict) : nullptr;
    }

    void *pfnPvAllocEntPrivateData(edict_t *pEdict, std::int32_t cb)
    {
        static bool vFuncInitialized = false;
        static bool playerEdictsInitialized = false;

        getEngine()->initEdict(pEdict);
        if (!vFuncInitialized)
        {
            vFuncInitialized = true;
            getGame()->initVFuncHooks();
        }
        if (!playerEdictsInitialized)
        {
            playerEdictsInitialized = true;
            getEngine()->initPlayerEdicts();
        }

        std::ignore = getEngine()->allocEntPrivateData(getEngine()->getEdict(pEdict), cb, FuncCallType::Hooks);

        return pEdict->pvPrivateData;
    }

    const char *pfnSzFromIndex(int iString)
    {
        return getEngine()->getString(StringOffset(static_cast<std::uint32_t>(iString)), FuncCallType::Hooks).data();
    }

    int pfnAllocString(const char *szValue)
    {
        return static_cast<int>(getEngine()->allocString(szValue, FuncCallType::Hooks).value);
    }

    int	pfnModelIndex (const char *m)
    {
        return static_cast<int>(getEngine()->modelIndex(m, FuncCallType::Hooks));
    }
    
    int pfnRandomLong (int  lLow,  int  lHigh)
    {
        return getEngine()->randomLong(lLow, lHigh,FuncCallType::Hooks);
    }

	float pfnRandomFloat (float flLow, float flHigh)
    {
        return getEngine()->randomFloat(flLow, flHigh, FuncCallType::Hooks);
    }

    void pfnClientPrintf(edict_t* pEdict, PRINT_TYPE ptype, const char* szMsg)
    {
        getEngine()->clientPrint(getEngine()->getEdict(pEdict), static_cast<PrintType>(ptype), szMsg, FuncCallType::Hooks);
    }
} // namespace Anubis::Engine::Callbacks::GameDLL
