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

#include "EngineExports.hpp"
#include "Metamod.hpp"
#include <MetaInfo.hpp>

#include <fmt/format.h>
#include <fmt/color.h>

#include <extdll.h>
#include <enginecallback.h>

#include <engine/ILibrary.hpp>

enginefuncs_t g_engfuncs;
globalvars_t *gpGlobals;

cvar_t gMetaLogLevelCvar = {"meta_loglevel", const_cast<char *>("info"), FCVAR_SERVER, 0.0f, nullptr};

namespace
{
    template<typename... Args>
    void serverPrint(std::string_view format, Args&&... args)
    {
      std::invoke(
          g_engfuncs.pfnServerPrint,
          fmt::vformat(format, fmt::make_args_checked<Args...>(format, args...)).c_str()
      );
    }

    template<typename... Args>
    void serverPrint(const fmt::text_style &ts, std::string_view format, Args&&... args)
    {
        std::invoke(
            g_engfuncs.pfnServerPrint,
            fmt::vformat(ts, format, fmt::make_args_checked<Args...>(format, args...)).c_str()
        );
    }

    void printInfoMsg()
    {
        using namespace fmt::literals;
        fmt::text_style textStyle(fg(fmt::terminal_color::bright_green));

        serverPrint("\n");
        serverPrint(textStyle, "   {} version {}  Copyright (c) 2020-{} {}\n", META_NAME, META_VERSION, CURRENT_YEAR,
                    META_AUTHOR);
        serverPrint(textStyle, "   {} comes with ABSOLUTELY NO WARRANTY; for details type `meta gpl'.\n", META_NAME);
        serverPrint(textStyle, "   This is free software, and you are welcome to redistribute it\n");
        serverPrint(textStyle, "   under certain conditions; type `meta gpl' for details.\n");
        serverPrint("\n");

        serverPrint(textStyle,"{} v{}  {}\n", META_NAME, META_VERSION, __DATE__);
        serverPrint(textStyle, "by {}", META_AUTHOR);
        serverPrint(textStyle,"   {}\n", META_HOMEPAGE);
        serverPrint(textStyle, "compiled: {} ({})\n", __TIME__ " " __DATE__, META_RELEASE_MODE);

        const auto& [majorVer, minorVer] = Metamod::gMetaGlobal->getEngine()->getReHLDSVersion();
        serverPrint(textStyle, "ReHLDS API: {}.{}\n", majorVer, minorVer);
    }
}

C_DLLEXPORT void WINAPI GiveFnptrsToDll(enginefuncs_t *pengfuncsFromEngine, globalvars_t *pGlobals)
{
    using namespace Metamod;
    g_engfuncs = *pengfuncsFromEngine;
    gpGlobals = pGlobals;

    try
    {
        char gameDir[MAX_PATH];
        GET_GAME_DIR(gameDir);
        auto metaConfig = std::make_unique<::Metamod::Config>(gameDir);

        CVAR_REGISTER(&gMetaLogLevelCvar);

        gMetaGlobal = std::make_unique<::Metamod::Metamod>(std::move(metaConfig));
        printInfoMsg();
    }
    catch (const std::exception &e)
    {
        fmt::text_style textStyle(fmt::emphasis::bold | fg(fmt::terminal_color::bright_red));
        serverPrint(textStyle, "[{}] FATAL ERROR: {}\n", LOG_TAG, e.what());
        throw;
    }
}
