#pragma once

#if 0

#include "IControllerComponent.h"

namespace w4::render {

class TouchControllerComponent: public IControllerComponent
{
    W4_COMPONENT(TouchControllerComponent, IControllerComponent);

public:
    enum class TouchMove
    {
        MoveHorizontal,
        MoveVertical
    };
    /*enum class TouchEvent
    {
        Tap,
        SwipeHorizontal,
        SwipeVertical,
        Zoom
    };*/

    struct Config
    {
        struct AxisInfo
        {
            Axis axis;
            float sensitivity;
        };

        //std::unordered_map<TouchEvent, Button> buttonMap;
        std::unordered_map<TouchMove, AxisInfo> axisMap = {
                {TouchMove::MoveHorizontal, {Axis::RightX, 0.005f}},
                {TouchMove::MoveVertical, {Axis::RightY, 0.005f}}
        };
    };

public:
    explicit TouchControllerComponent(core::Node&);
    ~TouchControllerComponent();

    void preInitialize() override;
    void postInitialize() override;

    void setConfig(const Config&);
    const Config& getConfig() const;

private:
    class Pimpl;
    uptr<Pimpl> m_d;
};

} //namespace w4::render

    #endif