#pragma once

#include <unordered_map>
#include <unordered_set>
#include <list>

#include "W4Common.h"
#include "W4Math.h"
#include "Object.h"
#include "Resource.h"

#include "Component.h"

#include "Collider.h"
#include "Nodes/DebugView.h"

namespace w4::render
{
    class Camera;
    class RootNode;
    class IRenderPass;
}

namespace w4::core {

#define W4_NODE(T, S)                                                       \
    W4_OBJECT_IMPL("Object", T, S)                                            \
    virtual w4::sptr<Node> clone() const override __W4_NODE_CLONE_BODY(T)   \

class VisibleNode;
class DataNode;

class Node
    : public Object
    , public render::IDebugView
    , public w4::enable_from_this<Node>
{
    W4_OBJECT_IMPL("Object", Node, Object)
protected:
    struct NodeCloning{};

public:
    enum class VisibilityPolice
    {
        Visible,
        Hidden
    };
    using Object::Object;

    using Callback   = std::function<void(Node&)>;
    using Predicate  = std::function<bool(w4::cref<Node>)>;
    using PredicateC = std::function<bool(Node&)>;

    using CollidersArray = std::unordered_map<std::string, sptr<Collider>>;

    explicit Node(const std::string& name = Object::defaultName);
    Node(NodeCloning, const Node&); //clone ctor
    virtual ~Node();

    bool isEnabled() const;
    void setEnabled(bool);

 // type cast and cloning

    bool canClone() const;
    void setCanClone(bool value);

    virtual w4::sptr<Node> clone() const;

    template<typename T> w4::sptr<T>       as();
    template<typename T> w4::sptr<const T> as()   const;
    template<typename T> T*                asRaw();
    template<typename T> const T*          asRaw() const;

// polymorph API

    virtual void log(uint indent = 0) const override ;

    virtual void onUpdate(float dt) override;
    virtual void onNameChange() override;
    virtual void onTransformChange();
    virtual void onRender(const render::IRenderPass&);

// hierarhy API

    w4::sptr<render::RootNode> getRoot()   const;

    w4::sptr<Node> getParent() const;
    bool hasParent() const;

    void addChild(w4::cref<Node>, bool preserveTransorm = true, VisibilityPolice = VisibilityPolice::Visible);
    void addChild(const std::string&, w4::cref<Node>, bool preserveTransorm = true, VisibilityPolice = VisibilityPolice::Visible);

    void removeChild(w4::cref<Node>);
    void removeChild(const std::string&);
    void removeChild(const std::list<w4::sptr<Node>>&);

    const std::unordered_set<w4::sptr<Node>>& getChildren() const;

    std::list<w4::sptr<Node>> getAllChildren() const;

    std::list<w4::sptr<Node>> findChildren(std::string const&) const;
    std::list<w4::sptr<Node>> findChildrenRecursive(std::string const&) const;

    template<typename T> w4::sptr<T> getChild(const std::string&);
//    template<typename T, typename ...Args> w4::sptr<T> getChild(const std::string&, Args...);

    void traversal(const Callback&);
    void traversal(const PredicateC & predicate, const Callback&);

    void foreachChildren(const Callback&);

    template<typename T>
    void traversalTyped(const std::function<void(w4::cref<T>)>&);

    virtual bool isInFrustum(const render::Camera& cam) const;

// transform API

    math::mat4::cref getWorldTransformMatrix();

    void setWorldTransform(math::Transform::cref);
    void setLocalTransform(math::Transform::cref);

    void setWorldTranslation(math::vec3::cref);
    void setLocalTranslation(math::vec3::cref);

    void setWorldRotation(math::Rotator::cref);
    void setLocalRotation(math::Rotator::cref);

    void setWorldScale(math::vec3::cref);
    void setLocalScale(math::vec3::cref);

    math::Transform::cref   getWorldTransform()     const;
    math::Transform::cref   getLocalTransform()     const;
    math::vec3::cref        getWorldTranslation()   const;
    math::vec3::cref        getLocalTranslation()   const;
    math::Rotator::cref     getWorldRotation()      const;
    math::Rotator::cref     getLocalRotation()      const;
    math::vec3::cref        getWorldScale()         const;
    math::vec3::cref        getLocalScale()         const;

    math::vec3              getWorldUp() const;
    math::vec3              getWorldForward() const;
    math::vec3              getWorldRight() const;

    math::mat4              getViewMatrix() const;

    void translateLocal(const math::vec3 &translation);
    void translateWorld(const math::vec3 &translation);

    void rotateLocal(const math::Rotator&rotator);
    void rotateWorld(const math::Rotator&rotator);

    void setWorldRotation(math::Rotator::cref, math::vec3::cref worldPt);
    void rotateAroundPoint(const math::Rotator& rotator, const math::vec3& worldPt);

    size_t subscribeToWorldTransformChanged(std::function<void(math::Transform::cref)>);
    void ubsubscribeFromWorldTransformChanged(size_t);

//components
    template<typename T>
    T& addComponent(const variant::Variant& data = {});
    template<typename T>
    T& addComponent(const IComponent::Id& id, const variant::Variant& data = {});

    template<typename T>
    void removeComponent(const IComponent::Id& id);
    template<typename T>
    void removeFirstComponent();
    template<typename T>
    void removeAllComponents();

    template<typename T>
    bool hasComponent(const IComponent::Id&) const;
    template<typename T>
    const T& getComponent(const IComponent::Id&) const;
    template<typename T>
    const T& getFirstComponent() const;
    template<typename T>
    T& getComponent(const IComponent::Id&);
    template<typename T>
    T& getFirstComponent();
    template<typename T>
    const std::unordered_set<T*>& getAllComponents() const;

    size_t registerTransformCallbacks(
          std::function<void(const w4::math::vec3&)>    posCallback,
          std::function<void(const w4::math::Rotator&)> rotCallback,
          std::function<void(const w4::math::vec3&)>    scaleCallback);
    void unregisterTransformCallbacks(size_t id);

    size_t registerNodeChangedCallback(const std::function<void (Node&)>&);
    void unregisterNodeChangedCallback(size_t id);

    // colliding
    const CollidersArray& getColliders() const;

    cref<Collider> addExistingCollider(const std::string& name, cref<Collider>);

    template<typename... Args>
    cref<Collider> addCollider(const char* name, Args&&... args);

    template<typename... Args>
    cref<Collider> addCollider(std::string name, Args&&... args);

    template<typename... Args>
    cref<Collider> addCollider(Args&&... args);

    template<typename T, typename... Args>
    cref<Collider> addCollider(const char* name, Args&&... args);

    template<typename T, typename... Args>
    cref<Collider> addCollider(std::string name, Args&&... args);

    template<typename T, typename... Args>
    cref<Collider> addCollider(Args&&... args);

    cref<Collider> getCollider(const std::string& name);

    void removeCollider(cref<Collider>);
    void removeCollider(const std::string& name);

    void setFrustumCollider(cref<Collider>);
    cref<Collider> getFrustumCollider() const;

//tool
    virtual void collectResourcesRecursive(std::unordered_set<sptr<resources::Resource>> & destination);

protected:
    void rebuildIndexes();
    void setRootNode(render::RootNode* root);
    void callNodeChanged();

private:
    void onParentTransformChanged();
    void onParentScaleChanged();
    void onParentRotationChanged();
    void onParentTranslationChanged();

    inline void callTransformCb();
    inline void callTransformTranslationCb();
    inline void callTransformRotationCb();
    inline void callTransformScaleCb();

    // colliding
    void updateCollidersTransform();
    static std::string generateColliderName();

    //IDebugView
    friend class core::IComponent;
    render::RootNode * debugViewGetRootNode() override;

private:
    static size_t m_transCallbackId;
    std::unordered_map<size_t, std::tuple<std::function<void(const w4::math::vec3&)>,
                                          std::function<void(const w4::math::Rotator&)>,
                                          std::function<void(const w4::math::vec3&)>>> m_transformCallbacks;

    static size_t m_nodeChangedCallbackId;
    std::unordered_map<size_t, std::function<void (Node&)>> m_nodeChangedCallbacks;

    Node* m_parent = nullptr;
    render::RootNode* m_root = nullptr;
    bool  m_isEnabled = true;
    bool m_canClone = true;

    std::unordered_set<w4::sptr<Node>>                   m_children;
    std::unordered_set<w4::sptr<Node>>                   m_hiddenChildren;
    std::unordered_multimap<std::string, w4::sptr<Node>> m_nameIndex;

    math::Transform m_localTransform;
    math::Transform m_worldTransform;
    math::mat4      m_transformMatrix;
    math::mat3      m_normalMatrix;
    bool            m_isTransformMatrixDirty = true;

    std::unordered_map<size_t, std::function<void(math::Transform::cref)>> m_transformChangeSubscribers;
    size_t m_nextTransformChangesSubscriberHandle = 0;

    //components
    std::unordered_map<TypeInfo::HashType, std::unordered_map<IComponent::Id, uptr<IComponent>>> m_components;
    std::unordered_map<TypeInfo::HashType, std::unordered_set<IComponent*>> m_componentsHierarchy;

    // colliding
    CollidersArray m_colliders;
    sptr<Collider> m_frustumCollider;
};


} //namespace w4::core

#include "impl/Node.inl"
