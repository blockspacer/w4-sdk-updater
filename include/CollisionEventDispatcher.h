#pragma once

#include <optional>
#include <unordered_set>
#include <vector>

#include "W4Common.h"
#include "W4Math.h"
#include "BoundingVolume.h"
#include "Input.h"

namespace w4::core {

class Collider;
enum class ScreencastEvent;

// manual(current) or auto queue control?
class CollisionEventDispatcher
{
public:
    CollisionEventDispatcher() = delete;
    CollisionEventDispatcher(const CollisionEventDispatcher&) = delete;

    static void addIntersectCollider(Collider* collider);
    static void removeIntersectCollider(Collider* collider);

    static void addActiveCollider(Collider* collider);
    static void removeActiveCollider(Collider* collider);

    static void addScreencastCollider(Collider* collider);
    static void removeScreencastCollider(Collider* collider);

    static void addRaycastCollider(Collider* collider);
    static void removeRaycastCollider(Collider* collider);

    static CollisionInfo raycast(const math::Ray& ray);
    static std::vector<CollisionInfo> raycastAll(const math::Ray& ray);

private:
    static void subscribeTouch();
    static void unsubscribeTouch();

    static void subscribeUpdate();
    static void unsubscribeUpdate();
    static void onUpdate(float dt);

    static void onTouch(const math::point&, ScreencastEvent);

    static w4::sptr<event::Touch::Handle> m_touchHndl;
    static std::optional<uint32_t> m_updateHndl;

    static std::unordered_set<Collider*> m_activeColliders; // for intersect
    static std::unordered_set<Collider*> m_intersectColliders; // if empty, don't calc intersections
    static std::unordered_set<Collider*> m_screencastColliders;
    static std::unordered_set<Collider*> m_raycastColliders;
};

} // namespace w4::core