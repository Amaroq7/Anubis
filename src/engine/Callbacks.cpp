/*
 *  Copyright (C) 2020 Metamod++ Development Team
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

#include <engine/Callbacks.hpp>
#include <Metamod.hpp>
#include <engine/Edict.hpp>

namespace Metamod::Engine::Callbacks::Direct
{
    std::uint32_t pfnPrecacheModel(std::string_view model)
    {
        return PRECACHE_MODEL(model.data());
    }

    std::uint32_t pfnPrecacheSound(std::string_view sound)
    {
        return PRECACHE_SOUND(sound.data());
    }

    void pfnChangeLevel(std::string_view level1, std::string_view level2)
    {
        CHANGE_LEVEL(level1.data(), level2.data());
    }

    void pfnServerCommand(std::string_view cmd)
    {
        SERVER_COMMAND(cmd.data());
    }

    void pfnServerExecute()
    {
        SERVER_EXECUTE();
    }

    void pfnMessageBegin(MsgDest msgDest, std::uint32_t msgType, const float *pOrigin, IEdict *edict)
    {
        MESSAGE_BEGIN(
            static_cast<int>(msgDest),
            static_cast<int>(msgType),
            pOrigin,
            *dynamic_cast<Edict *>(edict)
        );
    }

    void pfnMessageEnd()
    {
        MESSAGE_END();
    }

    void pfnWriteByte(std::byte byteArg)
    {
        WRITE_BYTE(std::to_integer<int>(byteArg));
    }

    void pfnWriteChar(char charArg)
    {
        WRITE_CHAR(static_cast<int>(charArg));
    }

    void pfnWriteShort(std::int16_t shortArg)
    {
        WRITE_SHORT(static_cast<int>(shortArg));
    }

    void pfnWriteLong(std::int32_t longArg)
    {
        WRITE_LONG(static_cast<int>(longArg));
    }

    void pfnWriteAngle(float angleArg)
    {
        WRITE_ANGLE(angleArg);
    }

    void pfnWriteCoord(float coordArg)
    {
        WRITE_COORD(coordArg);
    }

    void pfnWriteString(std::string_view strArg)
    {
        WRITE_STRING(strArg.data());
    }

    void pfnWriteEntity(std::int16_t entArg)
    {
        WRITE_ENTITY(static_cast<int>(entArg));
    }

    void pfnAddServerCommand(std::string_view cmdName, ServerCmdCallback cmdFunc)
    {
        REG_SVR_COMMAND(cmdName.data(), *cmdFunc.target<void(*)()>());
    }

    std::uint8_t pfnRegUserMsg(std::string_view pszName, std::uint8_t iSize)
    {
        std::uint8_t msgid = REG_USER_MSG(pszName.data(), iSize);
        gMetaGlobal->addNewMsg(msgid, pszName, iSize);
        return msgid;
    }
}

namespace Metamod::Engine::Callbacks::GameDLL
{
    static Funcs *engFuncs = gMetaGlobal->getEngine()->getFuncs(false);

    void pfnChangeLevel(const char *s1, const char *s2)
    {
        engFuncs->changeLevel(s1, s2 ? s2 : std::string_view());
    }

    void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
    {
        static Engine *engine = gMetaGlobal->getEngine();
        Edict *edict = engine->getEdict(ed);

        engFuncs->messageBegin(
            static_cast<MsgDest>(msg_dest),
            static_cast<std::uint32_t>(msg_type),
            pOrigin,
            edict
        );
    }

    void pfnMessageEnd()
    {
        engFuncs->messageEnd();
    }

    void pfnWriteByte(int iValue)
    {
        engFuncs->writeByte(static_cast<std::byte>(iValue));
    }

    void pfnWriteChar(int iValue)
    {
        engFuncs->writeChar(static_cast<char>(iValue));
    }

    void pfnWriteShort(int iValue)
    {
        engFuncs->writeShort(static_cast<std::int16_t>(iValue));
    }

    void pfnWriteLong(int iValue)
    {
        engFuncs->writeLong(static_cast<std::int32_t>(iValue));
    }

    void pfnWriteAngle(float flValue)
    {
        engFuncs->writeAngle(flValue);
    }

    void pfnWriteCoord(float flValue)
    {
        engFuncs->writeCoord(flValue);
    }

    void pfnWriteString(const char *sz)
    {
        engFuncs->writeString(sz);
    }

    void pfnWriteEntity(int iValue)
    {
        engFuncs->writeEntity(static_cast<std::int16_t>(iValue));
    }

    int pfnRegUserMsg(const char *pszName, int iSize)
    {
        return engFuncs->regUserMsg(pszName, static_cast<std::uint8_t>(iSize));
    }
}