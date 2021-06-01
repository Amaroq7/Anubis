/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/

#pragma once

#include <IHookChains.hpp>
#include <forward_list>
#include <stdexcept>
#include <functional>
#include <memory>
#include <utility>

#if defined _WIN32
#define WIN32_LEAN_AND_MEAN
    #include "windows.h"
#elif defined __linux__
    #include <sys/mman.h>
    #include <unistd.h>
#endif

namespace Metamod
{
    template<typename t_ret, typename... t_args>
    class HookInfo final : public IHookInfo
    {
    public:
        HookInfo(HookFunc<t_ret, t_args...> func, HookPriority priority) : m_hookFunc(func), m_priority(priority), m_state(State::Enabled) {}
        ~HookInfo() override = default;
        
        void setState(State state) override
        {
            m_state = state;
        }
        
        [[nodiscard]] HookPriority getPriority() const override
        {
            return m_priority;
        }
        
        const HookFunc<t_ret, t_args...> &getHookFn() const
        {
            return m_hookFunc;
        }
        
        [[nodiscard]] bool isEnabled() const
        {
            return m_state == State::Enabled;
        }

    private:
        HookFunc<t_ret, t_args...> m_hookFunc;
        HookPriority m_priority;
        State m_state;
    };

    // Implementation for chains in modules
    template<typename t_ret, typename... t_args>
    class Hook final : public IHook<t_ret, t_args...>
    {
    public:
        Hook(typename std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>>::iterator iter,
             const std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>> &hooks,
             OriginalFunc<t_ret, t_args...> orig)
            : m_iter(iter), m_hooks(hooks), m_origFunc(orig)
        {}
        
        Hook(typename std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>>::iterator iter,
             const std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>> &hooks,
             OriginalFunc<t_ret, t_args...> orig,
             OriginalFunc<t_ret, t_args...> last)
            : m_iter(iter), m_hooks(hooks), m_origFunc(orig), m_lastFn(last)
        {}

        ~Hook() override = default;

        t_ret callNext(t_args... args) override
        {
            if (m_iter != m_hooks.end())
            {
                const auto &currentHook = *m_iter;
                
                do
                {
                    ++m_iter;
                } while (m_iter != m_hooks.end() && !(*m_iter)->isEnabled());
                
                Hook nextChain(m_iter, m_hooks, m_origFunc, m_lastFn);
                return std::invoke(currentHook->getHookFn(), &nextChain, std::forward<t_args>(args)...);
            }
            
            if (m_lastFn)
            {
                return std::invoke(m_lastFn, std::forward<t_args>(args)...);
            }

            return callOriginal(std::forward<t_args>(args)...);
        }
        
        t_ret callOriginal(t_args... args) const override
        {
            return std::invoke(m_origFunc, std::forward<t_args>(args)...);
        }

    protected:
        typename std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>>::iterator m_iter;
        const std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>> &m_hooks;
        OriginalFunc<t_ret, t_args...> m_origFunc;
        OriginalFunc<t_ret, t_args...> m_lastFn;
    };

    template<typename t_ret, typename... t_args>
    class HookRegistry final : public IHookRegistry<t_ret, t_args...>
    {
    public:
        ~HookRegistry() override = default;

        t_ret callChain(OriginalFunc<t_ret, t_args...> origFunc, t_args... args)
        {
            if (hasHooks())
            {
                auto iter = m_hooks.begin();
                    
                while (iter != m_hooks.end() && !(*iter)->isEnabled())
                {
                    ++iter;
                }

                Hook<t_ret, t_args...> chain(iter, m_hooks, origFunc);
                return chain.callNext(std::forward<t_args>(args)...);
            }

            return origFunc(std::forward<t_args>(args)...);
        }
        
