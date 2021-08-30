#pragma once

#include <cmath>
#include <ostream>
#include <algorithm>
#include <limits>
#include <random>
#include <ctime>
#include <array>
#include <optional>

#include "W4Common.h"
#include "impl/W4MathIvec.h"

namespace w4::math {

static constexpr float EPSILON                 = 1e-5f;
static constexpr float MATRIX_INVERSE_EPSILON  = 1e-14f;
static constexpr float MATRIX_EPSILON          = 1e-5f;
static constexpr float PI                      = 3.141592654f;
static constexpr float TWO_PI                  = 2.0f * PI;
static constexpr float HALF_PI                 = 0.5f * PI;
static constexpr float SQRT_TWO                = 1.41421356237309504880f;
static constexpr float SQRT_THREE              = 1.73205080756887729352f;
static constexpr float DEG2RAD                 = PI/180.0f;
static constexpr float RAD2DEG                 = 180.0f/PI;
static constexpr float INF                     = 1e+9f;

class Rotator;
class Transform;

#include "impl/W4MathSwizzze.inl"

struct size
{
    size() = default;
    size(uint w, uint h)
        : w(w)
        , h(h)
    {}
    uint w;
    uint h;
    const size& operator/=(uint v) { w /= v; h /= v; return *this; }
    bool operator==(const size& rh) const { return w == rh.w && h == rh.h; }
    bool operator!=(const size& rh) const { return !operator==(rh); }
};

template<typename T>
struct TPoint
{
    using cref = const TPoint &;

    TPoint() = default;

    TPoint(T x, T y)
        :x(x), y(y) {}

    bool operator==(cref rh)  const { return x == rh.x && y == rh.y; }
    bool operator!=(cref rh) const { return !operator==(rh); }

    TPoint operator-(TPoint::cref rh) const { return {x - rh.x, y - rh.y};}

    T x;
    T y;
};

using point = TPoint<int>;

class vec2
{
public:
    using cref = const vec2&;
    using value_type = float;

    static const vec2 zero;

    vec2() = default;
    constexpr explicit vec2(const float v): x(v), y(v){}
    constexpr vec2(float _x, float _y): x(_x), y(_y){}

    vec2(cref src) = default;
    vec2(vec2&& src) = default;

    float dot(cref op) const;
    vec2 normalize() const;
    vec2 inverse() const;
    float length() const;
    float manhattanLength() const;
    bool equals(const vec2& op, float tolerance) const;
    float toAngle() const;

    vec2 operator+(cref op) const;
    vec2 operator+(const float op) const;
    vec2 operator-(cref op) const;
    vec2 operator-(const float op) const;
    vec2 operator*(cref op) const;
    vec2 operator/(cref op) const;

    vec2 operator*(float op) const;
    friend vec2 operator*(float, cref op);

    vec2 operator/(float op) const;

    vec2& operator+=(cref op);
    vec2& operator-=(cref op);

    vec2& operator*=(float op);
    vec2& operator/=(float op);

    vec2& operator=(cref op) = default;
    vec2& operator=(vec2&& op) = default;

    bool operator==(const vec2& op) const;
    bool operator!=(const vec2& op) const;

    union
    {
        struct
        {
            union { float x, u, s, w; };
            union { float y, v, t, h; };
        };
        SWIZZLE2_2(2, vec2)
        float elements[2] = {0};
    };

    float& operator[](unsigned int idx) { return elements[idx]; }

    float const& operator[](unsigned int idx) const { return elements[idx]; }
};

//static ivec2 operator-(const point& left, const point& right)
//{
//    return ivec2{left.x - right.x, left.y - right.y};
//}

class mat3;

class vec3
{
public:
    using cref = const vec3&;
    using value_type = float;

    vec3() = default;
    constexpr explicit vec3(const float v): x(v), y(v), z(v){}
    constexpr vec3(float _x, float _y, float _z): x(_x), y(_y), z(_z){}

