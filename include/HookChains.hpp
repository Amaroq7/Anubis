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

namespace Metamod
{
    template<typename t_ret, typename... t_args>
    struct HookFuncHolder
    {
        HookFuncHolder(HookFunc<t_ret, t_args...> func, HookPriority priority) : m_hookFunc(func), m_priority(priority)
        {
        }

        HookFunc<t_ret, t_args...> m_hookFunc;
        HookPriority m_priority;
    };

    // Implementation for chains in modules
    template<typename t_ret, typename... t_args>
    class Hook : public IHook<t_ret, t_args...>
    {
    public:
        Hook(typename std::forward_list<HookFuncHolder<t_ret, t_args...>>::iterator iter,
             const std::forward_list<HookFuncHolder<t_ret, t_args...>> &hooks,
             OriginalFunc<t_ret, t_args...> orig)
            : m_iter(iter), m_hooks(hooks), m_origFunc(orig)
        {}

        ~Hook() override = default;

        t_ret callNext(t_args... args) override
        {
            if (m_iter != m_hooks.end())
            {
                const HookFuncHolder<t_ret, t_args...> &currentHook = *m_iter;
                Hook nextChain(++m_iter, m_hooks, m_origFunc);
                return currentHook.m_hookFunc(&nextChain, std::forward<t_args>(args)...);
            }

            return callOriginal(std::forward<t_args>(args)...);
        }

        t_ret callOriginal(t_args... args) const override
        {
            return m_origFunc(std::forward<t_args>(args)...);
        }

    private:
        typename std::forward_list<HookFuncHolder<t_ret, t_args...>>::iterator m_iter;
        const std::forward_list<HookFuncHolder<t_ret, t_args...>> &m_hooks;
        OriginalFunc<t_ret, t_args...> m_origFunc;
    };

    template<typename t_ret, typename... t_args>
    class HookRegistry : public IHookRegistry<t_ret, t_args...>
    {
    public:
        ~HookRegistry() override = default;

        t_ret callChain(OriginalFunc<t_ret, t_args...> origFunc, t_args... args)
        {
            if (hasHooks())
            {
                Hook<t_ret, t_args...> chain(m_hooks.begin(), m_hooks, origFunc);
                return chain.callNext(std::forward<t_args>(args)...);
            }

            return origFunc(std::forward<t_args>(args)...);
        }

        bool hasHooks() const
        {
            return !m_hooks.empty();
        }

        bool registerHook(HookFunc<t_ret, t_args...> hook, HookPriority priority) override
        {
            if (!hook)
            {
                // TODO: Error message
                return false;
            }

            if (_findHook(hook))
            {
                // TODO: Error message
                return false;
            }

            if (!hasHooks())
            {
                m_hooks.emplace_front(hook, priority);
                return true;
            }

            typename std::forward_list<HookFuncHolder<t_ret, t_args...>>::iterator prevIter = m_hooks.before_begin();
            for (auto it = m_hooks.begin(); it != m_hooks.end(); ++it)
            {
                if ((*it).m_priority < priority)
                {
                    m_hooks.emplace_after(prevIter, hook, priority);
                    return true;
                }

                prevIter = it;
            }

            m_hooks.emplace_after(prevIter, hook, priority);
            return true;
        }

        void unregisterHook(HookFunc<t_ret, t_args...> hook) override
        {
            using BaseFunc = t_ret (*)(t_args...);
            m_hooks.remove_if([hook](const HookFuncHolder<t_ret, t_args...> &funcHolder) {
                return *funcHolder.m_hookFunc.template target<BaseFunc>() == *hook.template target<BaseFunc>();
            });
        }

    private:
        bool _findHook(HookFunc<t_ret, t_args...> hookFunc) const
        {
            auto isSameFunc = [hookFunc](const HookFunc<t_ret, t_args...> &func) {
                using BaseFunc = t_ret (*)(t_args...);
                return *func.template target<BaseFunc>() == *hookFunc.template target<BaseFunc>();
            };

            for (const auto &hook : m_hooks)
            {
                if (isSameFunc(hook.m_hookFunc))
                {
                    return true;
                }
            }
            return false;
        }

    private:
        std::forward_list<HookFuncHolder<t_ret, t_args...>> m_hooks;
    };

    template<typename t_ret, typename t_entityClass, typename... t_args>
    struct ClassHookFuncHolder
    {
        ClassHookFuncHolder(ClassHookFunc<t_ret, t_entityClass, t_args...> func, HookPriority priority)
            : m_hookFunc(func), m_priority(priority)
        {}

        ClassHookFunc<t_ret, t_entityClass, t_args...> m_hookFunc;
        HookPriority m_priority;
    };

