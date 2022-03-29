/*
*  Copyright (C) 2021 Anubis Development Team
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

#include <type_traits>
#include <memory>

namespace nstd
{
    template<typename W>
    class observer_ptr
    {
        using pointer = std::add_pointer_t<W>;            // exposition-only
        using reference = std::add_lvalue_reference_t<W>; // exposition-only

    public:
        using element_type = W;

        constexpr observer_ptr() noexcept : m_ptr(nullptr) {}

        constexpr observer_ptr(std::nullptr_t) noexcept : m_ptr(nullptr) {} // NOLINT(google-explicit-constructor)
        constexpr observer_ptr(pointer other) noexcept : m_ptr(other) {}    // NOLINT(google-explicit-constructor)

        template<typename W2>
        constexpr observer_ptr(const observer_ptr<W2> &other) noexcept // NOLINT(google-explicit-constructor)
            : m_ptr(other.get())
        {
        }

        template<typename W2>
        constexpr observer_ptr(const std::unique_ptr<W2> &ptr) noexcept // NOLINT(google-explicit-constructor)
            : m_ptr(ptr.get())
        {
        }

        template<typename W2>
        constexpr observer_ptr(const std::shared_ptr<W2> &ptr) noexcept // NOLINT(google-explicit-constructor)
            : m_ptr(ptr.get())
        {
        }

        constexpr pointer get() const noexcept
        {
            return m_ptr;
        }

        constexpr reference operator*() const
        {
            return *m_ptr;
        }

        constexpr pointer operator->() const noexcept
        {
            return m_ptr;
        }

        constexpr explicit operator bool() const noexcept
        {
            return m_ptr != nullptr;
        }

        constexpr bool operator==(const observer_ptr<W> &other) const noexcept
        {
            return m_ptr == other.m_ptr;
        }

        template<typename T>
        constexpr bool operator==(nstd::observer_ptr<T> other) const noexcept
        {
            return get() == other.get();
        }

        template<typename T>
        constexpr bool operator!=(nstd::observer_ptr<T> other) const noexcept
        {
            return get() != other.get();
        }

        constexpr bool operator==(std::nullptr_t) const noexcept
        {
            return !m_ptr;
        }

        template<typename T>
        constexpr bool operator==(const std::unique_ptr<T> &other) noexcept
        {
            return get() == other.get();
        }

        template<typename T>
        constexpr bool operator==(const std::shared_ptr<T> &other) noexcept
        {
            return get() == other.get();
        }

        constexpr bool operator!=(std::nullptr_t) const noexcept
        {
            return get() != nullptr;
        }

        template<typename T>
        constexpr bool operator!=(const std::unique_ptr<T> &other) const noexcept
        {
            return get() != other.get();
        }

        template<typename T>
        constexpr bool operator!=(const std::shared_ptr<T> &other) const noexcept
        {
            return get() != other.get();
        }

        constexpr explicit operator pointer() const noexcept
        {
            return m_ptr;
        }

        constexpr pointer release() noexcept
        {
            pointer p {m_ptr};
            reset();
            return p;
        }

        constexpr void reset(pointer ptr = nullptr) noexcept
        {
            m_ptr = ptr;
        }

        constexpr void swap(observer_ptr &other) noexcept
        {
            std::swap(m_ptr, other.m_ptr);
        }

    private:
        pointer m_ptr;
    }; // observer_ptr<>

    template<typename W>
    observer_ptr<W> make_observer(W *p) noexcept
    {
        return observer_ptr<W>(p);
    }

    template<typename W>
    observer_ptr<W> make_observer(const std::unique_ptr<W> &p) noexcept
    {
        return observer_ptr<W>(p);
    }

    template<typename W>
    observer_ptr<W> make_observer(const std::shared_ptr<W> &p) noexcept
    {
        return observer_ptr<W>(p);
    }

    template<typename W, typename T, typename = std::enable_if_t<std::is_base_of_v<W, T>>>
    observer_ptr<W> make_observer(const std::unique_ptr<T> &p) noexcept
    {
        return observer_ptr<W>(p);
    }

    template<typename W>
    observer_ptr<W> make_observer() noexcept
    {
        return observer_ptr<W>(nullptr);
    }

    template<typename W, typename T>
    observer_ptr<W> static_observer_cast(const observer_ptr<T> &p)
    {
        return make_observer(static_cast<W *>(p.get()));
    }

    template<typename W, typename T>
    observer_ptr<W> static_observer_cast(const std::unique_ptr<T> &p)
    {
        return make_observer(static_cast<W *>(p.get()));
    }

    template<typename W, typename T>
    observer_ptr<W> static_observer_cast(const std::shared_ptr<T> &p)
    {
        return make_observer(static_cast<W *>(p.get()));
    }

    template<typename W, typename T>
    observer_ptr<W> dynamic_observer_cast(const observer_ptr<T> &p)
    {
        return make_observer(dynamic_cast<W *>(p.get()));
    }

    template<typename W, typename T>
    observer_ptr<W> dynamic_observer_cast(const std::unique_ptr<T> &p)
    {
        return make_observer(dynamic_cast<W *>(p.get()));
    }

    template<typename W, typename T>
    observer_ptr<W> dynamic_observer_cast(const std::shared_ptr<T> &p)
    {
        return make_observer(dynamic_cast<W *>(p.get()));
    }
} // namespace nstd

namespace std
{
    template<typename T>
    struct hash<::nstd::observer_ptr<T>>
    {
        size_t operator()(::nstd::observer_ptr<T> p) const noexcept
        {
            return hash<T *>()(p.get());
        }
    };
} // namespace std

template<typename W>
bool operator==(std::nullptr_t, nstd::observer_ptr<W> p) noexcept
{
    return !p;
}

template<typename T, typename W>
bool operator==(const std::unique_ptr<T> &p1, nstd::observer_ptr<W> p2) noexcept
{
    return p1.get() == p2.get();
}

template<typename T, typename W>
bool operator==(const std::shared_ptr<T> &p1, nstd::observer_ptr<W> p2) noexcept
{
    return p1.get() == p2.get();
}

template<typename W>
bool operator!=(std::nullptr_t, nstd::observer_ptr<W> p) noexcept
{
    return static_cast<bool>(p);
}