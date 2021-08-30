namespace w4::math {

inline vec3& mat3::operator[](const size_t pos)
{
    return v[pos];
}

inline const vec3& mat3::operator[](const size_t pos) const
{
    return v[pos];
}

inline bool mat3::compare(const mat3 &a) const
{
    const float *p1 = data;
    const float *p2 = a.data;
    for (int  i = 0; i < 3*3; i++ )
        if (::fabs(p1[i] - p2[i]) > EPSILON) return false;
    return true;
}
inline bool mat3::operator==(const mat3 &a ) const
{
    return compare(a);
}

inline bool mat3::operator!=(const mat3 &a ) const
{
    return !compare(a);
}

inline vec4& mat4::operator[](const size_t pos)
{
    return v[pos];
}

inline const vec4& mat4::operator[](const size_t pos) const
{
    return v[pos];
}

inline bool mat4::compare(const mat4& a) const
{
    const float* p1 = data;
    const float* p2 = a.data;
    for(int i = 0; i < 4 * 4; i++)
        if(::fabs(p1[i] - p2[i]) > EPSILON) return false;
    return true;
}

inline bool mat4::operator==(const mat4& a) const
{
    return compare(a);
}

inline bool mat4::operator!=(const mat4& a) const
{
    return !compare(a);
}

template<typename T>
T lerp(const T& a, const T& b, float t)
{
    return a + t * (b - a);
}

template<typename T>
T hermit(const T& a, const T& b, const T& ak, const T& bk, float t)
{
    auto t2 = t * t;
    auto t3 = t * t * t;

    auto f1 = 2 * t3 - 3 * t2 + 1;
    auto f2 = -2 * t3 + 3 * t2;
    auto f3 = t3 - 2 * t2 + t;
    auto f4 = t3 - t2;

    return f1 * a + f2 * b + f3 * ak + f4 * bk;
}

template<typename T>
T cubic(const T &a, const T &b, const T &c, const T &d, float t)
{
    auto t2 = t * t;
    auto a0 = d - c - a + b;
    auto a1 = a - b - a0;
    auto a2 = c - a;
    auto a3 = b;
    return a0 * t * t2 + a1 * t2 + a2 * t + a3;
}

inline vec3 mat3::operator*(const vec3& rhs) const
{
    return vec3(data[0]*rhs.x + data[3]*rhs.y + data[6]*rhs.z,
                data[1]*rhs.x + data[4]*rhs.y + data[7]*rhs.z,
                data[2]*rhs.x + data[5]*rhs.y + data[8]*rhs.z);
}

inline vec4 mat4::operator*(const vec4& rhs) const
{
    return vec4(data[0] * rhs.x + data[4] * rhs.y + data[8] * rhs.z + data[12] * rhs.w,
                data[1] * rhs.x + data[5] * rhs.y + data[9] * rhs.z + data[13] * rhs.w,
                data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z + data[14] * rhs.w,
                data[3] * rhs.x + data[7] * rhs.y + data[11] * rhs.z + data[15] * rhs.w);
}

inline Rotator mat4::operator*(const Rotator& rhs) const
{
    return Rotator(data[0] * rhs.quaternion.x + data[4]*rhs.quaternion.y + data[8]*rhs.quaternion.z  + data[12]*rhs.quaternion.w,
                   data[1] * rhs.quaternion.x + data[5]*rhs.quaternion.y + data[9]*rhs.quaternion.z  + data[13]*rhs.quaternion.w,
                   data[2] * rhs.quaternion.x + data[6]*rhs.quaternion.y + data[10]*rhs.quaternion.z + data[14]*rhs.quaternion.w,
                   data[3] * rhs.quaternion.x + data[7]*rhs.quaternion.y + data[11]*rhs.quaternion.z + data[15]*rhs.quaternion.w);
}

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
equals(const T &x, const T &y, int ulp) {
    return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
           || std::fabs(x - y) < std::numeric_limits<T>::min();
}

template <typename T> inline constexpr
int signum(T x, std::false_type is_signed)
{
    return T(0) < x;
}

template <typename T> inline constexpr
int signum(T x, std::true_type is_signed)
{
    return (T(0) < x) - (x < T(0));
}

template <typename T> inline constexpr
int signum(T x)
{
    return signum(x, std::is_signed<T>());
}

}

namespace std {

template<> struct hash<w4::math::vec2>
{
    std::size_t operator()(const w4::math::vec2& v) const noexcept
    {
        size_t h1 = std::hash<float>{}(v.x);
        size_t h2 = std::hash<float>{}(v.y);
        return h1 ^ (h2 << 1);
    }
};

template<> struct hash<w4::math::vec3>
{
    size_t operator()(w4::math::vec3::cref v) const noexcept
    {
       // std::hash<std::pair<float, float>> h;
        std::hash<float> hasher;
        return hasher(v.x) ^ hasher(v.y * v.y) ^ hasher(v.z * v.z * v.z);
    }
};

}
