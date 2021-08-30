#pragma once
#include <optional>

#include "Track.h"
#include "Nodes/DataNode.h"

namespace w4::render {

    class Spline: public core::DataNode
    {
        W4_NODE(Spline, core::DataNode)
    public:
        enum class State
        {
            Stopped,
            Playing,
            Paused
        };

        Spline(const std::string& name);
        Spline(NodeCloning, const Spline&);

        static w4::sptr<Spline> load(const std::string& source);

        virtual void onUpdate(float dt) override;

        void setRepeatable(bool isRepeatable);
        bool isRepeatable() const;

        void play(float splineTime, std::function<void(const math::Transform&)> updateHandler, std::function<void(bool)> completionHandler = [](bool){});
        bool isPlaying() const;
        void pause();
        bool isPaused() const;
        void stop();
        void setPosition(float pos);

        float getDuration() const;

        void setOrder(uint32_t order);
        uint32_t getOrder() const;
        void addKnot(float knot);
        void addControlPoint(const w4::math::vec4& point);

        const std::vector<w4::math::vec4>& getPoints() const;
        const std::vector<float>& getKnots() const;

    private:
        bool m_repeatable = 0;
        bool m_playForward = true;
        float m_curPos = 0.f;
        float m_speedMultiplier = 1.f;
        State m_state;
        std::function<void(const math::Transform&)> m_updateHandler;
        std::function<void(bool)> m_completeCallback;

        uint32_t m_order;
        std::vector<float> m_knots;
        std::vector<w4::math::vec4> m_points;
        std::vector<w4::math::vec4> m_homogeneousPoints;

        std::optional<w4::math::vec3> m_lastPos;

        std::optional<uint32_t> m_sysUpdateHandle;

        w4::math::vec3 getValue(float t) const;
        void registerUpdate();
        void unregisterUpdate();
    };

}
