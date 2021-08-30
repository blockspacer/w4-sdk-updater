#pragma once

#include <cstdint>
#include "impl/W4MathSwizzze.inl"

namespace w4::math {

struct ivec2
{
    using value_type = int32_t;
    using cref = const ivec2&;

    union
    {
        struct
        {
            union { int32_t x, u, s, w; };
            union { int32_t y, v, t, h; };
        };
        SWIZZLE2_2(2, ivec2)
        int32_t elements[2];
    };

    int32_t operator[](size_t i) const {return elements[i];}
    int32_t& operator[](size_t i) {return elements[i];}

    inline bool operator<(const ivec2& op) const {return (x < op.x || (x == op.x && y < op.y));};

    bool operator==(const ivec2& op) const { return x == op.x && y == op.y;}
    bool operator!=(const ivec2& op) const { return x != op.x || y != op.y;}

    ivec2(): x(0), y(0) {}
    constexpr ivec2(int32_t _x, int32_t _y): x(_x), y(_y) {}
    explicit constexpr ivec2(int32_t v): ivec2(v, v) {}
};



struct ivec3
{
    using value_type = int32_t;
    using cref = const ivec3&;
    union
    {
        struct
        {
            union { int32_t x, r, s; };
            union { int32_t y, g, t; };
            union { int32_t z, b, p; };
        };
        SWIZZLE2_2(3, ivec2)
        SWIZZLE2_3(3, ivec2)
        SWIZZLE3_3(3, ivec3)
        int32_t elements[3] = {0};
    };

    int32_t operator[](size_t i) const {return elements[i];}
    int32_t& operator[](size_t i) {return elements[i];}

    constexpr ivec3(): x(0), y(0), z(0) {}
    constexpr ivec3(int32_t _x, int32_t _y, int32_t _z): x(_x), y(_y), z(_z) {}
    constexpr ivec3(int32_t _x, const ivec2& _yz): ivec3(_x, _yz.x, _yz.y) {}
    constexpr ivec3(const ivec2& _xy, int32_t _z): ivec3(_xy.x, _xy.y, _z) {}
    explicit constexpr ivec3(int32_t v): ivec3(v, v, v) {}
};

struct ivec4
{
    using value_type = int32_t;
    using cref = const ivec4&;
    union
    {
        struct
        {
            union { int32_t x, r, s; };
            union { int32_t y, g, t; };
            union { int32_t z, b, p; };
            union { int32_t w, a, q; };
        };
        SWIZZLE2_2(4, ivec2)
        SWIZZLE2_3(4, ivec2)
        SWIZZLE2_4(4, ivec2)
        SWIZZLE3_3(4, ivec3)
        SWIZZLE3_4(4, ivec3)
        SWIZZLE4(4, ivec4)
        int32_t elements[4];
    };

    int32_t operator[](size_t i) const {return elements[i];}
    int32_t& operator[](size_t i) {return elements[i];}

    constexpr ivec4(): x(0), y(0), z(0), w(0) {}
    constexpr ivec4(int32_t _x, int32_t _y, int32_t _z, int32_t _w): x(_x), y(_y), z(_z), w(_w) {}
    constexpr ivec4(int32_t _x, int32_t _y, ivec2 _zw): ivec4(_x, _y, _zw.x, _zw.y) {}
    constexpr ivec4(int32_t _x, ivec2 _yz, int32_t _w): ivec4(_x, _yz.x, _yz.y, _w) {}
    constexpr ivec4(ivec2 _xy, int32_t _z, int32_t _w): ivec4(_xy.x, _xy.y, _z, _w) {}
    constexpr ivec4(ivec2 _xy, ivec2 _zw): ivec4(_xy.x, _xy.y, _zw.x, _zw.y) {}
    constexpr ivec4(int32_t _x, const ivec3& _zyw): ivec4(_x, _zyw.x, _zyw.y, _zyw.z) {}
    constexpr ivec4(const ivec3& _xyz, int32_t _w): ivec4(_xyz.x, _xyz.y, _xyz.z, _w) {}
    explicit constexpr ivec4(int32_t v): ivec4(v, v, v, v) {}
};

template<typename T>
static constexpr bool is_ivec() { return std::is_same_v<T, ivec2> || std::is_same_v<T, ivec3> || std::is_same_v<T, ivec4>; }

template<class T, class E = std::enable_if_t<is_ivec<T>()>>
T operator+(const T& first, const T& second)
{
    T res;
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        res[i] = first[i] + second[i];
    }
    return res;
}

