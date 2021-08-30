#define __W4_COMPONENT(T, S)                                                                                                                            \
public:                                                                                                                                                 \
    W4_TYPEINFO(T, "Component", S)                                                                                                                      \
    using Self = T;                                                                                                                                     \
    T(const Id& id, ::w4::core::Node &owner): T(NoRegisterTag{}, id, owner) { ::w4::core::ComponentsSystem::onComponentAdded(RegisterTag{}, this); }    \
protected:                                                                                                                                              \
    T(NoRegisterTag, const Id& id, ::w4::core::Node &owner): Super(NoRegisterTag{}, id, owner) {}                                                       \
    virtual uptr<IComponent> clone(::w4::core::Node & owner) const override                                                                             \
    {                                                                                                                                                   \
        if constexpr (::w4::core::details::IsComponentCloneable<T>::value)                                                                              \
        {                                                                                                                                               \
            return make::uptr<T>(CloneTag{}, *this, owner);                                                                                             \
        }                                                                                                                                               \
        else                                                                                                                                            \
        {                                                                                                                                               \
            return nullptr;                                                                                                                             \
        }                                                                                                                                               \
    }                                                                                                                                                   \

#define __W4_COMPONENT_DISABLE_CLONING public: struct __COMPONENT_NOT_CLONEABLE{};

namespace w4::core::details {

template< typename ... Ts >
using void_t = void;

template< typename T, typename = void >
struct IsComponentCloneable : std::true_type {};

template< typename T >
struct IsComponentCloneable< T, void_t<typename T::__COMPONENT_NOT_CLONEABLE> > : std::false_type {};

}

template<typename T>
T& ::w4::core::IComponent::as()
{
    return static_cast<T&>(*this);
}

template<typename T>
const T& ::w4::core::IComponent::as() const
{
    return static_cast<const T&>(*this);
}

template<typename T>
const std::unordered_set<T*>& ::w4::core::ComponentsSystem::getComponents()
{
    static_assert(is_base_of_v<::w4::core::IComponent, T> || is_same_v<::w4::core::IComponent, T>);

    return static_cast<const std::unordered_set<T*>&>(m_enabledComponentsHierarchy[T::typeInfo.hash()]);
}


template<typename T>
void ::w4::core::ComponentsSystem::onComponentAdded(IComponent::RegisterTag, T * component)
{
    static_assert(is_base_of_v < ::w4::core::IComponent, T > || is_same_v < ::w4::core::IComponent, T > );

    onComponentEnabled(component, component->isEnabled());

    if ((void (T::*)(float)) &T::update != &IComponent::update) //if update fn is overloaded
    {
        if (component->isEnabled())
        {
            m_enabledUpdateableComponents.emplace(component);
        }
        else
        {
            m_disabledUpdateableComponents.emplace(component);
        }
    }
}

namespace std {

template<>
struct hash<::w4::core::IComponent::Id>
{
    size_t operator()(const w4::core::IComponent::Id& id) const
    {
        return id.value;
    }
};

} //namespace std
