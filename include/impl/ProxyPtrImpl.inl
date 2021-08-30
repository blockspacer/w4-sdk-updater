#pragma once

#include "W4Common.h"
#include "Object.h"

namespace w4::core::details {

enum class ProxyPtrStatus
{
    waiting,
    ready
};

template<class T>
class ProxyPtrImpl
{
public:
    ProxyPtrImpl(w4::sptr<T> object, ProxyPtrStatus status);
    ProxyPtrStatus status() const;
    w4::sptr<T> const& getObject() const;
    void setRegularObject(w4::sptr<T> object);

private:
    w4::sptr<T>    m_object;
    ProxyPtrStatus m_status;
};

template<class T>
ProxyPtrImpl<T>::ProxyPtrImpl(w4::sptr<T> object, ProxyPtrStatus status)
    : m_object(object)
    , m_status(status)
{

}

template<class T>
ProxyPtrStatus ProxyPtrImpl<T>::status() const
{
    return m_status;
}

template<class T>
w4::sptr<T> const& ProxyPtrImpl<T>::getObject() const
{
    return m_object;
}

template<class T>
void ProxyPtrImpl<T>::setRegularObject(w4::sptr<T> object)
{
    W4_ASSERT( m_status == ProxyPtrStatus::waiting);
    m_object.swap(object);
    m_status = ProxyPtrStatus::ready;
}

}