template<class T, class E = std::enable_if_t<is_ivec<T>()>>
T operator-(const T& first, const T& second)
{
    T res;
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        res[i] = first[i] - second[i];
    }
    return res;
}

template<class T, class E = std::enable_if_t<is_ivec<T>()>>
T& operator+=(T& first, const T& second)
{
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        first[i] += second[i];
    }
    return first;
}

template<class T, class E = std::enable_if_t<is_ivec<T>()>>
T& operator-=(T& first, const T& second)
{
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        first[i] -= second[i];
    }
    return first;
}

template<class T, class O, class E = std::enable_if_t<is_ivec<T>() && std::is_arithmetic_v<O>>>
T& operator+=(T& first, E second)
{
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        first[i] += second;
    }
    return first;
}

template<class T, class O, class E = std::enable_if_t<is_ivec<T>() && std::is_arithmetic_v<O>>>
T& operator-=(T& first, E second)
{
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        first[i] -= second;
    }
    return first;
}

template<class T, class O, class E = std::enable_if_t<is_ivec<T>() && std::is_arithmetic_v<O>>>
T& operator*=(T& first, E second)
{
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        first[i] *= second;
    }
    return first;
}

template<class T, class O, class E = std::enable_if_t<is_ivec<T>() && std::is_arithmetic_v<O>>>
T& operator/=(T& first, E second)
{
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        first[i] /= second;
    }
    return first;
}

template<class T, class O, class E = std::enable_if_t<is_ivec<T>() && std::is_arithmetic_v<O>>>
T operator+(const T& first, E second)
{
    T res;
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        res[i] = first[i] + second;
    }
    return res;
}

template<class T, class O, class E = std::enable_if_t<is_ivec<T>() && std::is_arithmetic_v<O>>>
T operator-(const T& first, E second)
{
    T res;
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        res[i] = first[i] - second;
    }
    return res;
}

template<class T, class O, class E = std::enable_if_t<is_ivec<T>() && std::is_arithmetic_v<O>>>
T operator*(const T& first, E second)
{
    T res;
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        res[i] = first[i] * second;
    }
    return res;
}

template<class T, class O, class E = std::enable_if_t<is_ivec<T>() && std::is_arithmetic_v<O>>>
T operator/(const T& first, E second)
{
    T res;
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        res[i] = first[i] / second;
    }
    return res;
}

template<class T, class E = std::enable_if_t<is_ivec<T>()>>
bool operator==(const T& first, const T& second)
{
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        if(first[i] != second[i])
        {
            return false;
        }
    }
    return true;
}

template<class T, class E = std::enable_if_t<is_ivec<T>()>>
bool operator<(const T& first, const T& second)
{
    constexpr auto sz = (sizeof(T::elements) / sizeof(T::elements[0]));
    for(size_t i = 0; i < sz; ++i)
    {
        if(first[i] >= second[i])
        {
            return false;
        }
    }
    return true;
}

template<class T, class E = std::enable_if_t<is_ivec<T>()>>
bool operator>(const T& first, const T& second)
{
    return second < first;
}

template<class T, class E = std::enable_if_t<is_ivec<T>()>>
bool operator<=(const T& first, const T& second)
{
    return !(first > second);
}

template<class T, class E = std::enable_if_t<is_ivec<T>()>>
bool operator>=(const T& first, const T& second)
{
    return !(first < second);
}

} //namespace w4::math

namespace std
{
    template <>
    struct hash<w4::math::ivec2>
    {
        std::size_t operator()(const w4::math::ivec2& p) const
        {
            auto h1 = std::hash<w4::math::ivec2::value_type>{}(p.x);
            auto h2 = std::hash<w4::math::ivec2::value_type>{}(p.y);

            return h1 ^ h2;
        }
    };
} // std
