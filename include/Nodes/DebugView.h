#pragma once

#include "W4Math.h"

#include <functional>

namespace w4::core {
class Node;
class Surface;
class IComponent;
} //
namespace w4::render {

class RootNode;
class IRenderPass;
class Camera;
class IDebugView
{
public:
    virtual ~IDebugView();
    IDebugView& setDebugViewColor(math::color::cref);
    math::color::cref getDebugViewColor() const;

    IDebugView& enableDebugView(bool enable);
    bool isDebugViewEnabled() const;

    virtual void debugViewForeachSurface(std::function<void(core::Surface &)> visitor);

protected:
    virtual RootNode * debugViewGetRootNode() = 0;
    virtual void onDebugViewEnabled(bool);

private:
    bool m_isEnabled = false;
    math::color m_color = math::color(0, 1, 0, 1);
};

} //namespace w4::render