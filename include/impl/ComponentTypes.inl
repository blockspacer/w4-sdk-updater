#include "FatalError.h"
#include "W4Logger.h"

template<typename T>
void ::w4::core::ComponentTypes::registerConstructor()
{
    static_assert(core::is_base_of_v<IComponent, T>);
    static_assert(T::__isComponentBase == false);

    if constexpr (core::is_base_of_v<IMultiComponent, T>)
    {
        registerConstructor(T::typeInfo, [](IMultiComponentId id, core::Node& owner) -> uptr<IMultiComponent>{ return make::uptr<T>(id, owner); });
    }
    else
    {
        registerConstructor(T::typeInfo, [](core::Node& owner) -> uptr <IComponent>{ return make::uptr<T>(owner); });
    }
}

template<typename T, typename U>
void ::w4::core::ComponentTypes::registerIncompatible()
{
    static_assert(core::is_base_of_v<IComponent, T>);
    static_assert(core::is_base_of_v<IComponent, U>);
    static_assert(!core::is_same_v<T, U>);
    static_assert(!core::is_same_v<U, IComponent>);
    static_assert(!core::is_same_v<U, IMultiComponent>);

    m_incompatibleComponents[T::typeInfo.hash()].emplace(U::typeInfo.hash());
    m_incompatibleComponents[U::typeInfo.hash()].emplace(T::typeInfo.hash());
}

namespace std {

template<>
struct hash<w4::core::IMultiComponentId>
{
    size_t operator()(const w4::core::IMultiComponentId& id) const
    {
        return hash<w4::core::IMultiComponentId::ValueType>()(id.value);
    }
};
}