    vec3(cref src) = default;
    vec3(vec3&& src) = default;

    void set(float val);
    void set(float x, float y, float z);

    float dot(cref op) const;
    vec3 cross(cref op) const;
    Rotator toRotator(cref up) const;
    vec3 normalize() const;
    vec3& normalizeSelf();
    vec3 inverse() const;
    float length() const;
    float manhattanLength() const;
    bool equals(cref op, float Tolerance) const;
    vec3 rotate(const Rotator& rot) const;
    vec3 transform(const Transform& t) const;
    [[nodiscard]] vec3 worldToLocal(const Transform& transform) const;
    [[nodiscard]] vec3 localToWorld(const Transform& transform) const;
    vec3 lerp(const vec3& op, float a) const;

    vec3 operator*(const mat3& op) const;

    //    ToString
    vec3 operator+(cref op) const;
    vec3 operator-() const;
    vec3 operator-(cref op) const;
    vec3 operator*(cref op) const;
    vec3 operator/(cref op) const;

    vec3 operator*(float op) const;
    friend vec3 operator*(float, cref op);
    vec3 operator+(float op) const;
    friend vec3 operator+(float, cref op);
    vec3 operator-(float op) const;
    friend vec3 operator-(float, cref op);

    vec3 operator/(float op) const;

    vec3& operator+=(cref op);
    vec3& operator-=(cref op);
    vec3& operator*=(cref op);
    vec3& operator/=(cref op);

    vec3& operator*=(float op);
    vec3& operator/=(float op);

    vec3& operator=(cref op) = default;
    vec3& operator=(vec3&& op) = default;

    bool operator<(cref op) const;
    bool operator<=(cref op) const;
    bool operator>(cref op) const;
    bool operator>=(cref op) const;

    bool operator==(const vec3& op) const;
    bool operator!=(const vec3& op) const;

    bool between(cref point1, cref point2, const float tolerance = EPSILON) const;
    bool isValid() const;
    bool isFinite() const;

    void removeComponent(const vec3& op);
    bool getIntersectPlane(vec3& normal, vec3& pointOnPlane, vec3& point1, vec3& point2);
    float getDistanceFromLine(const vec3& point1, const vec3& point2);
    float getDistanceFromLine2(vec3& project, const vec3& pointA, const vec3& pointB);

//    mat3 operator^ (const mat3& m) const;

    bool isConsiderZero() const;

    void setMin(const vec3& v1, const vec3& v2);
    void setMax(const vec3& v1, const vec3& v2);

    static const vec3 zero;

    friend std::ostream& operator<<(std::ostream& o, const vec3& a)  // NB: testing only
    {
        o << "{ " << a.x << ", " << a.y << ", "<< a.z << " }";
        return o;
    }

public:
    union
    {
        struct
        {
            union { float x, r, s; };
            union { float y, g, t; };
            union { float z, b, p; };
        };
        SWIZZLE2_2(3, vec2)
        SWIZZLE2_3(3, vec2)
        SWIZZLE3_3(3, vec3)
        float elements[3] = {0};
    };

    float& operator[](unsigned int idx) { return elements[idx]; }

    float const& operator[](unsigned int idx) const { return elements[idx]; }
};

class vec4
{
public:
    static const vec4 zero;
    using value_type = float;

    union
    {
        struct
        {
            union { float x, r, s; };
            union { float y, g, t; };
            union { float z, b, p; };
            union { float w, a, q; };
        };
        SWIZZLE2_2(4, vec2)
        SWIZZLE2_3(4, vec2)
        SWIZZLE2_4(4, vec2)
        SWIZZLE3_3(4, vec3)
        SWIZZLE3_4(4, vec3)
        SWIZZLE4(4, vec4)
        float elements[4] = {0};
    };

    using cref = const vec4&;

    vec4() = default;
    constexpr vec4(const float v): x(v), y(v), z(v), w(v){}
    vec4(const vec3&, float w);
    vec4(float x, float y, float z, float w);

