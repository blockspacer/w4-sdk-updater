#pragma once

#include <cstdint>

namespace w4::resources {

using BoneIndex = uint16_t;
static constexpr BoneIndex invalidBoneIndex = std::numeric_limits<BoneIndex>::max();
static constexpr BoneIndex maxBonesCount = std::numeric_limits<BoneIndex>::max() - 1;
inline static bool isBoneIndexValid(const BoneIndex& idx)
{
    return idx != invalidBoneIndex;
}

} //namespace w4::resources