        t_ret callChain(OriginalFunc<t_ret, t_args...> lastFunc, OriginalFunc<t_ret, t_args...> origFunc, t_args... args)
        {
            if (hasHooks())
            {
                auto iter = m_hooks.begin();
                
                while (iter != m_hooks.end() && !(*iter)->isEnabled())
                {
                    ++iter;
                }
                
                Hook<t_ret, t_args...> chain(iter, m_hooks, origFunc, lastFunc);
                return chain.callNext(std::forward<t_args>(args)...);
            }

            return lastFunc(std::forward<t_args>(args)...);
        }

        [[nodiscard]] bool hasHooks() const
        {
            return !m_hooks.empty();
        }

        HookInfo<t_ret, t_args...> *registerHook(HookFunc<t_ret, t_args...> hook, HookPriority priority) override
        {
            if (!hook)
            {
                // TODO: Error message
                return nullptr;
            }
            
            auto hookInfo = std::make_unique<HookInfo<t_ret, t_args...>>(hook, priority);

            if (!hasHooks())
            {
                return m_hooks.emplace_front(std::move(hookInfo)).get();
            }

            typename std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>>::iterator prevIter = m_hooks.before_begin();
            for (auto it = m_hooks.begin(); it != m_hooks.end(); ++it)
            {
                if ((*it)->getPriority() < priority)
                {
                    return (*m_hooks.emplace_after(prevIter, std::move(hookInfo))).get();
                }

                prevIter = it;
            }

            return (*m_hooks.emplace_after(prevIter, std::move(hookInfo))).get();
        }

        void unregisterHook(IHookInfo *hookInfo) override
        {
            m_hooks.remove_if([hookInfo](const std::unique_ptr<HookInfo<t_ret, t_args...>> &hook) {
                return hookInfo == hook.get();
            });
        }

    private:
        std::forward_list<std::unique_ptr<HookInfo<t_ret, t_args...>>> m_hooks;
    };

    template<typename t_ret, typename t_entity, typename... t_args>
    class ClassHookInfo final : public IHookInfo
    {
    public:
        ClassHookInfo(ClassHookFunc<t_ret, t_entity, t_args...> func, HookPriority priority) : m_hookFunc(func), m_priority(priority), m_state(State::Enabled) {}
        ~ClassHookInfo() override = default;
        
        void setState(State state) override
        {
            m_state = state;
        }
        
        [[nodiscard]] HookPriority getPriority() const override
        {
            return m_priority;
        }
        
        const ClassHookFunc<t_ret, t_entity, t_args...> &getHookFn() const
        {
            return m_hookFunc;
        }
        
        [[nodiscard]] bool isEnabled() const
        {
            return m_state == State::Enabled;
        }

    private:
        ClassHookFunc<t_ret, t_entity, t_args...> m_hookFunc;
        HookPriority m_priority;
        State m_state;
    };

    // Implementation for class chains in modules
    template<typename t_ret, typename t_entity, typename... t_args>
    class ClassHook final : public IClassHook<t_ret, t_entity, t_args...>
    {
    public:
        ClassHook(typename std::forward_list<std::unique_ptr<ClassHookInfo<t_ret, t_entity, t_args...>>>::iterator iter,
             const std::forward_list<std::unique_ptr<ClassHookInfo<t_ret, t_entity, t_args...>>> &hooks,
             ClassOriginalFunc<t_ret, t_entity, t_args...> lastFn)
            : m_iter(iter), m_hooks(hooks), m_endFunc(lastFn), m_originalFunc(lastFn)
        {}

        ClassHook(typename std::forward_list<std::unique_ptr<ClassHookInfo<t_ret, t_entity, t_args...>>>::iterator iter,
                  const std::forward_list<std::unique_ptr<ClassHookInfo<t_ret, t_entity, t_args...>>> &hooks,
                  ClassOriginalFunc<t_ret, t_entity, t_args...> lastFn,
                  ClassOriginalFunc<t_ret, t_entity, t_args...> origFn)
            : m_iter(iter), m_hooks(hooks), m_endFunc(lastFn), m_originalFunc(origFn)
        {}

