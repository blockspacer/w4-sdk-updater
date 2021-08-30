#pragma once

#include <array>
#include <limits>

#include "Object.h"
#include "W4Common.h"
#include "W4Math.h"

namespace w4::core {

class Node;
class VisibleNode;
class AABB;
class OBB;
class Sphere;
class Frustum;
class Surface;
class Collider;

struct Interval
{
    float min;
    float max;

    bool overlapOnAxis(const Interval& op, const math::vec3& axis)
    {
        return ((op.min <= min) && (min <= op.max));
    }
};

struct CollisionInfo
{
    // RayCast: nullptr, ScreenCast: nullptr, Intersect: callback owner
    const Collider* source = nullptr;
    // RayCast: target, ScreenCast: target, Intersect: the other guy
    const Collider* target = nullptr;

    // worldspace
    // AABB: collider's world translation
    math::vec3 point;

    // AABB: distance between collider's world translation and ray origin
    float distance = std::numeric_limits<float>::max();
};

struct SortCollisionInfoByDistance
{
    constexpr bool operator() (const CollisionInfo& lhs, const CollisionInfo& rhs) const
    {
        return lhs.distance < rhs.distance;
    }
};

class BoundingVolume
        : public CopyableObject
        , public w4::enable_from_this<BoundingVolume>
{
    W4_OBJECT(BoundingVolume, CopyableObject);
public:
    // visitor
    // may be it should return std::pair<bool, CollisionInfo>?
    virtual bool intersect(cref<BoundingVolume>, CollisionInfo&) const = 0;
    // actual intersections
    virtual bool intersect(const math::vec3& point, CollisionInfo&) const = 0;
    virtual bool intersect(const math::Ray&, CollisionInfo&) const = 0;
    virtual bool intersect(const AABB&, CollisionInfo&) const = 0;
    virtual bool intersect(const OBB&, CollisionInfo&) const = 0;
    virtual bool intersect(const Sphere&, CollisionInfo&) const = 0;
    virtual bool intersect(const Frustum&, CollisionInfo&) const = 0;
    // ...
    virtual sptr<BoundingVolume> clone() const = 0;

    // local transform
    void setLocalTransform(math::Transform::cref);
    math::Transform::cref   getLocalTransform() const;

    void setLocalTranslation(math::vec3::cref);
    math::vec3::cref getLocalTranslation() const;

    void setLocalRotation(math::Rotator::cref);
    math::Rotator::cref getLocalRotation() const;

    void setLocalScale(math::vec3::cref);
    math::vec3::cref getLocalScale() const;

    // world transform caching
    void onNodeTransformChanged(const math::Transform& transform);
    const math::Transform& getTransform() const;

    void update();
    virtual void onUpdate() = 0;

    template<typename T> w4::sptr<T> as()
    {
        if (!is<T>() && !derived_from<T>())
        {
            FATAL_ERROR("BoundingVolume requested as '[%s]', but actual type is '[%s]'", T::typeInfo.name(), getTypeInfo().name());
        }
        return std::static_pointer_cast<T>(shared_from_this());
    }

    template<typename T> w4::sptr<const T> as() const
    {
        if (!is<T>() && !derived_from<T>())
        {
            FATAL_ERROR("BoundingVolume requested as '[%s]', but actual type is '[%s]'", T::typeInfo.name(), getTypeInfo().name());
        }
        return std::static_pointer_cast<const T>(shared_from_this());
    }

private:
    bool m_updated = false;
    mutable bool m_transformChanged = true;
    mutable math::Transform m_worldTransform; // <- lazy evaluation, use getTransform()
    math::Transform m_localTransform;
    math::Transform m_parentTransform;
};

class Bounds
{
public:
    Bounds() = default;
    Bounds(const math::vec3& min, const math::vec3& max);
    Bounds(math::vec3 size);
    Bounds(VisibleNode* node);
    Bounds(const Surface& surface);

    math::vec3& min()                             { return m_bounds[0]; }
    [[nodiscard]] const math::vec3& min() const   { return m_bounds[0]; }

    math::vec3& max()                             { return m_bounds[1]; }
    [[nodiscard]] const math::vec3& max() const   { return m_bounds[1]; }

    math::vec3& operator[] (int i)                { return m_bounds[i]; }
    const math::vec3& operator[] (int i) const    { return m_bounds[i]; }

    std::array<math::vec3, 8> getPoints() const
    {
        const auto& n = min();
        const auto& p = max();

        return {
                    math::vec3{n.x, n.y, n.z},
                    math::vec3{n.x, n.y, p.z},
                    math::vec3{n.x, p.y, n.z},
                    math::vec3{n.x, p.y, p.z},
                    math::vec3{p.x, n.y, n.z},
                    math::vec3{p.x, n.y, p.z},
                    math::vec3{p.x, p.y, n.z},
                    math::vec3{p.x, p.y, p.z}
               };
    }