    vec4(cref src) = default;
    vec4(vec4&& src) = default;

    bool equals(const vec4& op, float Tolerance) const;

    float dot(cref op) const;
    vec4& operator=(cref op) = default;
    vec4& set(vec3::cref op1, float op2 = 0);
    vec4& operator=(vec4&& op) = default;
    vec4& operator+=(cref op);

    vec4 operator/(cref op) const;
    vec4 operator/(float op) const;
    vec4 operator-(cref op) const;
    vec4 operator-(float op) const;
    vec4 operator*(float op) const;
    vec4 operator*(cref op) const;
    vec4 operator+(cref op) const;
    vec4 operator+(float op) const;

    bool operator==(const vec4& op) const;
    bool operator!=(const vec4& op) const;

    float& operator[](unsigned int idx) { return elements[idx]; }

    float const& operator[](unsigned int idx) const { return elements[idx]; }

    vec3& asVec3();
    const vec3& asVec3() const;
};

const vec3 zeroVector = {0.0f, 0.0f, 0.0f};
const vec3 forwardVector = {0.0f, 0.0f, 1.0f};
const vec3 upVector = {0.0f, 1.0f, 0.0f};
const vec3 rightVector = {1.0f, 0.0f, 0.0f};

vec3 operator/(float op1, vec3::cref op2);

class mat3;

struct Quaternion
{
public:
    using cref = const Quaternion&;
    static const Quaternion identity;

public:
    constexpr Quaternion(): x(0.f), y(.0f), z(.0f), w(1.0f) {}
    Quaternion(float x, float y, float z, float w);
    Quaternion(vec4::cref quaternion);
    Quaternion(vec3::cref vector, float scalar);
    Quaternion(const Quaternion&) = default;

    float getZAngle() const;
    float getYAngle() const;
    float getXAngle() const;

    float getRoll() const;
    float getPitch() const;
    float getYaw() const;
    math::vec3 getEuler() const;
    std::tuple<math::vec3, float> getAxisAndAngle() const;

    float getNorm() const;
    float getMagnitude() const;
    Quaternion getConjugate() const;
    Quaternion getInverse() const;
    Quaternion getNormalized() const;

    Quaternion & conjugate();
    Quaternion & inverse();
    Quaternion & normalize();

    vec3 rotate(vec3::cref) const;

    Quaternion slerp(Quaternion::cref dst, float part) const;
    float dot(cref rh) const;

    Quaternion & set(float x, float y, float z, float w);

    mat3 toMat3() const;

    bool isNearlyZero(float epsilon = EPSILON) const;
    bool isEquals(cref rh, float epsilon = EPSILON) const;

    Quaternion operator+(cref) const;
    Quaternion operator-(cref) const;
    Quaternion operator*(cref) const;
    Quaternion operator*(float) const;
    Quaternion operator/(float) const;
    Quaternion operator*(vec3::cref) const;

    Quaternion & operator+=(cref);
    Quaternion & operator-=(cref);
    Quaternion & operator*=(cref);
    Quaternion & operator*=(float);
    Quaternion & operator/=(float);

    Quaternion & operator=(const Quaternion&) = default;

    float & operator[](size_t);
    float operator[](size_t) const;

public:
    static Quaternion fromMat3(const mat3&);
    static Quaternion fromEuler(float pitch, float yaw, float roll);
    static Quaternion fromEuler(vec3::cref euler);
    static Quaternion fromAxisAndAngle(vec3::cref axis, float angle);

public:
    struct QuaternionView
    {
        vec3 vector;
        float scalar;
    };
    union
    {
        vec4 vector4;
        QuaternionView view;
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float elements[4];
    };
};

class Rotator
{
public:
    using cref = const Rotator&;