        ~ClassHook() final = default;

        t_ret callNext(t_entity entity, t_args... args) final
        {
            if (m_iter != m_hooks.end())
            {
                const auto &currentHook = *m_iter;
                
                do
                {
                    ++m_iter;
                } while (m_iter != m_hooks.end() && !(*m_iter)->isEnabled());
                
                ClassHook nextChain(m_iter, m_hooks, m_endFunc);
                return std::invoke(currentHook->getHookFn(), &nextChain, entity, std::forward<t_args>(args)...);
            }

            return std::invoke(m_endFunc, entity, std::forward<t_args>(args)...);
        }

        t_ret callOriginal(t_entity entity, t_args... args) const override
        {
            return std::invoke(m_originalFunc, entity, std::forward<t_args>(args)...);
        }

    private:
        typename std::forward_list<std::unique_ptr<ClassHookInfo<t_ret, t_entity, t_args...>>>::iterator m_iter;
        const std::forward_list<std::unique_ptr<ClassHookInfo<t_ret, t_entity, t_args...>>> &m_hooks;
        ClassOriginalFunc<t_ret, t_entity, t_args...> m_endFunc;
        ClassOriginalFunc<t_ret, t_entity, t_args...> m_originalFunc;
    };

    template<typename t_ret, typename t_entity, typename... t_args>
    class ClassHookRegistry final : public IClassHookRegistry<t_ret, t_entity, t_args...>
    {
    public:
        ClassHookRegistry(std::function<void()> registerFn, std::function<void()> unregisterFn)
        : m_registerFn(std::move(registerFn)), m_unregisterFn(std::move(unregisterFn)) {}

        ClassHookRegistry(std::intptr_t vTable, std::uint32_t offset, std::intptr_t callbackFn)
            : m_vTable(vTable), m_vOffset(offset), m_vCallback(callbackFn), m_hookVTable(true) {}
        ~ClassHookRegistry() final
        {
            if (!m_hooks.empty())
            {
                _restoreOriginalVFunc();
            }
        }

        t_ret callChain(std::function<t_ret(t_entity, t_args...)> lastFn, t_entity entity, t_args... args)
        {
            auto iter = m_hooks.begin();
                
            while (iter != m_hooks.end() && !(*iter)->isEnabled())
            {
                ++iter;
            }
                
            ClassHook<t_ret, t_entity, t_args...> chain(iter, m_hooks, lastFn);
            return chain.callNext(entity, std::forward<t_args>(args)...);
        }

        t_ret callChain(ClassOriginalFunc<t_ret, t_entity, t_args...> lastFunc,
                        ClassOriginalFunc<t_ret, t_entity, t_args...> origFunc,
                        t_entity entity, t_args... args)
        {
            if (hasHooks())
            {
                auto iter = m_hooks.begin();

                while (iter != m_hooks.end() && !(*iter)->isEnabled())
                {
                    ++iter;
                }

                ClassHook<t_ret, t_entity, t_args...> chain(iter, m_hooks, lastFunc, origFunc);
                return chain.callNext(entity, std::forward<t_args>(args)...);
            }

            return lastFunc(entity, std::forward<t_args>(args)...);
        }

        [[nodiscard]] bool hasHooks() const
        {
            return !m_hooks.empty();
        }

