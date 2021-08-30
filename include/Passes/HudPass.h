#pragma once
#include "RenderPass.h"

namespace w4::render {

    class HudPass : public IRenderPass
    {
    public:
        HudPass() : HudPass(1024, 1024) {}
        HudPass(const w4::math::size& size) : HudPass(size.w, size.h) {}
        HudPass(uint32_t w, uint32_t h);

        HudPass(const HudPass&) = delete;
        HudPass& operator=(const HudPass&) = delete;

        void begin() override;
        void draw() override;
        void end() override;

        w4::sptr<Mesh> appendPlane(const std::string& name, const math::vec2& pos, const math::vec2& size, const w4::sptr<resources::Texture>& texture);
        bool removePlane(const std::string& name);
        bool removePlane(w4::cref<Mesh> plane);
        w4::cref<Mesh> getPlane(const std::string& name) const;
    private:
        std::vector<w4::sptr<Mesh>>   m_planes;
        w4::sptr<resources::Material> m_material;
        w4::math::size                m_size;
    };

}
