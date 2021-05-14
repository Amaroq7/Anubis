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

#include <IMetamod.hpp>

#include "game/Library.hpp"
#include "engine/Library.hpp"
#include "MetaConfig.hpp"

#include <fmt/format.h>

#include <memory>

namespace Metamod
{
    constexpr const char *LOG_TAG = "MM-CPP";

    class Metamod final : public IMetamod
    {
    public:
        Metamod(std::unique_ptr<Config> &&config);
        ~Metamod();

        std::uint32_t getInterfaceVersion() const final;
        Engine::Library *getEngine() const final;
        Game::Library *getGame() const final;
        const RegMsg *getMsgInfo(std::string_view name) const final;
        void logMsg(IPlugin *plugin, LogLevel level, LogDest dest, std::string_view msg) final;

        bool addNewMsg(Engine::MsgType id, std::string_view name, std::int16_t size);
        bool setLogLevel(std::string_view logLevel);

        template<typename... t_args>
        void logMsg(LogLevel level, LogDest dest, std::string_view format, t_args&&... args)
        {
            if (level < m_config->getLogLevel())
            {
                return;
            }

            std::string msg = fmt::vformat(format, fmt::make_args_checked<t_args...>(format, args...));
            auto sendToFileFn = std::bind(&Metamod::_sendToFile, m_config->getPath(PathType::Logs),
                                          std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

            if (dest & LogDest::Console)
            {
                _sendToConsole(LOG_TAG, level, msg);
            }
            if (dest & LogDest::File)
            {
                sendToFileFn(LOG_TAG, level, msg);
            }
        }

    private:
        bool _findMessage(Engine::MsgType id);
        void _loadPlugins();
        static void _sendToConsole(std::string_view logTag, LogLevel level, std::string_view msg);
        static void _sendToFile(const fs::path &logsPath, std::string_view logTag, LogLevel level, std::string_view msg);

    private:
        std::unique_ptr<Config> m_config;
        std::unique_ptr<Engine::Library> m_engineLib;
        std::unique_ptr<Game::Library> m_gameLib;
        std::vector<std::unique_ptr<Module>> m_plugins;
        std::array<RegMsg, 256> m_regMsgs = {
            RegMsg{"svc_bad", -1},
            RegMsg{"svc_nop", -1},
            RegMsg{"svc_disconnect", -1},
            RegMsg{"svc_event", -1},
            RegMsg{"svc_version", -1},
            RegMsg{"svc_setview", -1},
            RegMsg{"svc_sound", -1},
            RegMsg{"svc_time", -1},
            RegMsg{"svc_print", -1},
            RegMsg{"svc_stufftext", -1},
            RegMsg{"svc_setangle", -1},
            RegMsg{"svc_serverinfo", -1},
            RegMsg{"svc_lightstyle", -1},
            RegMsg{"svc_updateuserinfo", -1},
            RegMsg{"svc_deltadescription", -1},
            RegMsg{"svc_clientdata", -1},
            RegMsg{"svc_stopsound", -1},
            RegMsg{"svc_pings", -1},
            RegMsg{"svc_particle", -1},
            RegMsg{"svc_damage", -1},
            RegMsg{"svc_spawnstatic", -1},
            RegMsg{"svc_event_reliable", -1},
            RegMsg{"svc_spawnbaseline", -1},
            RegMsg{"svc_temp_entity", -1},
            RegMsg{"svc_setpause", -1},
            RegMsg{"svc_signonnum", -1},
            RegMsg{"svc_centerprint", -1},
            RegMsg{"svc_killedmonster", -1},
            RegMsg{"svc_foundsecret", -1},
            RegMsg{"svc_spawnstaticsound", -1},
            RegMsg{"svc_intermission", -1},
            RegMsg{"svc_finale", -1},
            RegMsg{"svc_cdtrack", -1},
            RegMsg{"svc_restore", -1},
            RegMsg{"svc_cutscene", -1},
            RegMsg{"svc_weaponanim", -1},
            RegMsg{"svc_decalname", -1},
            RegMsg{"svc_roomtype", -1},
            RegMsg{"svc_addangle", -1},
            RegMsg{"svc_newusermsg", -1},
            RegMsg{"svc_packetentities", -1},
            RegMsg{"svc_deltapacketentities", -1},
            RegMsg{"svc_choke", -1},
            RegMsg{"svc_resourcelist", -1},
            RegMsg{"svc_newmovevars", -1},
            RegMsg{"svc_resourcerequest", -1},
            RegMsg{"svc_customization", -1},
            RegMsg{"svc_crosshairangle", -1},
            RegMsg{"svc_soundfade", -1},
            RegMsg{"svc_filetxferfailed", -1},
            RegMsg{"svc_hltv", -1},
            RegMsg{"svc_director", -1},
            RegMsg{"svc_voiceinit", -1},
            RegMsg{"svc_voicedata", -1},
            RegMsg{"svc_sendextrainfo", -1},
            RegMsg{"svc_timescale", -1},
            RegMsg{"svc_resourcelocation", -1},
            RegMsg{"svc_sendcvarvalue", -1},
            RegMsg{"svc_sendcvarvalue2", -1},
        };
    };
    extern std::unique_ptr<Metamod> gMetaGlobal;
}
