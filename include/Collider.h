#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "W4Common.h"
#include "W4Math.h"
#include "BoundingVolume.h"

namespace w4::core {

class Node;

enum class ScreencastEvent
{
    Down,
    Move,
    Up
};

class Collider
{
public:
    // callbacks
    using IntersectCallback = std::function<void(const CollisionInfo&)>;
    using ScreencastCallback = std::function<void(const CollisionInfo&)>;
    using RaycastCallback = std::function<void(const CollisionInfo&)>;
    struct CtInfo
    {
        bool previous = false;
        bool current = false;
        CollisionInfo collisionInfo;
    };
    using CollisionTable = std::unordered_map<Collider*, CtInfo>;

    Collider(Node*, cref<BoundingVolume>);
    Collider(const Collider&);
    ~Collider();

    template<typename T, typename...Args>
    static sptr<Collider> create(Node*, Args&&... args);

    // volume contains cached transform
    void setVolume(cref<BoundingVolume>, bool preserveTransform = false);
    cref<BoundingVolume> getVolume() const;

    // intersect
    void setIntersecting(bool flag = true);
    bool isIntersecting() const;

    void setIntersectionBeginCallback(const IntersectCallback&);
    void resetIntersectionBeginCallback();

    void setIntersectionCallback(const IntersectCallback&);
    void resetIntersectionCallback();

    void setIntersectionEndCallback(const IntersectCallback&);
    void resetIntersectionEndCallback();

    // screencast
    void setBlockScreencasts(bool flag = true);
    bool isBlockScreencasts() const;

    void setScreencastCallback(ScreencastEvent, const ScreencastCallback&);
    void resetScreencastCallback(ScreencastEvent);

    void setScreencastCallback(const ScreencastCallback&);
    void resetScreencastCallback();

    // raycast

    void setReceiveRaycasts(bool flag = true);
    bool isReceiveRaycasts() const;

    void setBlockRaycasts(bool flag = true);
    bool isBlockRaycasts() const;

    void setRaycastCallback(const RaycastCallback&);
    void resetRaycastCallback();

    void setLocalTransform(math::Transform::cref);
    math::Transform::cref getLocalTransform() const;

    void setLocalTranslation(math::vec3::cref);
    math::vec3::cref getLocalTranslation() const;

    void setLocalRotation(math::Rotator::cref);
    math::Rotator::cref getLocalRotation() const;

    void setLocalScale(math::vec3::cref);
    math::vec3::cref getLocalScale() const;

    math::Transform::cref getWorldTransform() const;
    math::vec3::cref getWorldTranslation() const;
    math::Rotator::cref getWorldRotation() const;
    math::vec3::cref getWorldScale() const;

    void onNodeTransformChanged(const math::Transform& transform);

    // internal use only? add ColliderEventDispatcher as friend?
    bool intersect(const math::Ray&, CollisionInfo&);
    bool intersect(const Collider&, CollisionInfo&);

    void onIntersectionBegin(const CollisionInfo&);
    void onIntersection(const CollisionInfo&);
    void onIntersectionEnd(const CollisionInfo&);
    void onScreencast(ScreencastEvent, const CollisionInfo&);
    void onRaycast(const CollisionInfo&);

    CollisionTable& getCollisionTable();

    Node* getParent() const;

    bool isEnabled() const;

private:
    Collider(Node* parent);

    core::Node* m_parent;
    sptr<BoundingVolume> m_boundingVolume;

    bool m_isIntersecting = false;
    IntersectCallback m_intersectionBeginCallback;
    IntersectCallback m_intersectionCallback;
    IntersectCallback m_intersectionEndCallback;

    bool m_isBlockScreencasts = true;
    std::unordered_map<ScreencastEvent, ScreencastCallback> m_screencastCallback;

    bool m_isBlockRaycasts = false;
    bool m_isReceiveRaycasts = false;
    RaycastCallback m_raycastCallback;

    CollisionTable m_collisionTable;
};

class ColliderLayer
{
public:
    void add(const sptr<Collider>&);
    void remove(const sptr<Collider>&);
    sptr<AABB> getAABB();
    sptr<Sphere> getSphere();
    void intersect(const Collider&, const Collider::IntersectCallback&);
private:
    std::unordered_set<sptr<Collider>> m_colliders;
};

#include "impl/Collider.inl"

} // namespace w4::core