    Rotator();
    Rotator(float inX, float inY, float inZ, float inW);
    Rotator(vec4::cref quat);
    Rotator(Quaternion::cref quat);
    Rotator(float eulerX, float eulerY, float eulerZ);
    Rotator(vec3::cref euler);
    Rotator(vec3::cref axis, float angle);
    Rotator(Rotator const& rh) = default;
    Rotator(Rotator&& rh) = default;

    void set(vec3::cref axis, float angle);
    void set(float inX, float inY, float inZ, float inW);
    void reset();

    void getAxisAngle(vec3& axis, float& angle) const;
    float eulerRoll() const;
    float eulerPitch() const;
    float eulerYaw() const;
    vec3 euler() const;
    vec3 toVector() const;
    bool equals(const Rotator& op, float Tolerance) const;
    Rotator getInverse() const;
    Rotator& invert();
    Rotator slerp(const Rotator& dst, float a) const; // a=[0 ; 1]
    Rotator& normalizeSelf();
    Rotator normalize() const;
    bool isNearlyZero(float Tolerance = math::EPSILON) const;
    Rotator operator+(const Rotator& op) const;
    Rotator operator-() const;
    Rotator operator-(const Rotator& op) const;
    Rotator operator*(float) const;
    vec3 operator*(const vec3&) const;

    Rotator& operator+=(const Rotator& op);
    Rotator& operator-=(const Rotator& op);
    Rotator& operator*=(float);

    Rotator& operator=(Rotator const& op) = default;
    Rotator& operator=(Rotator&& op) = default;

    operator vec4() const;
    operator mat3() const;

public:
    Quaternion quaternion;
};

class mat3;
class mat4;

class Transform
{

public:

    using cref = const Transform&;

    Transform();
    Transform(const Rotator& rotation, vec3::cref translation, vec3::cref scale);

    Rotator& rotation();
    vec3& translation();
    vec3& scale();

    Rotator const& rotation() const;
    vec3::cref translation() const;
    vec3::cref scale() const;

    Transform rotate(const Rotator& rotation) const;
    Transform getInverse() const;

    mat4 getMatrix() const;
    mat4 getInverseMatrix() const;
    mat3 getMat3() const;
    mat3 getInverseMat3() const;
    mat3 getTransposedInverseMat3() const;

    Transform operator+(const Transform& op) const;
    Transform operator-(const Transform& op) const;

    void setIdentity();
    bool isIdentity() const;

private:
    Rotator m_rotator;
    vec3 m_translation;
    vec3 m_scale;

};

class mat3
{
    friend class Transform;
    friend mat3 operator*(const mat3& op1, const mat3& op2);

    float getCofactor(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8) const;
public:

    static const mat3 identity;
    static const mat3 zero;

    explicit mat3(const float _xx, const float _xy, const float _xz,
                  const float _yx, const float _yy, const float _yz,
                  const float _zx, const float _zy, const float _zz);
    mat3(float d = 1.f);

    using cref = const mat3 &;
    union
    {
        float data[9];
        struct
        {
            float xx, xy, xz;
            float yx, yy, yz;
            float zx, zy, zz;
        };
        struct
        {
            float m00, m10, m20;
            float m01, m11, m21;
            float m02, m12, m22;
        };
        vec3 v[3];
    };
    void transpose();
    float determinant() const;
    mat3 invert();
    void setZero();
    void setIdentity();

    inline vec3& operator[](const size_t pos);
    inline const vec3& operator[](const size_t pos) const;
    inline bool compare(const mat3 &a) const;
    bool operator==(const mat3 &a ) const;
    bool operator!=(const mat3 &a ) const;

    vec3 operator*(const vec3& rhs) const;
    mat3& operator*=(float rhs);

    mat3& operator+=(const mat3& op);
    mat3 operator-(const mat3& op) const;

    bool setInvert(const mat3& rhs);
    mat3& setTranspose(const mat3& m);

    Rotator getRotator() const;
    vec3 transposeMulV3(const vec3& rhs);

//    mat3 operator^(const vec3& vec) const;

