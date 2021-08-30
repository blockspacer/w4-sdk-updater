#pragma once

#include "SkinnedAnimation.h"
#include "Track.h"

namespace w4::resources {

class SimpleSkinnedAnimation
    : public SkinnedAnimation
    , public core::Cache<SimpleSkinnedAnimation, sptr<SimpleSkinnedAnimation>, ResourceLoadDescr>
{
    W4_OBJECT(SimpleSkinnedAnimation, SkinnedAnimation);

public:
    static w4::sptr<SimpleSkinnedAnimation> sample();
    static w4::sptr<SimpleSkinnedAnimation> load(const ResourceLoadDescr& descr);

public:
    SimpleSkinnedAnimation(const ResourceLoadDescr& name);

    float getDuration() const override;
    float getFps() const override;

    std::vector<std::string> getBoneNames() const override;
    math::mat4 getTransform(const resources::BoneIndex& boneIndex, float time) const override;

    void addTrack(const std::string& boneName, w4::cref<TrackVec3> translationTrack, w4::cref<TrackRotator> rotationTrack, w4::cref<TrackVec3> scaleTrack);
    size_t getBonesCount() const;

    cref<TrackVec3::Super> getTranslationTrack(BoneIndex) const;
    cref<TrackRotator::Super> getRotationTrack(BoneIndex) const;
    cref<TrackVec3::Super> getScaleTrack(BoneIndex) const;

    void setDuration(float seconds);

    virtual void collectResourcesRecursive(std::unordered_set<sptr<resources::Resource>> & destination) override;

private:
    struct TrackInfo
    {
        std::string boneName;
        TrackVec3::Accessor::sptr translation;
        TrackRotator::Accessor::sptr rotation;
        TrackVec3::Accessor::sptr scale;
    };
    std::vector<TrackInfo> m_tracks;

    float m_fps;
    float m_duration;
};

} //namespace w4::resources
