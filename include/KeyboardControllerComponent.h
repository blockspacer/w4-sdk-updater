#pragma once

#if 0

#include "IControllerComponent.h"
#include "Input.h"

namespace w4::render {

class KeyboardControllerComponent final: public IControllerComponent
{
    W4_COMPONENT(KeyboardControllerComponent, IControllerComponent);

public:
    struct Config
    {
        struct AxisInfo
        {
            Axis axis;
            float power = 1.f;
        };
        std::unordered_map<event::Keyboard::Key, Button> mapToButtons = {
                {event::Keyboard::Key::CtrlLeft, Button::A},
                {event::Keyboard::Key::Space, Button::B},
                {event::Keyboard::Key::Q, Button::D},
                {event::Keyboard::Key::E, Button::C},
                {event::Keyboard::Key::ArrowUp, Button::F},
                {event::Keyboard::Key::ArrowDown, Button::E},
                {event::Keyboard::Key::ArrowLeft, Button::H},
                {event::Keyboard::Key::ArrowRight, Button::G},
                {event::Keyboard::Key::Z, Button::I},
                {event::Keyboard::Key::X, Button::J}
        };
        std::unordered_map<event::Keyboard::Key, AxisInfo> mapToStick = {
                {event::Keyboard::Key::W, {Axis::LeftY, +1.f}},
                {event::Keyboard::Key::S, {Axis::LeftY, -1.f}},
                {event::Keyboard::Key::A, {Axis::LeftX, -1.f}},
                {event::Keyboard::Key::D, {Axis::LeftX, +1.f}}
        };
    };

public:
    explicit KeyboardControllerComponent(core::Node&);
    ~KeyboardControllerComponent();

    void ctorParameters();

    void preInitialize() override;
    void postInitialize() override;
    void update(float dt) override;
    void preFinalize() override;

    void setConfig(const Config&);
    const Config& getConfig() const;

protected:
    virtual void cloneData(const IComponent&) override;

private:
    class Pimpl;
    uptr<Pimpl> m_d;
};

} //namespace w4::render

#endif