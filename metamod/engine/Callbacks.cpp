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
        return static_cast<int>(gMetaGlobal->getEngine()->precacheModel(s, FuncCallType::Hooks));
    }

    int	pfnPrecacheSound(const char *s)
    {
        return static_cast<int>(gMetaGlobal->getEngine()->precacheSound(s, FuncCallType::Hooks));
    }

    void pfnChangeLevel(const char *s1, const char *s2)
    {
        gMetaGlobal->getEngine()->changeLevel(s1, s2 ? s2 : std::string_view(), FuncCallType::Hooks);
    }

    void pfnServerCommand(const char *str)
    {
        gMetaGlobal->getEngine()->serverCommand(str, FuncCallType::Hooks);
    }

    void pfnServerExecute()
    {
        gMetaGlobal->getEngine()->serverExecute(FuncCallType::Hooks);
    }

    void pfnAddServerCommand(const char *cmd_name, void (*function)())
    {
        ServerCmdCallback cmdCallback {function};
        gMetaGlobal->getEngine()->registerSrvCommand(cmd_name, cmdCallback, FuncCallType::Hooks);
    }

    void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
    {
        static Engine *engine = gMetaGlobal->getEngine();

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
        gMetaGlobal->getEngine()->messageEnd(FuncCallType::Hooks);
    }

    void pfnWriteByte(int iValue)
    {
        gMetaGlobal->getEngine()->writeByte(static_cast<std::byte>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteChar(int iValue)
    {
        gMetaGlobal->getEngine()->writeChar(static_cast<char>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteShort(int iValue)
    {
        gMetaGlobal->getEngine()->writeShort(static_cast<std::int16_t>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteLong(int iValue)
    {
        gMetaGlobal->getEngine()->writeLong(static_cast<std::int32_t>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteAngle(float flValue)
    {
        gMetaGlobal->getEngine()->writeAngle(flValue, FuncCallType::Hooks);
    }

    void pfnWriteCoord(float flValue)
    {
        gMetaGlobal->getEngine()->writeCoord(flValue, FuncCallType::Hooks);
    }

    void pfnWriteString(const char *sz)
    {
        gMetaGlobal->getEngine()->writeString(sz, FuncCallType::Hooks);
    }

    void pfnWriteEntity(int iValue)
    {
        gMetaGlobal->getEngine()->writeEntity(static_cast<std::int16_t>(iValue), FuncCallType::Hooks);
    }

    int pfnRegUserMsg(const char *pszName, int iSize)
    {
        return gMetaGlobal->getEngine()->regUserMsg(pszName, static_cast<std::int16_t>(iSize), FuncCallType::Hooks);
    }

    int pfnGetPlayerUserId(edict_t *e)
    {
        static Engine *engine = gMetaGlobal->getEngine();
        return engine->getPlayerUserID(engine->getEdict(e), FuncCallType::Hooks);
    }

    const char *pfnGetPlayerAuthId(edict_t *e)
    {
        static Engine *engine = gMetaGlobal->getEngine();
        return engine->getPlayerAuthID(engine->getEdict(e), FuncCallType::Hooks).data();
    }
    
    char *pfnInfoKeyValue(char *infobuffer, const char *key)
    {
        static Engine *engine = gMetaGlobal->getEngine();
        return const_cast<char *>(engine->infoKeyValue(infobuffer, key, FuncCallType::Hooks).data());
    }
    
    const char *pfnCmd_Args()
    {
        static Engine *engine = gMetaGlobal->getEngine();
        return engine->cmdArgs(FuncCallType::Hooks).data();
    }
    
	const char *pfnCmd_Argv(int argc)
	{
	    static Engine *engine = gMetaGlobal->getEngine();
        return engine->cmdArgv(static_cast<std::uint8_t>(argc), FuncCallType::Hooks).data();
    }
    
	int	pfnCmd_Argc()
	{
	    static Engine *engine = gMetaGlobal->getEngine();
        return static_cast<int>(engine->cmdArgc(FuncCallType::Hooks));
    }

    void pfnCVarRegister(cvar_t *pCvar)
    {
        static Engine *engine = gMetaGlobal->getEngine();
        engine->registerCvar(pCvar);
    }

    cvar_t *pfnCVarGetPointer(const char *szVarName)
    {
        static Engine *engine = gMetaGlobal->getEngine();
        
        return *engine->getCvar(szVarName, FuncCallType::Hooks);
    }
    
    void pfnSetModel(edict_t *e, const char *m)
    {
        static Engine *engine = gMetaGlobal->getEngine();

        engine->setModel(engine->getEdict(e), m, FuncCallType::Hooks);
    }
}