    friend std::ostream& operator<<(std::ostream& o, const mat3& a)  // NB: testing only
    {
        o << "{\n\t" <<
          a.xx << ", " << a.xy << ", "<< a.xz << "\n\t" <<
          a.yx << ", " << a.yy << ", "<< a.yz << "\n\t" <<
          a.zx << ", " << a.zy << ", "<< a.zz << "\n}";
        return o;
    }
};

class mat4
{

    friend class Transform;

    friend mat4 operator*(const mat4& op1, const mat4& op2);
    friend mat4 makeOrthoProjectionMatrix(float left, float right, float bottom, float top, float near, float far);
    friend mat4 makePerspectiveProjectionMatrix(float fov, float aspect, float near, float far);

    float getCofactor(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8) const;
public:

    static const mat4 identity;
    static const mat4 zero;

    explicit mat4(const float _xx, const float _xy, const float _xz, const float _xw,
                  const float _yx, const float _yy, const float _yz, const float _yw,
                  const float _zx, const float _zy, const float _zz, const float _zw,
                  const float _wx, const float _wy, const float _wz, const float _ww);

    explicit mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3);
    explicit mat4(mat3::cref rh);
    mat4();

    using cref = const mat4&;
    union
    {
        float data[16];
        struct
        {
            float xx, xy, xz, xw;
            float yx, yy, yz, yw;
            float zx, zy, zz, zw;
            float wx, wy, wz, ww;
        };
        struct
        {
            float m00, m10, m20, m30;
            float m01, m11, m21, m31;
            float m02, m12, m22, m32;
            float m03, m13, m23, m33;
        };
        vec4 v[4];
    };
    void transpose();
    Transform decompose() const;
    float determinant() const;
    mat4 invert() const;
    mat3 getRotationMatrix() const;

    inline vec4& operator[](const size_t pos);
    inline const vec4& operator[](const size_t pos) const;
    inline bool compare(const mat4& a) const;
    bool operator==(const mat4& a) const;
    bool operator!=(const mat4& a) const;

    vec4 operator*(const vec4& rhs) const;
    Rotator operator*(const Rotator& rhs) const;

    mat4 operator-(cref a) const;
    mat4 operator+(cref a) const;
    mat4 operator*(float a) const;
    mat4 operator/(float a) const;

    mat4& operator+=(cref a);
    mat4& operator-=(cref a);
    mat4& operator*=(float a);
    mat4& operator/=(float a);

    friend std::ostream& operator<<(std::ostream& o, const mat4& a)  // NB: testing only
    {
        o << "{\n\t" <<
          a.xx << ", " << a.xy << ", " << a.xz << ", " << a.xw << "\n\t" <<
          a.yx << ", " << a.yy << ", " << a.yz << ", " << a.yw << "\n\t" <<
          a.zx << ", " << a.zy << ", " << a.zz << ", " << a.zw << "\n\t" <<
          a.wx << ", " << a.wy << ", " << a.wz << ", " << a.ww << "\n}";
        return o;
    }
};

mat4 operator*(const mat4& op1, const mat4& op2);
mat4 operator*(float op1, const mat4& op2);

template<typename T>
T lerp(const T& a, const T& b, float t);

template<typename T>
T hermit(const T& a, const T& b, const T& ak, const T& bk, float t);

template<typename T>
T cubic(const T &a, const T &b, const T &c, const T &d, float t);

Rotator lerp(const Rotator& a, const Rotator& b, float t);

mat4 makeOrthoProjectionMatrix(float left, float right, float bottom, float top, float near, float far);
mat4 makePerspectiveProjectionMatrix(float fov, float aspect, float near, float far);
mat4 lookAt(vec3 const& eye, vec3 const& center, vec3 const& up);

std::optional<vec2> intersect2d(const vec2& startPos1, const vec2& endPos1, const vec2& startPos2, const vec2& endPos2);

