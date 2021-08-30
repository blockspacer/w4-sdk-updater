#pragma once

#include "Nodes/VisibleNode.h"
#include "PodGenerator.h"
#include "VerticesBuffer.h"
#include "SkinnedAnimation.h"
#include "Animator.h"
#include "Skeleton.h"
#include "Skin.h"
#include "Tween.h"

#include <array>
#include <optional>

POD_STRUCT(SkinnedMeshVertexFormat,
           POD_FIELD(w4::math::vec3, w4_a_position)
           POD_FIELD(w4::math::vec3, w4_a_normal)
           POD_FIELD(w4::math::vec3, w4_a_tangent)
           POD_FIELD(w4::math::vec3, w4_a_bitangent)
           POD_FIELD(w4::math::vec2, w4_a_uv0)
           POD_FIELD(w4::math::vec2, w4_a_uv1)
           POD_FIELD(w4::math::vec2, w4_a_uv2)
           POD_FIELD(w4::math::vec2, w4_a_uv3)
           POD_FIELD(w4::math::vec4, w4_a_color)
           POD_FIELD(w4::math::vec4, w4_a_boneId0)
           POD_FIELD(w4::math::vec4, w4_a_boneId1)
           POD_FIELD(w4::math::vec4, w4_a_boneWeight0)
           POD_FIELD(w4::math::vec4, w4_a_boneWeight1)
);

#define W4_MAX_BONES 16

namespace w4::render {

void packageBonesIds(SkinnedMeshVertexFormat& vf, resources::BoneIndex, resources::BoneIndex = 0, resources::BoneIndex = 0, resources::BoneIndex = 0, resources::BoneIndex = 0, resources::BoneIndex = 0, resources::BoneIndex = 0, resources::BoneIndex = 0);
void packageBonesIds(SkinnedMeshVertexFormat& vf, resources::BoneIndex const* first, size_t size);

void packageBonesWeights(SkinnedMeshVertexFormat& vf, float, float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0, float = 0.0);
void packageBonesWeights(SkinnedMeshVertexFormat& vf, float const* first, size_t size);

class SkeletonSplitSurface;
class SkeletonSplitSubSurface: public core::IOuterManaged
{
public:
    SkeletonSplitSubSurface(const SkeletonSplitSubSurface&) = delete;
    SkeletonSplitSubSurface(SkeletonSplitSubSurface&&) = delete;
    SkeletonSplitSubSurface& operator=(const SkeletonSplitSubSurface&) = delete;
    SkeletonSplitSubSurface& operator=(SkeletonSplitSubSurface&&) = delete;

    SkeletonSplitSubSurface(SkeletonSplitSurface & surface, const std::vector<std::string>& bones, w4::cref<resources::IIndicesBuffer> indices);
    SkeletonSplitSubSurface(SkeletonSplitSurface & surface, const SkeletonSplitSubSurface& rh);
    ~SkeletonSplitSubSurface();

    const std::vector<std::string>& getBonesNames() const;
    w4::cref<resources::IIndicesBuffer> getIndicesBuffer() const;

    virtual void outerCreator();
    virtual void outerDeleter();

    void onShadowReceiverChanged();

    void onRender();

    void setBoneWorldTransform(resources::BoneIndex skeletonBoneIndex, math::mat4::cref transform);

    void onMaterialChanged(w4::cref<resources::MaterialInst>);
    void onSkeletonChanged(w4::cref<resources::Skeleton>);
    void onVerticesBufferChanged(w4::cref<resources::IVerticesBuffer>);

private:
    SkeletonSplitSurface & m_surface;
    std::vector<std::string> m_boneNames;
    w4::sptr<resources::IIndicesBuffer> m_indicesBuffer;
//
    std::vector<resources::BoneIndex> m_skeletonToSurfaceMap;
    std::vector<math::mat4> m_boneTransforms;
};

class SkinnedMesh;
class SkeletonSplitSurface: public Surface
{
    W4_OBJECT(SkeletonSplitSurface, Surface);
public:
    SkeletonSplitSurface(SkinnedMesh & owner, const std::string& name);
    SkeletonSplitSurface(SurfaceCloning, const SkeletonSplitSurface& from, SkinnedMesh & owner);
    uptr<Surface> clone(core::VisibleNode & owner) const override;

