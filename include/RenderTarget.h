#pragma once

#include "W4Math.h"
#include "IOuterID.h"
#include "Texture.h"

namespace w4::render {

class IRenderTarget
{
public:
    virtual void setup(const math::vec2& st = math::vec2(0.0f, 0.0f), const math::vec2& size = math::vec2(1.0f, 1.0f)) = 0;
    virtual void clear() = 0;
    virtual math::size getSize() const = 0;
private:
};

class ScreenRenderTarget
        : public IRenderTarget
{
public:
    void setup(const math::vec2& st = math::vec2(0.0f, 0.0f), const math::vec2& size = math::vec2(1.0f, 1.0f)) override;
    void clear() override;
    math::size getSize() const override;

    static sptr<ScreenRenderTarget> get();
private:
    ScreenRenderTarget() = default;
};

class TextureRenderTarget
        : public IRenderTarget
        , public core::IOuterManaged
{
public:
    TextureRenderTarget(const math::size& size, bool createDepthTexture = false, bool floatPoint = false);

    void setup(const math::vec2& st = math::vec2(0.0f, 0.0f), const math::vec2& size = math::vec2(1.0f, 1.0f)) override;
    void clear() override;
    math::size getSize() const override;

    sptr<resources::Texture> getColorTexture();
    sptr<resources::Texture> getDepthTexture();

private:
    void outerCreator() override;
    void outerDeleter() override;

    math::size                  m_size;
    sptr<resources::Texture>    m_color;
    sptr<resources::Texture>    m_depth;
    bool                        m_floatPoint;
};

}
