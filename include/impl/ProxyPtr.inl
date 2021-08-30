template<class T>
ProxyPtr<T>::ProxyPtr(w4::sptr<details::ProxyPtrImpl<T>> impl)
    : m_impl(std::move(impl))
{

}

template<class T>
ProxyPtr<T>::ProxyPtr(std::nullptr_t)
{

}

template<class T>
void ProxyPtr<T>::reset()
{
    m_impl.reset();
}

template<class T>
T* ProxyPtr<T>::get()
{
    return m_impl.get();
}

template<class T>
T const* ProxyPtr<T>::get() const
{
    return m_impl.get();
}

template<class T>
T& ProxyPtr<T>::operator*()
{
    return *(m_impl->getObject());
}

template<class T>
T const& ProxyPtr<T>::operator*() const
{
    return *(m_impl->getObject());
}

template<class T>
T* ProxyPtr<T>::operator->()
{
    return m_impl->getObject().get();
}

template<class T>
T const* ProxyPtr<T>::operator->() const
{
    return m_impl->getObject().get();
}

template<class T>
ProxyPtr<T>::operator bool() const
{
    return m_impl != nullptr;
}

template<class T>
ProxyPtr<T>::operator w4::sptr<T>() const
{
    return m_impl->getObject();
}

template<class T>
bool ProxyPtr<T>::operator==(ProxyPtr<T> const& rh) const
{
    return m_impl == rh.m_impl;
}

template<class T>
bool ProxyPtr<T>::operator!=(ProxyPtr<T> const& rh) const
{
    return !operator==(rh);
}

template<class T>
bool ProxyPtr<T>::operator<(ProxyPtr<T> const& rh) const
{
    return m_impl < rh.m_impl;
}

template<class T>
bool ProxyPtr<T>::operator>(ProxyPtr<T> const& rh) const
{
    return rh.operator<(this);
}
