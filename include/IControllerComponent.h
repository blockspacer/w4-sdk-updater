#pragma once

#if 0

#include "Component.h"

namespace w4::render {

class IControllerComponent: public core::IComponent
{
    W4_COMPONENT(IControllerComponent, core::IComponent);

public:
    enum class Button: unsigned
    {
        A = 0,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,

        __count
    };
    enum class Axis: unsigned
    {
        LeftX = 0,
        LeftY,
        RightX,
        RightY,

        __count
    };
    enum class Action: unsigned
    {
        None,
        MoveForward,
        MoveUp,
        MoveRight,
        Pitch,
        Roll,
        Yaw,
        CameraForward,
        CameraUp,
        CameraRight,
        Crouch,
        Jump
    };

    struct ActionConfig
    {
        std::unordered_map<Button, std::pair<Action, float>> buttonMap = {
                {Button::A, {Action::Crouch, 1.f}},
                {Button::B, {Action::Jump, 1.f}},
                {Button::C, {Action::Roll, 1.f}},
                {Button::D, {Action::Roll, -1.f}},
                {Button::E, {Action::Pitch, 1.f}},
                {Button::F, {Action::Pitch, -1.f}},
                {Button::G, {Action::Yaw, 1.f}},
                {Button::H, {Action::Yaw, -1.f}},
                {Button::I, {Action::MoveUp, 1.f}},
                {Button::J, {Action::MoveUp, -1.f}}
        };

        std::unordered_map<Axis, Action> axisMap = {
                {Axis::LeftX, Action::MoveRight},
                {Axis::LeftY, Action::MoveForward},
                {Axis::RightX, Action::CameraRight},
                {Axis::RightY, Action::CameraUp}
        };
    };

public:
    IControllerComponent(CloneTag, const IControllerComponent&, core::Node&);
    virtual ~IControllerComponent();

    virtual void initialize(const variant::Variant& = {});
    virtual void update(float dt) override;

    void setActionConfig(const ActionConfig&);
    const ActionConfig& getActionConfig() const;

protected:
    explicit IControllerComponent(core::Node&);

    void stick(Axis, float);
    void buttonDown(Button);
    void buttonUp(Button);

    virtual void cloneData(const IComponent&) override;

private:
    void doAction(Action, float);

private:
    class Pimpl;
    uptr<Pimpl> m_d;
};

} //namespace w4::render

#endif