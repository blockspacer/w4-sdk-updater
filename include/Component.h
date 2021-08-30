#pragma once

#include "Variant.h"
#include "TypeInfo.h"

#include "Nodes/DebugView.h"

#include <functional>
#include <unordered_set>
#include <unordered_map>

#define W4_COMPONENT(T, S) __W4_COMPONENT(T, S)
#define W4_COMPONENT_DISABLE_CLONING __W4_COMPONENT_DISABLE_CLONING

namespace w4::core {

class Node;
class IComponent: public render::IDebugView
{
    friend class ComponentsSystem;
    IComponent(const IComponent&) = delete;
    IComponent(IComponent&&) = delete;
    IComponent& operator=(const IComponent&) = delete;
    IComponent& operator=(IComponent&&) = delete;
public:
    struct Id
    {
        using ValueType = uint32_t;

        Id(const char* s): value(utils::hash(s)) {}
        bool operator==(const Id& rh) const { return value == rh.value; }

        ValueType value;
    };

protected:
    struct CloneTag{};
    struct NoRegisterTag{};
    struct RegisterTag{};

public:
    explicit IComponent(NoRegisterTag, Id, Node &owner);
    IComponent(CloneTag, const IComponent&, Node&);
    virtual ~IComponent() noexcept;

    const Id& id() const;

    void enable(bool isEnabled);
    bool isEnabled() const;

    template<typename T>
    T& as();

    template<typename T>
    const T& as() const;

    ::w4::core::Node & getOwner();
    const ::w4::core::Node & getOwner() const;

    virtual void initialize(const variant::Variant& = {});
    virtual void finalize();
    virtual void update(float dt);

    static constexpr core::TypeInfo typeInfo{"IComponent"};
    virtual const core::TypeInfo& getTypeInfo() const = 0;

protected:
    ::w4::core::Node & m_owner;

    virtual void onEnabled(bool value);
    virtual uptr<IComponent> clone(Node & owner) const = 0;

private:
    friend class ::w4::core::Node;
    void onOwnerEnabled(bool);

    //IDebugView
    render::RootNode * debugViewGetRootNode() override;

private:
    Id const m_id;
    bool m_isEnabled = true;
};

class ComponentsSystem
{
public:
    using ComponentsCointainer = std::unordered_set<IComponent*>;

public:
    static void update(float dt);

    template<typename T = IComponent>
    static const std::unordered_set<T*>& getComponents();

    template<typename T>
    static void onComponentAdded(IComponent::RegisterTag, T * component);

private:
    friend class IComponent;
    static void onComponentRemoved(IComponent * component);

    static void onComponentEnabled(IComponent*, bool v);
    static bool checkComponentExists(IComponent*);

private:
    //all components enabled disabled hierarchy
    static ComponentsCointainer m_enabledComponents;
    static std::unordered_map<TypeInfo::HashType, ComponentsCointainer> m_enabledComponentsHierarchy;
    static ComponentsCointainer m_disabledComponents;

    //updateable components enabled disabled
    static ComponentsCointainer m_enabledUpdateableComponents;
    static ComponentsCointainer m_disabledUpdateableComponents;
};

} //namespace w4::core

#include "impl/Component.inl"
