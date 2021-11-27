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

#include "Module.hpp"
#include <AnubisInfo.hpp>

#include <fmt/format.h>

namespace Anubis
{
    Module::Module(const std::filesystem::path &path)
    {
        using namespace std::string_literals;

        if (!std::filesystem::exists(path))
        {
#if defined __linux__
            throw std::runtime_error("Plugin not found "s + path.c_str());
#else
            throw std::runtime_error("Plugin not found "s + path.string().c_str());
#endif
        }

#if defined __linux__
        m_libHandle = std::unique_ptr<void, std::function<void(void *)>>(
            dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND),
            [](void *lib)
            {
                if (lib)
                {
                    dlclose(lib);
                }
            });
#elif defined _WIN32
        m_libHandle =
            std::unique_ptr<void, std::function<void(void *)>>(LoadLibraryW(path.c_str()),
                                                               [](void *lib)
                                                               {
                                                                   if (lib)
                                                                   {
                                                                       FreeLibrary(reinterpret_cast<HMODULE>(lib));
                                                                   }
                                                               });
#endif

        if (!m_libHandle || !_getError().empty())
        {
            throw std::runtime_error(_getError().data());
        }
    }

    Module::Module(IPlugin::Type type, const std::filesystem::path &path)
    {
        using namespace std::string_literals;

        if (!std::filesystem::exists(path))
        {
#if defined __linux__
            throw std::runtime_error("Plugin not found "s + path.c_str());
#else
            throw std::runtime_error("Plugin not found "s + path.string().c_str());
#endif
        }

#if defined __linux__
        m_libHandle = std::unique_ptr<void, std::function<void(void *)>>(
            dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND),
            [](void *lib)
            {
                if (lib)
                {
                    dlclose(lib);
                }
            });
#elif defined _WIN32
        m_libHandle =
            std::unique_ptr<void, std::function<void(void *)>>(LoadLibraryW(path.c_str()),
                                                               [](void *lib)
                                                               {
                                                                   if (lib)
                                                                   {
                                                                       FreeLibrary(reinterpret_cast<HMODULE>(lib));
                                                                   }
                                                               });
#endif

        if (!m_libHandle || !_getError().empty())
        {
            throw std::runtime_error(_getError().data());
        }

        _queryPlugin();

        if (m_plType != type)
        {
            throw std::runtime_error("Invalid plugin type");
        }

        _findPluginFuncs();
    }

    std::string_view Module::_getError()
    {
#if defined __linux__
        if (char *errorMsg = dlerror(); errorMsg)
        {
            return std::string_view {errorMsg};
        }

        return {""};
#elif defined _WIN32
        static char errorMsg[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(), 0, errorMsg,
                      sizeof(errorMsg), nullptr);

        return errorMsg;
#endif
    }

    Module::operator SystemHandle()
    {
        return m_libHandle.get();
    }

    IPlugin::Type Module::getType() const
    {
        return m_plType;
    }

    std::string_view Module::getName() const
    {
        return m_plName;
    }

    std::string_view Module::getVersion() const
    {
        return m_plVersion;
    }

    std::string_view Module::getDate() const
    {
        return m_plDate;
    }

    std::string_view Module::getUrl() const
    {
        return m_plUrl;
    }

    std::string_view Module::getAuthor() const
    {
        return m_plAuthor;
    }

    void Module::_queryPlugin()
    {
        m_queryFn = getSymbol<fnQuery>(Module::FnQuerySgn);
        if (!m_queryFn)
        {
            throw std::runtime_error("Anubis::Query function not found");
        }

        const auto &plInfo = std::invoke(m_queryFn);
        if (!plInfo)
        {
            throw std::runtime_error("Could not get plugin info");
        }

        InterfaceVersion anubisInterfaceVersion = IAnubis::VERSION;
        InterfaceVersion pluginInterfaceVersion = plInfo->getInterfaceVersion();

        auto extractMajorVer = [](std::uint32_t version)
        {
            return (version >> 16) & 0xFFFF;
        };

        auto extractMinorVer = [](std::uint32_t version)
        {
            return version & 0xFFFF;
        };

        if (extractMajorVer(anubisInterfaceVersion) != extractMajorVer(pluginInterfaceVersion))
        {
            throw std::runtime_error(fmt::format("Major version mismatch. {} is not compatible with this version of {}"
                                                 "\n    Requested {} got {}",
                                                 plInfo->getName(), ANUBIS_NAME,
                                                 extractMajorVer(pluginInterfaceVersion),
                                                 extractMajorVer(anubisInterfaceVersion)));
        }
        else if (extractMinorVer(anubisInterfaceVersion) < extractMinorVer(pluginInterfaceVersion))
        {
            throw std::runtime_error(fmt::format("Minor version mismatch. {} is not compatible with this version of {}"
                                                 "\n    Requested at least {} got {}",
                                                 plInfo->getName(), ANUBIS_NAME,
                                                 extractMinorVer(pluginInterfaceVersion),
                                                 extractMinorVer(anubisInterfaceVersion)));
        }

        m_plName = plInfo->getName();
        m_plAuthor = plInfo->getAuthor();
        m_plDate = plInfo->getAuthor();
        m_plType = plInfo->getType();
        m_plDate = plInfo->getDate();
        m_plVersion = plInfo->getVersion();
    }

    bool Module::initPlugin(nstd::observer_ptr<IAnubis> api) const
    {
        return std::invoke(m_initFn, api);
    }

    void Module::_findPluginFuncs()
    {
        m_initFn = getSymbol<fnInit>(Module::FnInitSgn);
        if (!m_initFn)
        {
            throw std::runtime_error("Anubis::Init function not found");
        }

        m_shutdownFn = getSymbol<fnShutdown>(Module::FnShutdownSgn);
        if (!m_shutdownFn)
        {
            throw std::runtime_error("Anubis::Shutdown function not found");
        }

        m_installVFHooks = getSymbol<fnInstallVHooks>(Module::FnInstallVFHooksSgn);

        if (m_plType == IPlugin::Type::EntityDLL)
        {
            if (!m_installVFHooks)
            {
                throw std::runtime_error("Anubis::InstallVHooks function not found");
            }

            m_basePlayerHooksFn = getSymbol<fnBasePlayerHooks>(Module::FnBasePlayerHooksSgn);
            if (!m_basePlayerHooksFn)
            {
                throw std::runtime_error("Anubis::Game::BasePlayerHooks function not found");
            }

            m_getEntityHolderFn = getSymbol<fnEntityHolder>(Module::FnEntityHolderSgn);
            if (!m_getEntityHolderFn)
            {
                throw std::runtime_error("Anubis::Game::EntityHolder function not found");
            }
        }
    }

    void Module::deinitPlugin() const
    {
        std::invoke(m_shutdownFn);
    }

    nstd::observer_ptr<Game::IBasePlayerHooks> Module::getCBasePlayerHooks() const
    {
        if (m_basePlayerHooksFn)
            return std::invoke(m_basePlayerHooksFn);

        return {};
    }

    nstd::observer_ptr<Game::IEntityHolder> Module::getEntityHolder() const
    {
        if (m_getEntityHolderFn)
            return std::invoke(m_getEntityHolderFn);

        return {};
    }

    void Module::installVFHooks() const
    {
        if (m_installVFHooks)
            std::invoke(m_installVFHooks);
    }

    void Module::setPath(std::filesystem::path &&path)
    {
        auto plInfo = std::invoke(m_queryFn);
        plInfo->setPath(std::move(path));
    }
} // namespace Anubis