    std::array<math::vec3, 8> getPoints(const math::Transform& transform) const
    {
        const auto& n = min();
        const auto& p = max();

        return {
                math::vec3{n.x, n.y, n.z}.localToWorld(transform),
                math::vec3{n.x, n.y, p.z}.localToWorld(transform),
                math::vec3{n.x, p.y, n.z}.localToWorld(transform),
                math::vec3{n.x, p.y, p.z}.localToWorld(transform),
                math::vec3{p.x, n.y, n.z}.localToWorld(transform),
                math::vec3{p.x, n.y, p.z}.localToWorld(transform),
                math::vec3{p.x, p.y, n.z}.localToWorld(transform),
                math::vec3{p.x, p.y, p.z}.localToWorld(transform)
        };
    }

private:
    std::array<math::vec3, 2> m_bounds;
};

class AABB : public BoundingVolume
{
    W4_OBJECT(AABB, BoundingVolume);
public:
    AABB() = default;
    AABB(const AABB&) = default;
    explicit AABB(math::vec3 size);
    explicit AABB(float size);
    explicit AABB(VisibleNode*);
    explicit AABB(cref<VisibleNode>);
    explicit AABB(const Surface&);

    bool intersect(cref<BoundingVolume>, CollisionInfo&) const override;
    bool intersect(const math::vec3& point, CollisionInfo&) const override;
    bool intersect(const math::Ray&, CollisionInfo&) const override;
    bool intersect(const AABB&, CollisionInfo&) const override;
    bool intersect(const OBB&, CollisionInfo&) const override;
    bool intersect(const Sphere&, CollisionInfo&) const override;
    bool intersect(const Frustum&, CollisionInfo&) const override;

    sptr<BoundingVolume> clone() const override;

    [[nodiscard]] const Bounds& getBounds() const;
    Interval getInterval(const math::vec3& axis) const;

    // internal use only
    void setBoundsRaw(const Bounds&);
    [[nodiscard]] const Bounds& getBoundsRaw() const;
    const std::array<math::vec3, 8>& getPoints() const;

    void onUpdate() override;

private:
    void updateBounds();

    Bounds m_boundsLocal;
    Bounds m_boundsWorld;

    mutable bool m_dirtyPoints = true;
    mutable std::array<math::vec3, 8> m_points;
};

class Sphere : public BoundingVolume
{
    W4_OBJECT(Sphere, BoundingVolume);
public:
    explicit Sphere(float radius);
    explicit Sphere(VisibleNode*);
    explicit Sphere(cref<VisibleNode>);
    explicit Sphere(const Surface&);

    bool intersect(cref<BoundingVolume>, CollisionInfo&) const override;
    bool intersect(const math::vec3& point, CollisionInfo&) const override;
    bool intersect(const math::Ray&, CollisionInfo&) const override;
    bool intersect(const AABB&, CollisionInfo&) const override;
    bool intersect(const OBB&, CollisionInfo&) const override;
    bool intersect(const Sphere&, CollisionInfo&) const override;
    bool intersect(const Frustum&, CollisionInfo&) const override;

    sptr<BoundingVolume> clone() const override;

    [[nodiscard]] float getRadius() const;

    // internal use only
    void setRadiusRaw(float);
    float getRadiusRaw() const;

    void onUpdate() override;

private:
    // move this and similar method from ArcBall to w4::math
    bool solve(float a, float b, float c, float& x) const;

    float m_radiusLocal;
    float m_radiusWorld;
};

class Frustum : public BoundingVolume
{
    W4_OBJECT(Frustum, BoundingVolume);
public:
    explicit Frustum(const math::Frustum&);

    bool intersect(cref<BoundingVolume>, CollisionInfo&) const override;
    bool intersect(const math::vec3& point, CollisionInfo&) const override;
    bool intersect(const math::Ray&, CollisionInfo&) const override;
    bool intersect(const AABB&, CollisionInfo&) const override;
    bool intersect(const OBB&, CollisionInfo&) const override;
    bool intersect(const Sphere&, CollisionInfo&) const override;
    bool intersect(const Frustum&, CollisionInfo&) const override;

    sptr<BoundingVolume> clone() const override;

    const math::Frustum& getFrustum() const;

    // internal use only
    void setFrustumRaw(const math::Frustum&);
    const math::Frustum& getFrustumRaw() const;

    void onUpdate() override;

private:
    math::Frustum m_frustumLocal;
    math::Frustum m_frustumWorld;
};

class OBB : public BoundingVolume
{
    W4_OBJECT(OBB, BoundingVolume);
public:
    explicit OBB(const math::vec3&);
    explicit OBB(float);
    explicit OBB(VisibleNode*);
    explicit OBB(cref<VisibleNode>);

    bool intersect(cref<BoundingVolume>, CollisionInfo&) const override;
    bool intersect(const math::vec3& point, CollisionInfo&) const override;
    bool intersect(const math::Ray&, CollisionInfo&) const override;
    bool intersect(const AABB&, CollisionInfo&) const override;
    bool intersect(const OBB&, CollisionInfo&) const override;
    bool intersect(const Sphere&, CollisionInfo&) const override;
    bool intersect(const Frustum&, CollisionInfo&) const override;

    sptr<BoundingVolume> clone() const override;

    Interval getInterval(const math::vec3& axis) const;
    const std::array<math::vec3, 8>& getPoints() const;
    const math::vec3 getSize() const { return m_size; }
    const math::vec3 getCenter() const { return m_centerWorld; }

    void onUpdate() override;

private:
    void build(VisibleNode*);
    void update();
    math::vec3 getClosestPoint(const math::vec3&) const;
    float classify(const math::vec4& plane) const;

    math::vec3 m_centerLocal;
    math::vec3 m_centerWorld;
    math::vec3 m_size;

    mutable std::array<math::vec3, 8> m_points;
};

} // namespace w4::core