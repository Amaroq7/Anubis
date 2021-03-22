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

#include <Module.hpp>

namespace Metamod
{
    Module::Module(const fs::path &path)
    {
        using namespace std::string_literals;
#if defined __linux__
        m_libHandle = std::unique_ptr<void, std::function<void(void *)>>(
            dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND), [](void *lib) {
                if (lib)
                {
                    dlclose(lib);
                }
            });
#elif defined _WIN32
        m_libHandle = LoadLibraryW(path.c_str());
#endif
    }

    bool Module::isLoaded() const
    {
        return m_libHandle != nullptr;
    }

    void Module::unload()
    {
        if (m_libHandle)
        {
#if defined __linux__
            dlclose(m_libHandle.get());
#elif defined _WIN32
            FreeLibrary(m_libHandle);
#endif
            m_libHandle = nullptr;
        }
    }

    std::string_view Module::getError()
    {
#if defined __linux__
        return dlerror();
#elif defined _WIN32
        static char errorMsg[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      nullptr,
                      GetLastError(),
                      0,
                      errorMsg,
                      sizeof(errorMsg),
                      nullptr);

        return errorMsg;
#endif
    }
    Module::operator SystemHandle()
    {
        return m_libHandle
#if defined __linux__
            .get()
#endif
            ;
    }

#if defined _WIN32
    Module::~Module()
    {
        if (m_libHandle)
        {
            FreeLibrary(m_libHandle);
        }
    }
#endif
}
