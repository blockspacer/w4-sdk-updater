#pragma once
#include "RenderPass.h"

namespace w4::render {

    class OffscreenPass : public IRenderPass
    {
    public:
        OffscreenPass(const OffscreenPass&) = delete;
        OffscreenPass& operator=(const OffscreenPass&) = delete;

        OffscreenPass(const w4::math::size& size) : OffscreenPass(size.w, size.h) {}
        OffscreenPass(uint32_t w, uint32_t h);

        void begin() override;
        void draw() override;
        void end() override;

        void setMaterialInst(const w4::sptr<resources::MaterialInst>& value);
        w4::sptr<resources::MaterialInst>& getMaterialInst() { return m_matInst; }
        const w4::sptr<resources::MaterialInst>& getMaterialInst() const { return m_matInst; }

        void setFiltering(w4::resources::Filtering filtering) { m_filtering = filtering; }
        w4::resources::Filtering getFiltering() const { return m_filtering; }

        w4::sptr<resources::Texture> getTexture() const;
    private:
        void update();

        w4::resources::Filtering m_filtering = w4::resources::Filtering::Level0;

        w4::sptr<Mesh>                     m_plane;
        w4::sptr<resources::MaterialInst>  m_matInst;
        w4::math::size                     m_size;
        w4::sptr<TextureRenderTarget>             m_renderTarget;
    };

}