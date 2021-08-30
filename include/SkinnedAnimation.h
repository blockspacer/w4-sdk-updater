#pragma once

#include "Resource.h"
#include "BoneIndex.h"
#include "W4Math.h"

namespace w4::resources {

class SkinnedAnimation
    : public Resource
{
    W4_OBJECT(SkinnedAnimation, Resource);

public:
    SkinnedAnimation(const ResourceLoadDescr& descr);
    virtual ~SkinnedAnimation() noexcept;

    virtual float getDuration() const = 0;
    virtual float getFps() const = 0;
    virtual std::vector<std::string> getBoneNames() const = 0;
    virtual math::mat4 getTransform(const resources::BoneIndex& boneIndex, float time) const = 0;
};

} //namespace w4::resources