    // Implementation for class chains in modules
    template<typename t_ret, typename t_entity, typename... t_args>
    class ClassHook : public IClassHook<t_ret, t_entity, t_args...>
    {
    public:
        ClassHook(typename std::forward_list<ClassHookFuncHolder<t_ret, t_entity, t_args...>>::iterator iter,
             const std::forward_list<ClassHookFuncHolder<t_ret, t_entity, t_args...>> &hooks,
             std::function<t_ret(t_entity *, t_args...)> orig)
            : m_iter(iter), m_hooks(hooks), m_endFunc(orig)
        {}

        ~ClassHook() override = default;

        t_ret callNext(t_entity *entity, t_args... args) override
        {
            if (m_iter != m_hooks.end())
            {
                const ClassHookFuncHolder<t_ret, t_entity, t_args...> &currentHook = *m_iter;
                ClassHook nextChain(++m_iter, m_hooks, m_endFunc);
                return std::invoke(currentHook.m_hookFunc, &nextChain, entity, std::forward<t_args>(args)...);
            }

            return callOriginal(std::forward<t_args>(args)...);
        }

        t_ret callOriginal(t_entity *entity, t_args... args) const override
        {
            if constexpr (std::is_same_v<t_ret, void>)
            {
                std::invoke(m_endFunc, entity, std::forward<t_args>(args)...);
            }
            else
            {
                return std::invoke(m_endFunc, entity, std::forward<t_args>(args)...);
            }
        }

    private:
        typename std::forward_list<ClassHookFuncHolder<t_ret, t_entity, t_args...>>::iterator m_iter;
        const std::forward_list<ClassHookFuncHolder<t_ret, t_entity, t_args...>> &m_hooks;
        std::function<t_ret(t_entity *, t_args...)> m_endFunc;
    };

    template<typename t_ret, typename t_entity, typename... t_args>
    class ClassHookRegistry : public IClassHookRegistry<t_ret, t_entity, t_args...>
    {
    public:
        ClassHookRegistry(intptr_t vtable, std::uint32_t offset, std::intptr_t hook)
            : m_vtable(vtable), m_funcOffset(offset), m_hookFunc(hook)
        {}

        ~ClassHookRegistry() override = default;

        t_ret callChain(t_entity *entity, t_args... args)
        {
            ClassHook<t_ret, t_entity, t_args...> chain(m_hooks.begin(), m_hooks, m_origFunc);
            return chain.callNext(entity, std::forward<t_args>(args)...);
        }

        bool hasHooks() const
        {
            return !m_hooks.empty();
        }

        bool registerHook(ClassHookFunc<t_ret, t_entity, t_args...> hook, HookPriority priority) override
        {
            if (!hook)
            {
                // TODO: Error message
                return false;
            }

            if (_findHook(hook))
            {
                // TODO: Error message
                return false;
            }

            if (!hasHooks())
            {
                m_hooks.emplace_front(hook, priority);
                return true;
            }

            typename std::forward_list<ClassHookFuncHolder<t_ret, t_entity, t_args...>>::iterator prevIter = m_hooks.before_begin();
            for (auto it = m_hooks.begin(); it != m_hooks.end(); ++it)
            {
                if ((*it).m_priority < priority)
                {
                    m_hooks.emplace_after(prevIter, hook, priority);
                    return true;
                }

                prevIter = it;
            }

            m_hooks.emplace_after(prevIter, hook, priority);
            return true;
        }

        void unregisterHook(ClassHookFunc<t_ret, t_entity, t_args...> hook) override
        {
            using BaseFunc = t_ret (*)(t_entity *, t_args...);
            m_hooks.remove_if([hook](const ClassHookFuncHolder<t_ret, t_args...> &funcHolder) {
              return *funcHolder.m_hookFunc.template target<BaseFunc>() == *hook.template target<BaseFunc>();
            });
        }

    private:
        bool _findHook(ClassHookFunc<t_ret, t_entity, t_args...> hookFunc) const
        {
            auto isSameFunc = [hookFunc](const ClassHookFunc<t_ret, t_entity, t_args...> &func) {
              using BaseFunc = t_ret (*)(t_entity *, t_args...);
              return *func.template target<BaseFunc>() == *hookFunc.template target<BaseFunc>();
            };

            for (const auto &hook : m_hooks)
            {
                if (isSameFunc(hook.m_hookFunc))
                {
                    return true;
                }
            }
            return false;
        }

    private:
        intptr_t m_vtable;
        std::uint32_t m_funcOffset;
        intptr_t m_hookFunc;
        intptr_t m_origFunc;
        std::forward_list<ClassHookFuncHolder<t_ret, t_entity, t_args...>> m_hooks;
    };
}