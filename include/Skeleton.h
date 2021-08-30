#pragma once

#include "W4Math.h"
#include "Resource.h"
#include "BoneIndex.h"
#include "Cache.h"

namespace w4::resources {

class Skeleton: public Resource, public core::Cache<Skeleton>
{
    W4_OBJECT(Skeleton, Resource);
public:
    struct BoneHierarchyInfo
    {
        BoneHierarchyInfo(BoneIndex inIndex, BoneIndex inParentIndex);

        const BoneIndex index;
        const BoneIndex parentIndex;
    };
    using BonesHierarchyContainer = std::vector<BoneHierarchyInfo>;
public:
    explicit Skeleton(const ResourceLoadDescr& descr);
    ~Skeleton();

    static w4::sptr<Skeleton> sample();
    static w4::sptr<Skeleton> load(const std::string&);

    size_t getBonesRootsCount() const;
    std::vector<std::string> getBoneNames() const;
    BoneIndex getBonesCount() const;

    const BonesHierarchyContainer& getBonesHierarchy() const;
    BoneIndex getBoneIndex(const std::string& boneName) const;
    BoneIndex getParentIndex(BoneIndex) const;
    math::mat4::cref getBoneLocalTransform(BoneIndex) const;

    size_t binarySize() const;
    size_t toBinary(void * destination, size_t bufferSize) const;
    void fromBinary(const void * source, size_t bufferSize);

    void combineWith(const Skeleton& other);
    BoneIndex addBone(std::string boneName, math::mat4::cref localTransform, BoneIndex parent = invalidBoneIndex);
    void calculateHierarchy();

private:
    struct BoneInfo
    {
        std::string name;
        math::mat4 localTransform;

        BoneIndex parent;
        std::vector<BoneIndex> children;
    };

private:

    std::vector<BoneInfo> m_bones;
    std::vector<BoneIndex> m_roots;
    std::unordered_map<std::string, BoneIndex> m_bonesIndices;
    BonesHierarchyContainer m_hierarchy;
};

} //namespace w4::resources
