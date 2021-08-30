template<typename T>
void ::w4::core::ComponentObjects::foreachComponent(std::function<void(T&)> callback) const
{
    static_assert(is_base_of_v<::w4::core::IComponent, T> || is_same_v<::w4::core::IComponent, T>);

    auto it = m_enabledComponentsByType.find(T::typeInfo.hash());
    if(it != m_enabledComponentsByType.end())
    {
        for(::w4::core::IComponent * component: it->second)
        {
            callback(component->as<T>());
        }
    }
}