struct rect
{
    vec2 position;
    vec2 size;
    rect() = default;
    rect(vec2 position, vec2 size): position(position), size(size) {}
    rect(float x, float y, float w, float h): position(x, y), size(w, h) { }
};

class Ray
{
public:
    using cref = const Ray&;

    Ray() = default;
    Ray(const vec3& origin, const vec3& direction);

    vec3 origin;
    vec3 direction;
    float length = 0.f;

    Ray toLocal(Transform::cref) const;
};

class Plain
{
private:
    vec3 m_point;
    vec3 m_normal;
public:
    Plain() = default;
    Plain(const vec3& point, const vec3& normal);
    Plain(const vec3& pointA, const vec3& pointB, const vec3& pointC);
    vec3 vectorIntersect(const vec3& vecStart, const vec3& vecDirection) const;
    float pointDistance(const vec3& point) const;
    bool isPointAbove(const vec3& point) const;

    vec3::cref point() const{return m_point;}
    vec3::cref normal() const{return m_normal;}
};

struct Frustum
{
    Frustum() = default;
    Frustum(const std::array<math::vec3, 4>& near, const std::array<math::vec3, 4>& far);
    void normalize();
    std::array<math::vec4, 6> planes;
    std::array<math::vec3, 8> points;
};

class color: public vec4
{
public:

    color(): vec4(0, 0, 0, 1) {}
    color(float r, float g, float b, float a): vec4(r, g, b, a) {}
    color(const vec4& op): vec4(op) {}
    color(const vec3& op): vec4(op, 1.f) {}
    color(uint32_t h);

    float grayscale() const;
    vec3 rgb() const;
    vec3 hsl() const;
    color lerpRgb(const color& op, float pos) const;
    color lerpHsl(const color& op, float pos) const;

    uint32_t hexRgba() const;
    uint32_t hexArgb() const;
    uint32_t hexAbgr() const;
    uint32_t hexBgra() const;
    uint32_t hexRgb() const;
    uint32_t hexBgr() const;

    static color random(bool alpha=false);
    static color hslToRgb(const vec3& hsl, float alpha = 1.f);
    static color hexRgbaToRgb(uint32_t h);
    static color hexArgbToRgb(uint32_t h);
    static color hexAbgrToRgb(uint32_t h);
    static color hexBgraToRgb(uint32_t h);
    static color hexRgbToRgb(uint32_t h);
    static color hexBgrToRgb(uint32_t h);

    enum name
    {
        IndianRed  = 0xCD5C5CFF,
        LightCoral = 0xF08080FF,
        Salmon	   = 0xFA8072FF,
        DarkSalmon = 0xE9967AFF,
        Crimson     = 0xDC143CFF,
        Red	        = 0xFF0000FF,
        FireBrick	= 0xB22222FF,
        DarkRed	    = 0x8B0000FF,

        Pink	    = 0xFFC0CBFF,
        LightPink	= 0xFFB6C1FF,
        HotPink	    = 0xFF69B4FF,
        DeepPink	    = 0xFF1493FF,
        MediumVioletRed	= 0xC71585FF,
        PaleVioletRed	= 0xDB7093FF,

        LightSalmon	= 0xFFA07AFF,
        Coral	= 0xFF7F50FF,
        Tomato	= 0xFF6347FF,
        OrangeRed	= 0xFF4500FF,
        DarkOrange	= 0xFF8C00FF,
        Orange	= 0xFFA500FF,

        Gold	             = 0xFFD700FF,
        Yellow	             = 0xFFFF00FF,
        LightYellow	         = 0xFFFFE0FF,
        LemonChiffron	     = 0xFFFACDFF,
        LightGoldenrodYellow = 0xFAFAD2FF,
        PapayaWhip	         = 0xFFEFD5FF,
        Moccasin	         = 0xFFE4B5FF,
        PeachPuff	         = 0xFFDAB9FF,
        PaleGoldenrod	     = 0xEEE8AAFF,
        Khaki	             = 0xF0E68CFF,
        DarkKhaki	         = 0xBDB76BFF,

