#pragma once

#define __W4_NODE_CLONE_BODY(TYPE)                                              \
{                                                                               \
    if (!canClone())                                                            \
    {                                                                           \
        W4_LOG_ERROR("node [" #TYPE "] %s can not be cloned", getName().data());\
        return nullptr;                                                         \
    }                                                                           \
    return make::sptr<TYPE>(NodeCloning{}, *this);                              \
}

namespace w4::core {

/*
 * components
 * */

template<typename T>
T& Node::addComponent(const variant::Variant& data)
{
    static uint64_t counter;
    IComponent::Id id("");
    auto & typeContainer = m_components[T::typeInfo.hash()];
    do
    {
        //anti-chamomile-man check. Performance at this point is not important
        if (counter == std::numeric_limits<uint64_t>::max())
        {
            counter = 0;
        }
        id = IComponent::Id(utils::format("GENERATED_ID NODE '%s' NODETYPE '%s' COMPONENT '%s' NUMBER '%d'"
                                          , getName().data()
                                          , getTypeInfo().name()
                                          , T::typeInfo.name()
                                          , counter++).data());
    } while(typeContainer.count(id));

    return addComponent<T>(id, data);
}

template<typename T>
T& Node::addComponent(const IComponent::Id& id, const variant::Variant& data)
{
    auto & typeContainer = m_components[T::typeInfo.hash()];
    auto it = typeContainer.find(id);
    if (it != typeContainer.end())
    {
        FATAL_ERROR("Node '%s' of type '%s': unable to add component of type '%s' with id '%u' because component with this id is already exists"
                    , getName().data()
                    , getTypeInfo().name()
                    , T::typeInfo.name()
                    , id.value);
    }
    auto &result = typeContainer.emplace(id, make::uptr<T>(id, *this)).first->second->template as<T>();
    auto ti = &T::typeInfo;
    do
    {
        m_componentsHierarchy[ti->hash()].emplace(&result);
        ti = ti->parentInfo();
    } while(ti);
    result.initialize(data);
    return result;
}

template<typename T>
void Node::removeComponent(const IComponent::Id& id)
{
    auto & typeContainer = m_components[T::typeInfo.hash()];
    auto it = typeContainer.find(id);
    if (it == typeContainer.end())
    {
        W4_LOG_WARNING("Node '%s' of type '%s': call remove for component of type '%s' with id '%u', but this component is not exists"
        , getName().data()
        , getTypeInfo().name()
        , T::typeInfo.name()
        , id.value);
    }
    it->second->finalize();

    auto ti = &T::typeInfo;
    do
    {
        m_componentsHierarchy[ti->hash()].erase(it->second.get());
        ti = ti->parentInfo();
    } while(ti);
    typeContainer.erase(it);
}

template<typename T>
void Node::removeFirstComponent()
{
    auto & typeContainer = m_components[T::typeInfo.hash()];
    if (!typeContainer.empty())
    {
        removeComponent<T>(typeContainer.begin()->second->id());
    }
    else
    {
        W4_LOG_WARNING("Node '%s' of type '%s': call remove first component of type '%s', but no one component of this type are exists"
        , getName().data()
        , getTypeInfo().name()
        , T::typeInfo.name());
    }
}

template<typename T>
void Node::removeAllComponents()
{
    auto & typeContainer = m_components[T::typeInfo.hash()];
    std::vector<IComponent::Id> ids;
    ids.reserve(typeContainer.size());
    for(auto & [id, component]: typeContainer)
    {
        ids.emplace_back(id);
    }
    for (auto & id: ids)
    {
        removeComponent<T>(id);
    }
}

template<typename T>
bool Node::hasComponent(const IComponent::Id& id) const
{
    auto containerIt = m_components.find(T::typeInfo.hash());
    if (containerIt != m_components.end())
    {
        return containerIt->second.count(id) > 0;
    }
    return false;
}

template<typename T>
const T& Node::getComponent(const IComponent::Id& id) const
{
    auto containerIt = m_components.find(T::typeInfo.hash());
    if (containerIt != m_components.end())
    {
        auto componentIt = containerIt->second.find(id);
        if (componentIt != containerIt->second.end())
        {
            return componentIt->second->template as<T>();
        }
    }
    FATAL_ERROR("Node '%s' of type '%s': unable to get component of type '%s' with id '%u', because this component is not exists"
        , getName().data()
        , getTypeInfo().name()
        , T::typeInfo.name()
        , id.value);
}

template<typename T>
const T& Node::getFirstComponent() const
{
    auto containerIt = m_components.find(T::typeInfo.hash());
    if (containerIt == m_components.end() || containerIt->second.empty())
    {
        FATAL_ERROR("Node '%s' of type '%s': unable to get first component of type '%s', because no one component of this type exists"
        , getName().data()
        , getTypeInfo().name()
        , T::typeInfo.name());
    }
    return containerIt->second.begin()->second->template as<T>();
}

template<typename T>
T& Node::getComponent(const IComponent::Id& id)
{
    auto containerIt = m_components.find(T::typeInfo.hash());
    if (containerIt != m_components.end())
    {
        auto componentIt = m_components[T::typeInfo.hash()].find(id);
        if (componentIt != containerIt->second.end())
        {
            return componentIt->second->template as<T>();
        }
    }
    FATAL_ERROR("Node '%s' of type '%s': unable to get component of type '%s' with id '%u', because this component is not exists"
    , getName().data()
    , getTypeInfo().name()
    , T::typeInfo.name()
    , id.value);
}

template<typename T>
T& Node::getFirstComponent()
{
    auto containerIt = m_components.find(T::typeInfo.hash());
    if (containerIt == m_components.end() || containerIt->second.empty())
    {
        FATAL_ERROR("Node '%s' of type '%s': unable to get first component of type '%s', because no one component of this type exists"
        , getName().data()
        , getTypeInfo().name()
        , T::typeInfo.name());
    }
    return containerIt->second.begin()->second->template as<T>();
}

template<typename T>
const std::unordered_set<T*>& Node::getAllComponents() const
{
    auto it = m_componentsHierarchy.find(T::typeInfo.hash());
    if (it == m_componentsHierarchy.end())
    {
        static std::unordered_set<T*> empty;
        return empty;
    }
    return reinterpret_cast<const std::unordered_set<T*>&>(it->second);
}
template<typename T> w4::sptr<T> Node::getChild(const std::string& name)
{
    auto res = findChildren(name);
    if(res.empty())
        FATAL_ERROR("can't find child named '%s' in node '%s'", name.c_str(), getName().c_str());
    if(res.size() > 1)
        W4_LOG_ERROR("collision: more than one child '%s' in node '%s'", name.c_str(), getName().c_str());
    if (!res.back()->is<T>() && !res.back()->derived_from<T>())
    {
        FATAL_ERROR("node '%s': request child '%s' as [%s], but he is [%s]", getName().data(), name.data(), T::typeInfo.name(), res.back()->getTypeInfo().name());
    }
    return res.back()->as<T>();
}

template<typename T>
void Node::traversalTyped(const std::function<void(w4::cref<T>)>& callback)
{
    if (is<T>() || derived_from<T>())
    {
        callback(as<T>());
    }
    for(auto & node: m_children)
    {
        node->traversalTyped<T>(callback);
    }
}

template<typename T> w4::sptr<T> Node::as()
{
    if (!is<T>() && !derived_from<T>())
    {
        FATAL_ERROR("node '%s': request as [%s], but this is [%s]", getName().data(), T::typeInfo.name(), getTypeInfo().name());
    }
    return std::static_pointer_cast<T>(shared_from_this());
}

template<typename T> w4::sptr<const T> Node::as() const
{
    if (!is<T>() && !derived_from<T>())
    {
        FATAL_ERROR("node '%s': request as [%s], but this is [%s]", getName().data(), T::typeInfo.name(), getTypeInfo().name());
    }
    return std::static_pointer_cast<const T>(shared_from_this());
}

template<typename T> T* Node::asRaw()                   {return static_cast<T*>(this);}
template<typename T> const T* Node::asRaw() const       {return static_cast<const T*>(this);}

// collision

template<typename... Args>
cref<Collider> Node::addCollider(const char* name, Args&&... args)
{
    return addCollider(std::string(name), std::forward<Args>(args)...);
}

template<typename... Args>
cref<Collider> Node::addCollider(std::string name, Args&&... args)
{
    return addExistingCollider(name, make::sptr<Collider>(this, std::forward<Args>(args)...));
}

template<typename... Args>
cref<Collider> Node::addCollider(Args&&... args)
{
    return addExistingCollider(Node::generateColliderName(), make::sptr<Collider>(this, std::forward<Args>(args)...));
}

template<typename T, typename... Args>
cref<Collider> Node::addCollider(const char* name, Args&&... args)
{
    return addCollider<T>(std::string(name), std::forward<Args>(args)...);
}

template<typename T, typename... Args>
cref<Collider> Node::addCollider(std::string name, Args&&... args)
{
    return addExistingCollider(name, Collider::create<T>(this, std::forward<Args>(args)...));
}

template<typename T, typename... Args>
cref<Collider> Node::addCollider(Args&&... args)
{
    return addExistingCollider(Node::generateColliderName(), Collider::create<T>(this, std::forward<Args>(args)...));
}

}
