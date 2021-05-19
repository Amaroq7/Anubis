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

#include "Callbacks.hpp"
#include <Metamod.hpp>

namespace Metamod::Engine::Callbacks::GameDLL
{
    int	pfnPrecacheModel(const char *s)
    {
        static Library *engine = gMetaGlobal->getEngine();
        return static_cast<int>(engine->precacheModel(s, FuncCallType::Hooks));
    }

    int	pfnPrecacheSound(const char *s)
    {
        static Library *engine = gMetaGlobal->getEngine();
        return static_cast<int>(engine->precacheSound(s, FuncCallType::Hooks));
    }

    void pfnChangeLevel(const char *s1, const char *s2)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->changeLevel(s1, s2 ? s2 : std::string_view(), FuncCallType::Hooks);
    }

    void pfnServerCommand(const char *str)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->serverCommand(str, FuncCallType::Hooks);
    }

    void pfnServerExecute()
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->serverExecute(FuncCallType::Hooks);
    }

    void pfnAddServerCommand(const char *cmd_name, void (*function)())
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->registerSrvCommand(cmd_name, function, FuncCallType::Hooks);
    }

    void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->messageBegin(
            static_cast<MsgDest>(msg_dest),
            static_cast<std::uint32_t>(msg_type),
            pOrigin,
            (ed) ? engine->getEdict(ed) : nullptr,
            FuncCallType::Hooks
        );
    }

    void pfnMessageEnd()
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->messageEnd(FuncCallType::Hooks);
    }

    void pfnWriteByte(int iValue)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->writeByte(static_cast<std::byte>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteChar(int iValue)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->writeChar(static_cast<char>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteShort(int iValue)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->writeShort(static_cast<std::int16_t>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteLong(int iValue)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->writeLong(static_cast<std::int32_t>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteAngle(float flValue)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->writeAngle(flValue, FuncCallType::Hooks);
    }

    void pfnWriteCoord(float flValue)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->writeCoord(flValue, FuncCallType::Hooks);
    }

    void pfnWriteString(const char *sz)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->writeString(sz, FuncCallType::Hooks);
    }

    void pfnWriteEntity(int iValue)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->writeEntity(static_cast<std::int16_t>(iValue), FuncCallType::Hooks);
    }

    int pfnRegUserMsg(const char *pszName, int iSize)
    {
        static Library *engine = gMetaGlobal->getEngine();
        return engine->regUserMsg(pszName, static_cast<std::int16_t>(iSize), FuncCallType::Hooks);
    }

    int pfnGetPlayerUserId(edict_t *e)
    {
        static Library *engine = gMetaGlobal->getEngine();
        return engine->getPlayerUserID(engine->getEdict(e), FuncCallType::Hooks);
    }

    const char *pfnGetPlayerAuthId(edict_t *e)
    {
        static Library *engine = gMetaGlobal->getEngine();
        return engine->getPlayerAuthID(engine->getEdict(e), FuncCallType::Hooks).data();
    }
    
    char *pfnInfoKeyValue(char *infobuffer, const char *key)
    {
        static Library *engine = gMetaGlobal->getEngine();
        return const_cast<char *>(engine->infoKeyValue(infobuffer, key, FuncCallType::Hooks).data());
    }
    
    const char *pfnCmd_Args()
    {
        static Library *engine = gMetaGlobal->getEngine();
        return engine->cmdArgs(FuncCallType::Hooks).data();
    }
    
	const char *pfnCmd_Argv(int argc)
	{
	    static Library *engine = gMetaGlobal->getEngine();
        return engine->cmdArgv(static_cast<std::uint8_t>(argc), FuncCallType::Hooks).data();
    }
    
	int	pfnCmd_Argc()
	{
	    static Library *engine = gMetaGlobal->getEngine();
        return static_cast<int>(engine->cmdArgc(FuncCallType::Hooks));
    }

    void pfnCVarRegister(cvar_t *pCvar)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->registerCvar(pCvar);
    }

    cvar_t *pfnCVarGetPointer(const char *szVarName)
    {
        static Library *engine = gMetaGlobal->getEngine();
        if (Cvar *cvar = engine->getCvar(szVarName, FuncCallType::Hooks); cvar)
        {
            return *cvar;
        }

        return nullptr;
    }
    
    void pfnSetModel(edict_t *e, const char *m)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->setModel(engine->getEdict(e), m, FuncCallType::Hooks);
    }

    edict_t *pfnCreateEntity()
    {
        static Library *engine = gMetaGlobal->getEngine();
        return *engine->createEntity(FuncCallType::Hooks);
    }

    void pfnRemoveEntity(edict_t *e)
    {
        static Library *engine = gMetaGlobal->getEngine();
        engine->removeEntity(engine->getEdict(e), FuncCallType::Hooks);
    }
}
