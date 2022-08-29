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

#include "Anubis.hpp"
#include <AnubisInfo.hpp>

#include <fmt/format.h>
#include <fmt/color.h>

#include <extdll.h>

#include <cstdlib>
#include <thread>

cvar_t gAnubisLogLevelCvar = {"anubis_loglevel", const_cast<char *>("info"), FCVAR_SERVER, 0.0f, nullptr};

namespace
{
    template<typename... t_args>
    void serverPrint(nstd::observer_ptr<enginefuncs_t> engFuncs, std::string_view format, t_args... args)
    {
        std::invoke(engFuncs->pfnServerPrint, fmt::vformat(format, fmt::make_format_args(format, args...)).c_str());
    }

    template<typename... t_args>
    void serverPrint(nstd::observer_ptr<enginefuncs_t> engFuncs,
                     const fmt::text_style &ts,
                     std::string_view format,
                     t_args... args)
    {
        std::invoke(engFuncs->pfnServerPrint, fmt::vformat(ts, format, fmt::make_format_args(format, args...)).c_str());
    }

    template<typename... t_args>
    void serverPrint(std::string_view format, t_args... args)
    {
        static nstd::observer_ptr<Anubis::Engine::ILibrary> engLib = Anubis::gAnubisApi->getEngine();
        engLib->print(fmt::vformat(format, fmt::make_format_args(format, args...)), Anubis::FuncCallType::Direct);
    };

    void printStartUpMsg(nstd::observer_ptr<enginefuncs_t> engFuncs)
    {
        using namespace fmt::literals;
        fmt::text_style textStyle(fg(fmt::terminal_color::bright_green));

        serverPrint(engFuncs, "\n");
        serverPrint(engFuncs, textStyle, "   {} version {}  Copyright (c) 2020-{} {}\n", ANUBIS_NAME, ANUBIS_VERSION,
                    CURRENT_YEAR, ANUBIS_AUTHOR);
        serverPrint(engFuncs, textStyle, "   {} comes with ABSOLUTELY NO WARRANTY; for details type `anubis gpl'.\n",
                    ANUBIS_NAME);
        serverPrint(engFuncs, textStyle, "   This is free software, and you are welcome to redistribute it\n");
        serverPrint(engFuncs, textStyle, "   under certain conditions; type `anubis gpl' for details.\n");
        serverPrint(engFuncs, "\n");

        serverPrint(engFuncs, textStyle, "{} v{}  {}\n", ANUBIS_NAME, ANUBIS_VERSION, __DATE__);
        serverPrint(engFuncs, textStyle, "by {}", ANUBIS_AUTHOR);
        serverPrint(engFuncs, textStyle, "   {}\n", ANUBIS_HOMEPAGE);
        serverPrint(engFuncs, textStyle, "compiled: {} ({})\n", __TIME__ " " __DATE__, ANUBIS_RELEASE_MODE);

        const auto [majorVer, minorVer] = Anubis::gAnubisApi->getEngine()->getReHLDSVersion();
        serverPrint(engFuncs, textStyle, "ReHLDS API: {}.{}\n", majorVer, minorVer);
    }

