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

#include <functional>
#include <cinttypes>
#include <memory>
#include <observer_ptr.hpp>

namespace Anubis
{
    // Specifies priorities for hooks call order in the chain.
    // For equal priorities first registered hook will be called first.
    enum class HookPriority : std::uint8_t
    {
        Uninterruptable = 255, // Hook will be called before other hooks.
        High = 192,            // Hook will be called before hooks with default priority.
        Default = 128,         // Default hook call priority.
        Medium = 64,           // Hook will be called after hooks with default priority.
        Low = 0,               // Hook will be called after all other hooks.
    };

    class IHookInfo
    {
    public:
        enum class State : std::uint8_t
        {
            Disabled = 0,
            Enabled
        };

    public:
        virtual ~IHookInfo() = default;
        virtual void setState(State state) = 0;
        [[nodiscard]] virtual HookPriority getPriority() const = 0;
    };

    template<typename t_ret, typename... t_args>
    class IHook
    {
    public:
        virtual ~IHook() = default;

        virtual t_ret callNext(t_args... args) = 0;
        virtual t_ret callOriginal(t_args... args) const = 0;
    };

    template<typename t_ret, typename... t_args>
    using HookFunc = std::function<t_ret(const std::unique_ptr<IHook<t_ret, t_args...>> &, t_args...)>;

    template<typename t_ret, typename... t_args>
    using OriginalFunc = std::function<t_ret(t_args...)>;

    // Hook chain registry(for hooks [un]registration)
    template<typename t_ret, typename... t_args>
    class IHookRegistry
    {
    public:
        virtual ~IHookRegistry() = default;

        virtual nstd::observer_ptr<IHookInfo> registerHook(HookFunc<t_ret, t_args...> hook, HookPriority priority) = 0;
        virtual void unregisterHook(nstd::observer_ptr<IHookInfo> hookInfo) = 0;

        virtual t_ret callChain(OriginalFunc<t_ret, t_args...> origFunc, t_args... args) = 0;
        virtual t_ret callChain(OriginalFunc<t_ret, t_args...> lastFunc,
                                OriginalFunc<t_ret, t_args...> origFunc,
                                t_args... args) = 0;
    };

    template<typename t_ret, typename t_entity, typename... t_args>
    class IClassHook
    {
    public:
        virtual ~IClassHook() = default;

        virtual t_ret callNext(t_entity entity, t_args... args) = 0;
        virtual t_ret callOriginal(t_entity entity, t_args... args) const = 0;
    };

    template<typename t_ret, typename t_entity, typename... t_args>
    using ClassHookFunc =
        std::function<t_ret(const std::unique_ptr<IClassHook<t_ret, t_entity, t_args...>> &, t_entity, t_args...)>;

    template<typename t_ret, typename t_entity, typename... t_args>
    using ClassOriginalFunc = std::function<t_ret(t_entity, t_args...)>;

    template<typename t_ret, typename t_entity, typename... t_args>
    class IClassHookRegistry
    {
    public:
        virtual ~IClassHookRegistry() = default;

        virtual nstd::observer_ptr<IHookInfo> registerHook(ClassHookFunc<t_ret, t_entity, t_args...> hook,
                                                           HookPriority priority) = 0;
        virtual void unregisterHook(nstd::observer_ptr<IHookInfo> hookInfo) = 0;

        virtual t_ret callChain(std::function<t_ret(t_entity, t_args...)> lastFn, t_entity entity, t_args... args) = 0;
        virtual t_ret callChain(ClassOriginalFunc<t_ret, t_entity, t_args...> lastFunc,
                                ClassOriginalFunc<t_ret, t_entity, t_args...> origFunc,
                                t_entity entity,
                                t_args... args) = 0;

        [[nodiscard]] virtual std::intptr_t getVFuncAddr() const = 0;
    };
} // namespace Anubis