        Lavender	= 0xE6E6FAFF,
        Thistle	    = 0xD8BFD8FF,
        Plum	    = 0xDDA0DDFF,
        Violet	    = 0xEE82EEFF,
        Orchid	    = 0xDA70D6FF,
        Fuchsia	    = 0xFF00FFFF,
        Magenta	    = 0xFF00FFFF,
        MediumOrchid	= 0xBA55D3FF,
        MediumPurple	= 0x9370DBFF,
        BlueViolet	= 0x8A2BE2FF,
        DarkViolet	= 0x9400D3FF,
        DarkOrchid	= 0x9932CCFF,
        DarkMagenta	= 0x8B008BFF,
        Purple	    = 0x800080FF,
        RebeccaPurple = 0x663399FF,
        Indigo	      = 0x4B0082FF,
        SlateBlue	    = 0x6A5ACDFF,
        DarkSlateBlue	= 0x483D8BFF,
        MediumSlateBlue	= 0x7B68EEFF,

        GreenYellow	= 0xADFF2FFF,
        Chartreuse	= 0x7FFF00FF,
        LawnGreen	= 0x7CFC00FF,
        Lime	    = 0x00FF00FF,
        LimeGreen	= 0x32CD32FF,
        PaleGreen	= 0x98FB98FF,
        LightGreen	= 0x90EE90FF,
        MediumSpringGreen = 0x00FA9AFF,
        SpringGreen	      = 0x00FF7FFF,
        MediumSeaGreen	  = 0x3CB371FF,
        SeaGreen	= 0x2E8B57FF,
        ForestGreen	= 0x228B22FF,
        Green	    = 0x008000FF,
        DarkGreen	= 0x006400FF,
        YellowGreen	= 0x9ACD32FF,
        OliveDrab	= 0x6B8E23FF,
        Olive	    = 0x808000FF,
        DarkOliveGreen	 = 0x556B2FFF,
        MediumAquaMarine = 0x66CDAAFF,
        DarkSeaGreen	 = 0x8FBC8FFF,
        LightSeaGreen	 = 0x20B2AAFF,
        DarkCyan	     = 0x008B8BFF,
        Teal	         = 0x008080FF,

        Aqua	= 0x00FFFFFF,
        Cyan	= 0x00FFFFFF,
        LightCyan	= 0xE0FFFFFF,
        PaleTurquoise	= 0xAFEEEEFF,
        AquaMarine	= 0x7FFFD4FF,
        Turquoise	= 0x40E0D0FF,
        MediumTurquoise	= 0x48D1CCFF,
        DarkTurquoise	= 0x00CED1FF,
        CadetBlue	= 0x5F9EA0FF,
        SteelBlue	= 0x4682B4FF,
        LightSteelBlue	= 0xB0C4DEFF,
        PowderBlue	= 0xB0E0E6FF,
        LightBlue	= 0xADD8E6FF,
        SkyBlue	    = 0x87CEEBFF,
        LightSkyBlue	= 0x87CEFAFF,
        DeepSkyBlue	= 0x00BFFFFF,
        DodgerBlue	= 0x1E90FFFF,
        CornflowerBlue	= 0x6495EDFF,
        RoyalBlue	= 0x4169E1FF,
        Blue	    = 0x0000FFFF,
        MediumBlue	= 0x0000CDFF,
        DarkBlue	= 0x00008BFF,
        Navy	    = 0x000080FF,
        MidnightBlue = 0x191970FF,

