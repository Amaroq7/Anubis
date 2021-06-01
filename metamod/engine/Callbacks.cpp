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

#include "Callbacks.hpp"
#include "Library.hpp"
#include <engine/ICvar.hpp>
#include <engine/IEdict.hpp>

namespace Metamod::Engine::Callbacks::GameDLL
{
    static Library *gEngine;
    void init(Engine::Library *engine)
    {
        gEngine = engine;
    }

    int	pfnPrecacheModel(const char *s)
    {
        return static_cast<int>(gEngine->precacheModel(s, FuncCallType::Hooks));
    }

    int	pfnPrecacheSound(const char *s)
    {
        return static_cast<int>(gEngine->precacheSound(s, FuncCallType::Hooks));
    }

    void pfnChangeLevel(const char *s1, const char *s2)
    {
        gEngine->changeLevel(s1, s2 ? s2 : std::string_view(), FuncCallType::Hooks);
    }

    void pfnServerCommand(const char *str)
    {
        gEngine->serverCommand(str, FuncCallType::Hooks);
    }

    void pfnServerExecute()
    {
        gEngine->serverExecute(FuncCallType::Hooks);
    }

    void pfnAddServerCommand(const char *cmd_name, void (*function)())
    {
        gEngine->registerSrvCommand(cmd_name, function, FuncCallType::Hooks);
    }

    void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
    {
        gEngine->messageBegin(
            static_cast<MsgDest>(msg_dest),
            static_cast<std::uint32_t>(msg_type),
            pOrigin,
            (ed) ? gEngine->getEdict(ed) : nullptr,
            FuncCallType::Hooks
        );
    }

    void pfnMessageEnd()
    {
        gEngine->messageEnd(FuncCallType::Hooks);
    }

    void pfnWriteByte(int iValue)
    {
        gEngine->writeByte(static_cast<std::byte>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteChar(int iValue)
    {
        gEngine->writeChar(static_cast<char>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteShort(int iValue)
    {
        gEngine->writeShort(static_cast<std::int16_t>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteLong(int iValue)
    {
        gEngine->writeLong(static_cast<std::int32_t>(iValue), FuncCallType::Hooks);
    }

    void pfnWriteAngle(float flValue)
    {
        gEngine->writeAngle(flValue, FuncCallType::Hooks);
    }

    void pfnWriteCoord(float flValue)
    {
        gEngine->writeCoord(flValue, FuncCallType::Hooks);
    }

    void pfnWriteString(const char *sz)
    {
        gEngine->writeString(sz, FuncCallType::Hooks);
    }

    void pfnWriteEntity(int iValue)
    {
        gEngine->writeEntity(static_cast<std::int16_t>(iValue), FuncCallType::Hooks);
    }

    int pfnRegUserMsg(const char *pszName, int iSize)
    {
        return gEngine->regUserMsg(pszName, static_cast<std::int16_t>(iSize), FuncCallType::Hooks);
    }

    int pfnGetPlayerUserId(edict_t *e)
    {
        return gEngine->getPlayerUserID(gEngine->getEdict(e), FuncCallType::Hooks);
    }

    const char *pfnGetPlayerAuthId(edict_t *e)
    {
        return gEngine->getPlayerAuthID(gEngine->getEdict(e), FuncCallType::Hooks).data();
    }
    
    char *pfnInfoKeyValue(char *infobuffer, const char *key)
    {
        return const_cast<char *>(gEngine->infoKeyValue(infobuffer, key, FuncCallType::Hooks).data());
    }
    
    const char *pfnCmd_Args()
    {
        return gEngine->cmdArgs(FuncCallType::Hooks).data();
    }
    
    const char *pfnCmd_Argv(int argc)
    {
        return gEngine->cmdArgv(static_cast<std::uint8_t>(argc), FuncCallType::Hooks).data();
    }

    int	pfnCmd_Argc()
    {
        return static_cast<int>(gEngine->cmdArgc(FuncCallType::Hooks));
    }

    void pfnCVarRegister(cvar_t *pCvar)
    {
        gEngine->registerCvar(pCvar);
    }

    cvar_t *pfnCVarGetPointer(const char *szVarName)
    {
        if (ICvar *cvar = gEngine->getCvar(szVarName, FuncCallType::Hooks); cvar)
        {
            return static_cast<cvar_t *>(*cvar);
        }

        return nullptr;
    }
    
    void pfnSetModel(edict_t *e, const char *m)
    {
        gEngine->setModel(gEngine->getEdict(e), m, FuncCallType::Hooks);
    }

    edict_t *pfnCreateEntity()
    {
        return static_cast<edict_t *>(*gEngine->createEntity(FuncCallType::Hooks));
    }

    void pfnRemoveEntity(edict_t *e)
    {
        gEngine->removeEntity(gEngine->getEdict(e), FuncCallType::Hooks);
    }

    void pfnServerPrint(const char *szMsg)
    {
        gEngine->print(szMsg, FuncCallType::Hooks);
    }
}