    const std::vector<uptr<SkeletonSplitSubSurface>>& getSubSurfaces() const;

    SkeletonSplitSubSurface& addSubSurface(const std::vector<std::string> & bones, w4::cref<resources::IIndicesBuffer> indices);
    SkeletonSplitSurface& setBoneWorldTransform(resources::BoneIndex skeletonBoneIndex, math::mat4::cref transform);

    virtual void outerCreator() override;
    virtual void outerDeleter() override;


    virtual void onRender(const IRenderPass&) override;

    void onSkeletonChanged(w4::cref<resources::Skeleton>);
    virtual void onMaterialChanged(w4::cref<resources::MaterialInst>) override;
    virtual void onVerticesBufferChanged(w4::cref<resources::IVerticesBuffer>) override;

    void setShadowReceiver(bool flag) override;
private:
    std::vector<uptr<SkeletonSplitSubSurface>> m_subSurfaces;
};

class SkinnedMesh final: public core::VisibleNode
{
    W4_NODE(SkinnedMesh, core::VisibleNode);

public:
    static constexpr w4::resources::BoneIndex maxBonesPerVertice = 8;
    static constexpr w4::resources::BoneIndex maxBonesPerSurface = W4_MAX_BONES;

    using VertexFormat = SkinnedMeshVertexFormat;
    using VerticesBuffer = resources::IVerticesBufferSpec<VertexFormat>;
    using IndicesBuffer = resources::IIndicesBuffer;
    using Skeleton = resources::Skeleton;
    using BoneIndex = resources::BoneIndex;
    using Skin = resources::Skin;


    class Animator final: public w4::render::Animator
    {
    public:
        Animator(SkinnedMesh & s, w4::cref<resources::SkinnedAnimation> animation, size_t index);

        w4::cref<resources::SkinnedAnimation> getAnimation() const;
        const std::vector<float>& getAnimatedBonesWeights() const;

        size_t getIndex()const;
        float getWeight() const;
        void setWeight(float);
        void setWeight(float weight, float dt, resources::Tween<float>::TweenFunction function = resources::easing::Linear<float>, float fps = 5.f);

        void onUpdate(float dt);

        float getDuration() const;
        float getFps() const;

        void processUpdate(float currentTime);

    private:
        SkinnedMesh & self;
        const size_t c_index;
        const w4::sptr<resources::SkinnedAnimation> c_animation;
        std::vector<std::pair<BoneIndex, BoneIndex>> m_indicesMapping;

        float m_weight = 0.f;

        uptr<resources::Tween<float>> m_weightTween;
        float m_tweenTimePassed;
        float m_tweenDuration;

        std::vector<float> m_animatedBonesWeights;
    };

public:
    static w4::sptr<SkinnedMesh> sample();

    explicit SkinnedMesh(const std::string& name);
    SkinnedMesh(NodeCloning, const SkinnedMesh& from);
    SkinnedMesh(const std::string& name, w4::cref<resources::IVerticesBuffer> vertices);
    ~SkinnedMesh() noexcept;

    uint32_t getSkeletonSplitSurfacesCount() const;
    void foreachSkeletonSplitSurface(std::function<void(SkeletonSplitSurface&)>);
    void foreachSkeletonSplitSurface(std::function<void(const SkeletonSplitSurface&)>) const;

    SkeletonSplitSurface& addSplittedSurface(const std::string& name);
    SkeletonSplitSubSurface& addSplittedSubSurface(core::StringHash name, w4::cref<resources::IIndicesBuffer> indices, const std::vector<std::string>& bones);

    std::vector<std::string> getBoneNames() const;

    w4::cref<Skeleton> getSkeleton() const;

