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

#include <IAnubis.hpp>
#include <game/IEntityHolder.hpp>
#include <game/IEntitiesHooks.hpp>

#include <functional>
#include <filesystem>
#include <memory>

#if defined __linux__
    #include <dlfcn.h>
    #include <unistd.h>
#elif defined _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

class CGameRules;

namespace Anubis
{
    class Module
    {
    public:
        using SystemHandle = void *;

    public:
        explicit Module(const std::filesystem::path &path);
        Module(IPlugin::Type type, const std::filesystem::path &path);

        [[nodiscard]] bool initPlugin(nstd::observer_ptr<IAnubis> api) const;
        void installVFHooks() const;
        void deinitPlugin() const;
        void setupHook(Game::SetupHookType setupHookType, std::function<void(std::any)> hook) const;
        [[nodiscard]] IPlugin::Type getType() const;
        [[nodiscard]] std::string_view getName() const;
        [[nodiscard]] std::string_view getVersion() const;
        [[nodiscard]] std::string_view getDate() const;
        [[nodiscard]] std::string_view getUrl() const;
        [[nodiscard]] std::string_view getAuthor() const;
        void setPath(std::filesystem::path &&path);

    public:
        template<typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
        T getSymbol(std::string_view name)
        {
#if defined __linux__
            return reinterpret_cast<T>(dlsym(m_libHandle.get(), name.data()));
#elif defined _WIN32
            return reinterpret_cast<T>(GetProcAddress(reinterpret_cast<HMODULE>(m_libHandle.get()), name.data()));
#endif
        }

        explicit operator SystemHandle();

    private:
#if defined _WIN32
        static constexpr std::string_view FnQuerySgn = "?Query@Anubis@@YA?AV?$observer_ptr@VIPlugin@Anubis@@@nstd@@XZ";
        static constexpr std::string_view FnInitSgn = "?Init@Anubis@@YA_NV?$observer_ptr@VIAnubis@Anubis@@@nstd@@@Z";
        static constexpr std::string_view FnShutdownSgn = "?Shutdown@Anubis@@YAXXZ";
        static constexpr std::string_view FnInstallVFHooksSgn = "?InstallVHooks@Anubis@@YAXXZ";
        static constexpr std::string_view FnSetupHookSgn =
            "?GetGameRules@Game@Anubis@@YAXV?$function@$$A6AXV?$observer_ptr@VCGameRules@@@nstd@@@Z@std@@V?$observer_"
            "ptr@VCGameRules@@@nstd@@@Z";
#else
        static constexpr std::string_view FnQuerySgn = "_ZN6Anubis5QueryEv";
        static constexpr std::string_view FnInitSgn = "_ZN6Anubis4InitEN4nstd12observer_ptrINS_7IAnubisEEE";
        static constexpr std::string_view FnShutdownSgn = "_ZN6Anubis8ShutdownEv";
        static constexpr std::string_view FnInstallVFHooksSgn = "_ZN6Anubis13InstallVHooksEv";
        static constexpr std::string_view FnSetupHookSgn =
            "_ZN6Anubis4Game9SetupHookENS0_13SetupHookTypeESt8functionIFvSt3anyEE";
#endif

    private:
        static std::string_view _getError();

    private:
        void _findPluginFuncs();
        void _queryPlugin();

    private:
        IPlugin::Type m_plType = IPlugin::Type::Extension;
        std::string m_plName;
        std::string m_plVersion;
        std::string m_plDate;
        std::string m_plAuthor;
        std::string m_plUrl;
        fnQuery m_queryFn = nullptr;
        fnInit m_initFn = nullptr;
        fnShutdown m_shutdownFn = nullptr;
        fnInstallVHooks m_installVFHooks = nullptr;
        fnSetupHook m_setupHookFn = nullptr;
        std::unique_ptr<void, std::function<void(SystemHandle)>> m_libHandle;
    };
} // namespace Anubis