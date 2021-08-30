#pragma once

#include <chrono>
#include <queue>
#include "W4Math.h"

namespace w4::math {

class ArcBall
{

public:
    ArcBall() {}

    void init(float radius, const vec3& eye, const vec3& up, const vec3& sphereCenter, const mat4& projectionView, const vec4& viewport);
    void stop();

    void setSensitivity(float v);
    float getSensitivity() const;
    void setFriction(float v);
    void setVelocity(const Rotator& v);

    void onPress(float x, float y);
    void onMove(float x, float y);
    void onRelease(float x, float y);

    void onUpdate(float dt);

    Rotator getQuaternion() const;

private:
    std::pair<bool, vec3> sphere_coords(float x, float y);
    bool solve(float a, float b, float c, float& x);

    // utility
    static vec3 unProject(const vec3& source, const mat4& projectionView, const vec4& viewport);
    Rotator calcQuaternion(const vec3& from, const vec3& to, float a);

private:
    float m_radius = 1.0;
    float m_sensitivity = 1.f;
    float m_friction = 0.95f;

    vec3 m_begin = vec3::zero;
    vec3 m_current = vec3::zero;
    vec3 m_end = vec3::zero;

    Rotator m_velocity;
    Rotator m_quaternion;

    std::chrono::system_clock::time_point m_timeBegin;
    std::chrono::system_clock::time_point m_timeEnd;

    vec3 m_rayOrigin = {0.0, 0.0, 1.0};
    vec3 m_sphereCenter = {0.0, 0.0, 0.0};

    mat4 m_projectionView = mat4::identity;
    vec4 m_viewport = {0.0, 0.0, 640.0, 480.0};

//    constexpr static size_t MAX_POINTS = 4;
//    std::queue<Point> m_points;
};

}