        Cornsilk	 = 0xFFF8DCFF,
        BlanchedAlmond	= 0xFFEBCDFF,
        Bisque	        = 0xFFE4C4FF,
        NavajoWhite	    = 0xFFDEADFF,
        Wheat	    = 0xF5DEBFF,
        BurlyWood	= 0xDEB887FF,
        Tan	        = 0xD2B48CFF,
        RosyBrown	= 0xBC8F8FFF,
        SandyBrown	= 0xF4A460FF,
        Goldenrod	= 0xDAA520FF,
        DarkGoldenrod	= 0xB8860BFF,
        Peru	        = 0xCD853FFF,
        Chocolate	    = 0xD2691EFF,
        SaddleBrown	    = 0x8B4513FF,
        Sienna	= 0xA0522DFF,
        Brown	= 0xA52A2AFF,
        Maroon	= 0x800000FF,

        White	= 0xFFFFFFFF,
        Snow	= 0xFFFAFAFF,
        Honeydew	= 0xF0FFF0FF,
        MintCream	= 0xF5FFFAFF,
        Azure	= 0xF0FFFFFF,
        AliceBlue	= 0xF0F8FFFF,
        GhostWhite	= 0xF8F8FFFF,
        WhiteSmoke	= 0xF5F5F5FF,
        SeaShell	= 0xFFF5EEFF,
        Beige	= 0xF5F5DCFF,
        OldLace	= 0xFDF5E6FF,
        FloralWhite	= 0xFFFAF0FF,
        Ivory	= 0xFFFFF0FF,
        AntiqueWhite	= 0xFAEBD7FF,
        Linen	= 0xFAF0E6FF,
        LavenderBlush	= 0xFFF0F5FF,
        MistyRose	= 0xFFE4E1FF,

        Gainsboro	= 0xDCDCDCFF,
        LightGray	= 0xD3D3D3FF,
        Silver      = 0xC0C0C0FF,
        DarkGray	= 0xA9A9A9FF,
        Gray        = 0x808080FF,
        DimGray	    = 0x696969FF,
        LightSlateGray	= 0x778899FF,
        SlateGray	    = 0x708090FF,
        DarkSlateGray	= 0x2F4F4FFF,
        Black = 0x000000FF,

        Transparent = 0x00000000
    };
};



template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
equals(const T& x, const T& y, int ulp = 2);

template <typename T> inline constexpr
int signum(T x, std::false_type is_signed);

template <typename T> inline constexpr
int signum(T x, std::true_type is_signed);

template <typename T> inline constexpr
int signum(T x);

inline std::mt19937 & initRandomGenerator(uint seed)
{
    static std::mt19937 gen(seed);
    return gen;
}

inline std::mt19937& getRandomGenerator()
{
    static std::mt19937 & gen = initRandomGenerator(static_cast<uint>(time(0)));
    return gen;
}

template<typename T>
auto random(T from, T to) -> decltype(typename std::enable_if<std::is_integral_v<T>>::type(), T())
{
    std::uniform_int_distribution<> dis(from, to);
    return dis(getRandomGenerator());
}

template<typename T>
auto random(T from, T to) -> decltype(typename std::enable_if<std::is_floating_point_v<T>>::type(), T())
{
    std::uniform_real_distribution<> dis(from, to);
    return static_cast<T>(dis(getRandomGenerator()));
}

template<typename T>
auto random() -> decltype(typename std::enable_if<std::is_integral_v<T>>::type(), T())
{
    return random<T>(0, std::numeric_limits<T>::max());
}

template<typename T>
auto random() -> decltype(typename std::enable_if<std::is_floating_point_v<T>>::type(), T())
{
    return random<T>(static_cast<T>(0.0f), static_cast<T>(1.0f));
}

}

#include <string>

namespace std
{
    w4::math::vec3 min(const w4::math::vec3& a, const w4::math::vec3& b);
    w4::math::vec3 max(const w4::math::vec3& a, const w4::math::vec3& b);
    template<>
    void swap<w4::math::vec3>(w4::math::vec3& a, w4::math::vec3& b) noexcept;

    string to_string(const w4::math::mat4& m);
}



#include "impl/W4Math.inl"
