#pragma once

#if 0

#include "Nodes/SkinnedMesh.h"
#include "MovementComponent.h"
#include "SkinnedAnimation.h"

namespace w4::render {

class MovementAnimationComponent final: public core::IComponent
{
    W4_COMPONENT(MovementAnimationComponent, core::IComponent);
public:
    MovementAnimationComponent(core::Node& owner);
    ~MovementAnimationComponent();

    void preInitialize() override;
    void initialize() override;
    void preFinalize() override;
    SkinnedMesh& getSkinnedMesh() const;
    std::map<MovementComponent::State, size_t>& getAnimationMapping() const;

    void addStateAnimation(MovementComponent::State, w4::cref<resources::SkinnedAnimation>);
    void addStateAnimation(MovementComponent::State, size_t animationIndex);
    void addStateAnimation(MovementComponent::State, const std::string& animName);

protected:
    void cloneData(const core::IComponent&) override;

private:
    class Pimpl;
    uptr<Pimpl> m_d;
};

} //namespace w4::render

#endif