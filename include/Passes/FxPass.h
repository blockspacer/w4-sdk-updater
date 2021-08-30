#pragma once
#include "RenderPass.h"

namespace w4::render {

    class FxPass : public IRenderPass
    {
    public:

        enum class FxPassRT_Id {
            Screen = -1,
            Input = 0,
            RT_1,
            RT_2,
            RT_3,
            RT_4,
            RT_5,
            RT_6,
            RT_7,
            RT_8,
            RT_9,
            RT_10
        };

        struct FxStep
        {
            std::vector<FxPassRT_Id> m_inputTargets;
            FxPassRT_Id m_outputTarget;
            sptr<resources::MaterialInst> m_material;
            w4::resources::Filtering m_filtering;
        };

        class FxElement
        {
        private:
            struct private_ctor_t {};
        public:

            FxElement(private_ctor_t, FxPass* p) : FxElement(p)
            {}

            FxElement(const FxElement&) = delete;
            FxElement& operator=(const FxElement&) = delete;

            FxElement(FxElement&&) noexcept = default;
            FxElement& operator=(FxElement&&) noexcept = default;

            FxElement& addFxStep(const std::vector<FxPassRT_Id>& inputs, FxPassRT_Id output, const sptr<resources::MaterialInst>& mat, resources::Filtering filtering = w4::resources::Filtering::Level0);
        private:
            FxElement(FxPass* p);

            friend class FxPass;

            std::vector<FxStep> m_steps;
            FxPass* parent;
        };

        FxPass();

        FxPass(const FxPass&) = delete;
        FxPass& operator=(const FxPass&) = delete;

        void begin() override;
        void draw() override;
        void end() override;

        FxPassRT_Id createFixedRenderTarget(const w4::math::size& size);
        FxPassRT_Id createRelativeRenderTarget(const w4::math::vec2& relSize);
        FxPassRT_Id addCustomRenderTarget(const w4::sptr<TextureRenderTarget>&);

        void setFixedRenderTargetSize(FxPassRT_Id, const w4::math::size& size);
        void setRelativeRenderTargetSize(FxPassRT_Id, const w4::math::vec2& relSize);

        FxElement& createFx(const std::string& name);
        void setCurrentFx(const std::string& name);

        const w4::sptr<TextureRenderTarget>& getInputRenderTarget() const { return m_inputTarget; }
        void resizeRenderTargets(const math::size& size);

        static w4::sptr<resources::MaterialInst> getDefaultMaterialInst();

    protected:
        void onEnabled() override;
    private:
        bool update();

        enum class RT_Type
        {
            Fixed,
            Relative,
            Custom
        };

        struct RT
        {
            sptr<TextureRenderTarget> rt;
            RT_Type type;
        };

        w4::sptr<Mesh> m_plane;
        math::size m_renderSize;

        std::unordered_map<std::string, size_t> m_aliasToFxId;
        size_t m_currentFx = 0;

        std::vector<FxElement> m_elements = {};
        std::vector<RT> m_renderTargets;
        w4::sptr<TextureRenderTarget>  m_inputTarget;

        std::unordered_map<size_t, math::vec2> m_relSizes;
    };
}