    void anubisSrvCmd()
    {
        static nstd::observer_ptr<Anubis::Engine::ILibrary> engLib = Anubis::gAnubisApi->getEngine();

        static auto printUsage = []()
        {
            serverPrint("Usage: anubis <command>\n");
            serverPrint("Valid commands are:\n");
            serverPrint("   gpl              - display license\n");
            serverPrint("   version          - display anubis version info\n");
            serverPrint("   list             - list currently loaded extensions\n");
        };

        if (engLib->cmdArgc(Anubis::FuncCallType::Direct) == 1)
        {
            printUsage();
            return;
        }

        std::string_view cmd = engLib->cmdArgv(1, Anubis::FuncCallType::Direct);

        if (cmd == "gpl")
        {
            serverPrint("{} version {} {}\n", ANUBIS_NAME, ANUBIS_VERSION, __DATE__);
            serverPrint("Copyright (c) 2020-{} {}\n", CURRENT_YEAR, ANUBIS_AUTHOR);
            serverPrint("");
            serverPrint("   {} is free software; you can redistribute it and/or\n", ANUBIS_NAME);
            serverPrint("   modify it under the terms of the GNU General Public License\n");
            serverPrint("   as published by the Free Software Foundation; either\n");
            serverPrint("   version 3 of the License, or (at your option) any later\n");
            serverPrint("   version.\n");
            serverPrint("   \n");
            serverPrint("   {} is distributed in the hope that it will be useful,\n", ANUBIS_NAME);
            serverPrint("   but WITHOUT ANY WARRANTY; without even the implied warranty\n");
            serverPrint("   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
            serverPrint("   See the GNU General Public License for more details.\n");
            serverPrint("   \n");
            serverPrint("   You should have received a copy of the GNU General Public\n");
            serverPrint("   License along with {}; if not, write to the Free\n", ANUBIS_NAME);
            serverPrint("   Software Foundation, Inc., 59 Temple Place, Suite 330,\n");
            serverPrint("   Boston, MA 02111-1307  USA\n");
            serverPrint("   \n");
            serverPrint("   In addition, as a special exception, the author gives\n");
            serverPrint("   permission to link the code of this program with the\n");
            serverPrint(R"(   Half-Life Game Engine ("HL Engine") and Modified Game)");
            serverPrint("\n");
            serverPrint(R"(   Libraries("MODs") developed by Valve, L.L.C("Valve").)");
            serverPrint("\n");
            serverPrint("   You must obey the GNU General Public License in all\n");
            serverPrint("   respects for all of the code used other than the HL Engine\n");
            serverPrint("   and MODs from Valve.  If you modify this file, you may\n");
            serverPrint("   extend this exception to your version of the file, but you\n");
            serverPrint("   are not obligated to do so.  If you do not wish to do so,\n");
            serverPrint("   delete this exception statement from your version.\n");
        }
        else if (cmd == "version")
        {
            Anubis::gAnubisApi->printInfo();
        }
        else if (cmd == "list")
        {
            Anubis::gAnubisApi->printPluginList();
        }
        else
        {
            printUsage();
        }
    }
} // namespace

extern "C" ANUBIS_API void
#if defined _WIN32
    __stdcall
#endif
    GiveFnptrsToDll(enginefuncs_t *pengfuncsFromEngine, globalvars_t *pGlobals)
{
    auto engineFuncs = std::make_unique<enginefuncs_t>();
    *engineFuncs = *pengfuncsFromEngine;

    try
    {
        char gameDir[MAX_PATH];
        engineFuncs->pfnGetGameDir(gameDir);
        auto anubisConfig = std::make_unique<Anubis::Config>(gameDir);

        std::string_view initLogLevel {anubisConfig->getInitLogLevel()};

        Anubis::gAnubisApi = std::make_unique<Anubis::Anubis>(std::move(anubisConfig));

        engineFuncs->pfnCVarRegister(&gAnubisLogLevelCvar);
        engineFuncs->pfnCvar_DirectSet(&gAnubisLogLevelCvar, initLogLevel.data());

        engineFuncs->pfnAddServerCommand("anubis", anubisSrvCmd);
    }
    catch (const std::exception &e)
    {
        using namespace std::chrono_literals;

        fmt::text_style textStyle(fmt::emphasis::bold | fg(fmt::terminal_color::bright_red));
        serverPrint(pengfuncsFromEngine, textStyle, "[{}] FATAL ERROR: {}\n", Anubis::Anubis::LOG_TAG, e.what());
        std::this_thread::sleep_for(3s);
        std::exit(EXIT_FAILURE);
    }

    nstd::observer_ptr<globalvars_t> engGlobals {pGlobals};

    Anubis::gAnubisApi->initEngine(std::move(engineFuncs), engGlobals);

    printStartUpMsg(pengfuncsFromEngine);

    Anubis::gAnubisApi->initLogger();

    try
    {
        Anubis::gAnubisApi->initGameDLL();
    }
    catch (const std::runtime_error &e)
    {
        using namespace std::chrono_literals;

        fmt::text_style textStyle(fmt::emphasis::bold | fg(fmt::terminal_color::bright_red));
        serverPrint(pengfuncsFromEngine, textStyle, "[{}] FATAL ERROR: {}\n", Anubis::Anubis::LOG_TAG, e.what());
        std::this_thread::sleep_for(3s);
        std::exit(EXIT_FAILURE);
    }

    Anubis::gAnubisApi->loadPlugins();
}
