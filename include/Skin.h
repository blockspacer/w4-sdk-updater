#pragma once

#include "Resource.h"
#include "BoneIndex.h"
#include "W4Math.h"

#include <unordered_map>

namespace w4::resources {

class Skin final: public Resource
{
    W4_OBJECT(Skin, Resource);
public:
    explicit Skin(const ResourceLoadDescr& name);
    ~Skin() noexcept;

    BoneIndex getBoneIndex(const std::string& boneName) const;
    BoneIndex getBonesCount() const;
    std::vector<std::string> getBoneNames() const;
    math::mat4::cref getInvertedBindPose(BoneIndex boneIndex) const;
    void addInvertedBindPose(std::string boneName, w4::math::mat4::cref invertedBindPose);

    size_t getBinarySize() const;
    size_t toBinary(void * destination, size_t bufferSize) const;
    void fromBinary(const void* source, size_t bufferSize);

private:
    std::vector<std::pair<std::string, math::mat4>> m_invertedBindPoses;
    std::unordered_map<std::string, BoneIndex> m_bonesIndicesMap;
};

} //namespace w4::resources