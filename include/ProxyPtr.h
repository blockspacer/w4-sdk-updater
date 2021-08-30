#pragma once

#include "W4Common.h"
#include "impl/ProxyPtrImpl.inl"

namespace w4::core {

template<typename T>
class ProxyPtr
{
    friend struct std::hash<ProxyPtr<T>>;
    friend class Cache<T>;

    ProxyPtr(w4::sptr<details::ProxyPtrImpl<T>> impl);

public:
    ProxyPtr() = default;
    ProxyPtr(std::nullptr_t);
    ProxyPtr(ProxyPtr const&) = default;
    ProxyPtr(ProxyPtr &&) = default;
    ProxyPtr & operator=(ProxyPtr const&) = default;
    ProxyPtr & operator=(ProxyPtr &&) = default;

    void reset();

    T* get();
    T const* get() const;

    T& operator*();
    T const& operator*() const;

    T* operator->();
    T const* operator->() const;

    operator bool() const;
    explicit operator w4::sptr<T>() const;

    bool operator==(ProxyPtr<T> const& rh) const;
    bool operator!=(ProxyPtr<T> const& rh) const;
    bool operator<(ProxyPtr<T> const& rh) const;
    bool operator>(ProxyPtr<T> const& rh) const;

private:
    w4::sptr<details::ProxyPtrImpl<T>> m_impl;
};

#include "impl/ProxyPtr.inl"

}