        ClassHookInfo<t_ret, t_entity, t_args...> *registerHook(ClassHookFunc<t_ret, t_entity, t_args...> hook, HookPriority priority) final
        {
            using namespace std::string_literals;
            if (!hook)
            {
                // TODO: Error message
                return nullptr;
            }

            auto hookInfo = std::make_unique<ClassHookInfo<t_ret, t_entity, t_args...>>(hook, priority);
            if (!hasHooks())
            {
                if (m_hookVTable)
                {
                    std::intptr_t vFunc = m_vTable + sizeof(std::intptr_t) * m_vOffset;

                    m_origVFunc = *reinterpret_cast<std::intptr_t *>(vFunc);
                    std::int32_t memProtection = _unprotect(vFunc);
                    *(reinterpret_cast<std::intptr_t *>(vFunc)) = m_vCallback;
                    _protect(vFunc, memProtection);
                }
                else if (m_registerFn)
                {
                    std::invoke(m_registerFn);
                }
                else
                {
                    return nullptr;
                }
                return m_hooks.emplace_front(std::move(hookInfo)).get();
            }

            typename std::forward_list<std::unique_ptr<ClassHookInfo<t_ret, t_entity, t_args...>>>::iterator prevIter = m_hooks.before_begin();
            for (auto it = m_hooks.begin(); it != m_hooks.end(); ++it)
            {
                if ((*it)->getPriority() < priority)
                {
                    return (*m_hooks.emplace_after(prevIter, std::move(hookInfo))).get();
                }

                prevIter = it;
            }

            return (*m_hooks.emplace_after(prevIter, std::move(hookInfo))).get();
        }

        void unregisterHook(IHookInfo *hookInfo) final
        {
            if (!hookInfo)
            {
                return;
            }

            bool wereHooksPresent = !m_hooks.empty();
            m_hooks.remove_if([hookInfo](const std::unique_ptr<ClassHookInfo<t_ret, t_entity, t_args...>> &hook) {
                return hookInfo == hook.get();
            });

            bool shouldRemoveHook = wereHooksPresent && m_hooks.empty();
            if (shouldRemoveHook)
            {
                _restoreOriginalVFunc();
            }
        }

        [[nodiscard]] std::intptr_t getVFuncAddr() const
        {
            return m_origVFunc;
        }

    private:
        static void _protect(std::intptr_t region, std::int32_t protection)
        {
#if defined _WIN32
            MEMORY_BASIC_INFORMATION mbi;
            VirtualQuery(reinterpret_cast<void *>(region), &mbi, sizeof(mbi));
            VirtualProtect(mbi.BaseAddress, mbi.RegionSize, protection, &mbi.Protect);
#elif defined __linux__
            static std::int32_t pageSize = sysconf(_SC_PAGE_SIZE);
            mprotect(reinterpret_cast<void *>(region & ~(pageSize - 1)), pageSize, protection);
#endif
        }

        static std::int32_t _unprotect(std::intptr_t region)
        {
#if defined _WIN32
            MEMORY_BASIC_INFORMATION mbi;
            VirtualQuery(reinterpret_cast<void *>(region), &mbi, sizeof(mbi));
            VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);
            return mbi.Protect;
#elif defined __linux__
            static std::int32_t pageSize = sysconf(_SC_PAGE_SIZE);
            mprotect(reinterpret_cast<void *>(region & ~(pageSize - 1)), pageSize,
                     PROT_READ | PROT_WRITE | PROT_EXEC);
            return PROT_READ | PROT_EXEC;
#endif
        }

    private:
        void _restoreOriginalVFunc()
        {
            if (m_hooks.empty())
            {
                return;
            }

            if (m_origVFunc)
            {
                std::intptr_t vFunc = m_vTable + sizeof(std::intptr_t) * m_vOffset;
                std::int32_t memProtection = _unprotect(vFunc);
                *(reinterpret_cast<std::intptr_t *>(vFunc)) = m_origVFunc;
                _protect(vFunc, memProtection);

                m_origVFunc = 0;
            }
            else if (m_unregisterFn)
            {
                std::invoke(m_unregisterFn);
            }
        }

    private:
        std::intptr_t m_vTable = 0;
        std::uint32_t m_vOffset = 0;
        std::intptr_t m_vCallback = 0;
        std::intptr_t m_origVFunc = 0;
        std::function<void()> m_registerFn;
        std::function<void()> m_unregisterFn;
        std::forward_list<std::unique_ptr<ClassHookInfo<t_ret, t_entity, t_args...>>> m_hooks;
        bool m_hookVTable = false;
    };
}