    void setSkeleton(w4::cref<Skeleton>);
    void setSkin(w4::cref<Skin> bones);
    void setBonesOffset(math::mat4::cref);
    Animator& addAnimation(w4::cref<resources::SkinnedAnimation>);

    void onRender(const IRenderPass&) override;
    void onUpdate(float dt) override;

    w4::cref<core::Node> createSocket(BoneIndex);
    w4::cref<core::Node> createSocket(const std::string& boneName);

    w4::cref<core::Node> getSocket(BoneIndex) const;
    w4::cref<core::Node> getSocket(const std::string& boneName) const;

    resources::BoneIndex getBoneIndex(const std::string& boneName) const;
    void setBoneLocalTransform(const resources::BoneIndex& boneIndex, const math::mat4& transform);

    std::vector<std::string> getAvailableAnimations() const;
    bool haveAnimation(const std::string& animationName) const;
    size_t getAnimationIndex(const std::string& animationName) const;
    const std::string& getAnimationName(size_t animationIndex) const;

    void setAnimationStateChangedCallback(std::function<void(Animator&, Animator::State)>);
    void setAnimationStateChangedCallback(Animator::State, std::function<void(Animator&)>);

    Animator & getAnimator(size_t);
    Animator & getAnimator(const std::string&);

    const Animator & getAnimator(size_t) const;
    const Animator & getAnimator(const std::string&) const;

    bool removeAnimator(const std::string& name);
    void removeAllAnimations();

    w4::cref<resources::Skin> getSkin() const;
    math::mat4 getBonesOffset() const;

    void play(const std::string& animationName, float duration = 0.f); //eq play({{animationName, 1.0f}})
    void play(size_t animationIndex = 0, float duration = 0.f); //eq play ({{animationIndex, 1.0f}})
    void play(std::initializer_list<std::pair<std::string, float>>, float duration = 0.f);
    void play(std::initializer_list<std::pair<size_t, float>>, float duration = 0.f);

    void pause(); //pause all
    void resume(); //resume all
    void stop(); //stop all

    bool isPlaying() const; //is any animation played

    uint32_t getRenderType() const override;

//tool
    virtual void collectResourcesRecursive(std::unordered_set<sptr<resources::Resource>> & destination) override;

protected:
    virtual void onSurfaceAdded(const Surface& surface) override;
    virtual void onSurfaceRemoved(const Surface& surface) override;

private:
    void onTransformChange() override;
    void handleAnimatorStart(const w4::render::Animator&);

    void updateBonesTransform();
    void calculateBones();
    template<bool IsRoot> void calcBoneTransform(BoneIndex index, BoneIndex parentIndex);

private:
    struct BoneInfo
    {
        BoneInfo(BoneIndex inSkinIndex, BoneIndex inParentSkeletonIndex, math::mat4::cref inLocalTransform);

        const BoneIndex skinIndex;
        const BoneIndex parentIndex;
        math::mat4 localTransform;
        math::mat4 animationTransform;

        math::mat4 globalTransform;
        bool isLocalTransformChanged = true;
        bool isTransformAnimated = false;

        w4::sptr<core::Node> socket;
    };

    w4::sptr<resources::Skeleton> m_skeleton;
    size_t m_skeletonRootsCount;
    w4::sptr<resources::Skin> m_skin;

    std::vector<uptr<Animator>> m_animators;
    std::unordered_map<std::string, size_t> m_animatorsIndices;
    std::vector<float> m_animatorsWeightsSumm;

    std::vector<BoneInfo> m_bones;
    std::vector<math::mat4> m_bonesWorldTransforms;
    math::mat4 m_bonesOffset = math::mat4::identity;

    bool m_isBonesPositionsChanged = false;
    bool m_isMeshTransformChanged = false;
    std::optional<uint32_t> m_updateHandle;

    std::unordered_map<uint32_t, SkeletonSplitSurface*> m_skeletonSplitSurfaces;
};

} //namespace w4::render
