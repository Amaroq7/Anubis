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
#include <engine/Engine.hpp>
#include <Metamod.hpp>
#include <engine/Funcs.hpp>

namespace Metamod::Engine
{
    Funcs::Funcs(bool direct) : m_direct(direct)
    {}

    std::uint32_t Funcs::precacheModel(std::string_view model) const
    {
        return static_cast<std::uint32_t>(PRECACHE_MODEL(model.data()));
    }

    std::uint32_t Funcs::precacheSound(std::string_view sound) const
    {
        return static_cast<std::uint32_t>(PRECACHE_SOUND(sound.data()));
    }

    void Funcs::changeLevel(std::string_view level1, std::string_view level2) const
    {
        static ChangeLevelHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->changeLevel();

        if (m_direct)
        {
            return Callbacks::Direct::pfnChangeLevel(level1, level2);
        }

        return hookchain->callChain(Callbacks::Direct::pfnChangeLevel, level1, level2);
    }

    void Funcs::serverCommand(std::string_view cmd) const
    {
        SERVER_COMMAND(cmd.data());
    }

    void Funcs::serverExecute() const
    {
        SERVER_EXECUTE();
    }

    void Funcs::registerSrvCommand(std::string_view cmd, ServerCmdCallback cb) const
    {
        REG_SVR_COMMAND(cmd.data(), *cb.target<void(*)()>());
    }

    void Funcs::messageBegin(MsgDest msgDest, std::uint32_t msgType, const float *pOrigin, IEdict *pEdict) const
    {
        static MessageBeginHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->messageBegin();

        if (m_direct)
        {
            return Callbacks::Direct::pfnMessageBegin(msgDest, msgType, pOrigin, pEdict);
        }

        return hookchain->callChain(Callbacks::Direct::pfnMessageBegin, msgDest, msgType, pOrigin, pEdict);
    }

    void Funcs::messageEnd() const
    {
        static MessageEndHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->messageEnd();

        if (m_direct)
        {
            return Callbacks::Direct::pfnMessageEnd();
        }

        return hookchain->callChain(Callbacks::Direct::pfnMessageEnd);
    }

    void Funcs::writeByte(std::byte byteArg) const
    {
        static WriteByteHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->writeByte();

        if (m_direct)
        {
            return Callbacks::Direct::pfnWriteByte(byteArg);
        }

        return hookchain->callChain(Callbacks::Direct::pfnWriteByte, byteArg);
    }

    void Funcs::writeChar(char charArg) const
    {
        static WriteCharHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->writeChar();

        if (m_direct)
        {
            return Callbacks::Direct::pfnWriteChar(charArg);
        }

        return hookchain->callChain(Callbacks::Direct::pfnWriteChar, charArg);
    }

    void Funcs::writeShort(std::int16_t shortArg) const
    {
        static WriteShortHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->writeShort();

        if (m_direct)
        {
            return Callbacks::Direct::pfnWriteShort(shortArg);
        }

        return hookchain->callChain(Callbacks::Direct::pfnWriteShort, shortArg);
    }

    void Funcs::writeLong(std::int32_t longArg) const
    {
        static WriteLongHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->writeLong();

        if (m_direct)
        {
            return Callbacks::Direct::pfnWriteLong(longArg);
        }

        return hookchain->callChain(Callbacks::Direct::pfnWriteLong, longArg);
    }

    void Funcs::writeEntity(std::int16_t entArg) const
    {
        static WriteEntityHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->writeEntity();

        if (m_direct)
        {
            return Callbacks::Direct::pfnWriteEntity(entArg);
        }

        return hookchain->callChain(Callbacks::Direct::pfnWriteEntity, entArg);
    }

    void Funcs::writeAngle(float angleArg) const
    {
        static WriteAngleHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->writeAngle();

        if (m_direct)
        {
            return Callbacks::Direct::pfnWriteAngle(angleArg);
        }

        return hookchain->callChain(Callbacks::Direct::pfnWriteAngle, angleArg);
    }

    void Funcs::writeCoord(float coordArg) const
    {
        static WriteCoordHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->writeCoord();

        if (m_direct)
        {
            return Callbacks::Direct::pfnWriteCoord(coordArg);
        }

        return hookchain->callChain(Callbacks::Direct::pfnWriteCoord, coordArg);
    }

    void Funcs::writeString(std::string_view strArg) const
    {
        static WriteStringHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->writeString();

        if (m_direct)
        {
            return Callbacks::Direct::pfnWriteString(strArg);
        }

        return hookchain->callChain(Callbacks::Direct::pfnWriteString, strArg);
    }

    std::uint8_t Funcs::regUserMsg(std::string_view name, std::uint8_t size) const
    {
        static RegUserMsgHookRegistry *hookchain = gMetaGlobal->getEngine()->getHooks()->regUserMsg();

        if (m_direct)
        {
            return Callbacks::Direct::pfnRegUserMsg(name, size);
        }

        return hookchain->callChain(Callbacks::Direct::pfnRegUserMsg, name, size);
    }
}