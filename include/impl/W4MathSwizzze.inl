#pragma once

template<size_t DataSize, typename T, size_t A, size_t B>
struct vec2Swizzle
{
    typename T::value_type m_data[DataSize];
    T operator=(const T& vec)
    {
        return T(m_data[A] = vec.x, m_data[B] = vec.y);
    }
    operator T() const
    {
        return T(m_data[A], m_data[B]);
    }
};

template<size_t DataSize, typename T, size_t A, size_t B, size_t C>
struct vec3Swizzle
{
    typename T::value_type m_data[DataSize];
    T operator=(const T& vec)
    {
        return T(m_data[A] = vec.x, m_data[B] = vec.y, m_data[C] = vec.z);
    }
    operator T() const
    {
        return T(m_data[A], m_data[B], m_data[C]);
    }
};


template<size_t DataSize, typename T, size_t A, size_t B, size_t C, size_t D>
struct vec4Swizzle
{
    typename T::value_type m_data[DataSize];
    T operator=(const T& vec)
    {
        return T(m_data[A] = vec.x, m_data[B] = vec.y, m_data[C] = vec.z, m_data[D] = vec.w);
    }
    operator T() const
    {
        return T(m_data[A], m_data[B], m_data[C], m_data[D]);
    }
};

#define SWIZZLE2_2(S, T) \
union { vec2Swizzle<S, T, 0, 0> xx, uu, ss; }; \
union { vec2Swizzle<S, T, 1, 1> yy, vv, tt; }; \
union { vec2Swizzle<S, T, 0, 1> xy, uv, st; }; \
union { vec2Swizzle<S, T, 1, 0> yx, vu, ts; };

#define SWIZZLE2_3(S, T) \
union { vec2Swizzle<S, T, 2, 2> zz, bb, pp; }; \
union { vec2Swizzle<S, T, 0, 2> xz, rb, sp; }; \
union { vec2Swizzle<S, T, 1, 2> yz, gb, tp; }; \
union { vec2Swizzle<S, T, 2, 0> zx, br, ps; }; \
union { vec2Swizzle<S, T, 2, 1> zy, bg, pt; };

#define SWIZZLE2_4(S, T) \
union { vec2Swizzle<S, T, 3, 0> wx, ar, qs; }; \
union { vec2Swizzle<S, T, 3, 1> wy, ag, qt; }; \
union { vec2Swizzle<S, T, 3, 2> wz, ab, qp; }; \
union { vec2Swizzle<S, T, 0, 3> xw, ra, sq; }; \
union { vec2Swizzle<S, T, 1, 3> yw, ga, tq; }; \
union { vec2Swizzle<S, T, 2, 3> zw, ba, pq; }; \
union { vec2Swizzle<S, T, 3, 3> ww, aa, qq; };

#define SWIZZLE3_3(S, T) \
union { vec3Swizzle<3, T, 0, 1, 2> xyz, rgb, stp; }; \
union { vec3Swizzle<3, T, 0, 2, 1> xzy, rbg, spt; }; \
union { vec3Swizzle<3, T, 1, 0, 2> yxz, grb, tsp; }; \
union { vec3Swizzle<3, T, 1, 2, 0> yzx, gbr, tps; }; \
union { vec3Swizzle<3, T, 2, 0, 1> zxy, brg, pst; }; \
union { vec3Swizzle<3, T, 2, 1, 0> zyx, bgr, pts; }; \
\
union { vec3Swizzle<3, T, 0, 0, 0> xxx, rrr, sss; }; \
union { vec3Swizzle<3, T, 0, 0, 1> xxy, rrg, sst; }; \
union { vec3Swizzle<3, T, 0, 0, 2> xxz, rrb, ssp; }; \
union { vec3Swizzle<3, T, 0, 1, 0> xyx, rgr, sps; }; \
union { vec3Swizzle<3, T, 0, 2, 0> xzx, rbr, sts; }; \
union { vec3Swizzle<3, T, 1, 0, 0> yxx, grr, tss; }; \
union { vec3Swizzle<3, T, 2, 0, 0> zxx, brr, pss; }; \
\
union { vec3Swizzle<3, T, 1, 1, 1> yyy, ggg, ttt; }; \
union { vec3Swizzle<3, T, 1, 1, 0> yyx, ggr, tts; }; \
union { vec3Swizzle<3, T, 1, 1, 2> yyz, ggb, ttp; }; \
union { vec3Swizzle<3, T, 1, 0, 1> yxy, grg, tst; }; \
union { vec3Swizzle<3, T, 1, 2, 1> yzy, gbg, tpt; }; \
union { vec3Swizzle<3, T, 0, 1, 1> xyy, rgg, stt; }; \
union { vec3Swizzle<3, T, 2, 1, 1> zyy, bgg, ptt; }; \
\
union { vec3Swizzle<3, T, 2, 2, 2> zzz, bbb, ppp; }; \
union { vec3Swizzle<3, T, 2, 2, 0> zzx, bbr, pps; }; \
union { vec3Swizzle<3, T, 2, 2, 1> zzy, bbg, ppt; }; \
union { vec3Swizzle<3, T, 2, 0, 2> zxz, brb, psp; }; \
union { vec3Swizzle<3, T, 2, 1, 2> zyz, bgb, ptp; }; \
union { vec3Swizzle<3, T, 0, 2, 2> xzz, rbb, spp; }; \
union { vec3Swizzle<3, T, 1, 2, 2> yzz, gbb, tpp; };

#define SWIZZLE3_4(S, T) \
union { vec3Swizzle<3, T, 3, 0, 1> wxy, arg, qst; }; \
union { vec3Swizzle<3, T, 3, 0, 2> wxz, arb, qsp; }; \
union { vec3Swizzle<3, T, 3, 1, 0> wyx, agr, qts; }; \
union { vec3Swizzle<3, T, 3, 1, 2> wyz, agb, qtp; }; \
union { vec3Swizzle<3, T, 3, 2, 0> wzx, abr, qps; }; \
union { vec3Swizzle<3, T, 3, 2, 1> wzy, abg, qpt; }; \
\
union { vec3Swizzle<3, T, 3, 3, 3> www, aaa, qqq; }; \
union { vec3Swizzle<3, T, 3, 3, 0> wwx, aar, qqs; }; \
union { vec3Swizzle<3, T, 3, 3, 1> wwy, aag, qqt; }; \
union { vec3Swizzle<3, T, 3, 3, 2> wwz, aab, qqp; }; \
union { vec3Swizzle<3, T, 3, 0, 3> wxw, ara, qsq; }; \
union { vec3Swizzle<3, T, 3, 1, 3> wyw, aga, qtq; }; \
union { vec3Swizzle<3, T, 3, 2, 3> wzw, aba, qpq; }; \
union { vec3Swizzle<3, T, 0, 3, 3> xww, raa, sqq; }; \
union { vec3Swizzle<3, T, 1, 3, 3> yww, gaa, tqq; }; \
union { vec3Swizzle<3, T, 2, 3, 3> zww, baa, pqq; }; \
\
union { vec3Swizzle<3, T, 3, 0, 0> wxx, arr, qss; }; \
union { vec3Swizzle<3, T, 3, 1, 1> wyy, agg, qtt; }; \
union { vec3Swizzle<3, T, 3, 2, 2> wzz, abb, qpp; }; \
union { vec3Swizzle<3, T, 0, 3, 0> xwx, rar, sqs; }; \
union { vec3Swizzle<3, T, 1, 3, 1> ywy, gag, tqt; }; \
union { vec3Swizzle<3, T, 2, 3, 2> zwz, bab, pqp; }; \
union { vec3Swizzle<3, T, 0, 0, 3> xxw, rra, ssq; }; \
union { vec3Swizzle<3, T, 1, 1, 3> yyw, gga, ttq; }; \
union { vec3Swizzle<3, T, 2, 2, 3> zzw, bba, ppq; };

#define SWIZZLE4(S, T) \
union { vec4Swizzle<4, T, 0, 1, 2, 3> xyzw, rgba, stpq; }; \
union { vec4Swizzle<4, T, 0, 1, 3, 2> xywz, rgab, stqp; }; \
union { vec4Swizzle<4, T, 0, 2, 1, 3> xzyw, rbga, sptq; }; \
union { vec4Swizzle<4, T, 0, 2, 3, 1> xzwy, rbag, spqt; }; \
union { vec4Swizzle<4, T, 0, 3, 1, 2> xwyz, ragb, sqtp; }; \
union { vec4Swizzle<4, T, 0, 3, 2, 1> xwzy, rabg, sqpt; }; \
\
union { vec4Swizzle<4, T, 1, 0, 2, 3> yxzw, grba, tspq; }; \
union { vec4Swizzle<4, T, 1, 0, 3, 2> yxwz, grab, tsqp; }; \
union { vec4Swizzle<4, T, 1, 2, 0, 3> yzxw, gbra, tpsq; }; \
union { vec4Swizzle<4, T, 1, 2, 3, 0> yzwx, gbar, tpqs; }; \
union { vec4Swizzle<4, T, 1, 3, 0, 2> ywxz, garb, tqsp; }; \
union { vec4Swizzle<4, T, 1, 3, 2, 0> ywzx, gabr, tqps; }; \
\
union { vec4Swizzle<4, T, 2, 0, 1, 3> zxyw, brga, pstq; }; \
union { vec4Swizzle<4, T, 2, 0, 3, 1> zxwy, brag, psqt; }; \
union { vec4Swizzle<4, T, 2, 1, 0, 3> zyxw, bgra, ptsq; }; \
union { vec4Swizzle<4, T, 2, 1, 3, 0> zywx, bgar, ptqs; }; \
union { vec4Swizzle<4, T, 2, 3, 0, 1> zwxy, barg, pqst; }; \
union { vec4Swizzle<4, T, 2, 3, 1, 0> zwyx, bagr, pqts; }; \
\
union { vec4Swizzle<4, T, 3, 0, 1, 2> wxyz, argb, qstp; }; \
union { vec4Swizzle<4, T, 3, 0, 2, 1> wxzy, arbg, qspt; }; \
union { vec4Swizzle<4, T, 3, 1, 0, 2> wyxz, agrb, qtsp; }; \
union { vec4Swizzle<4, T, 3, 1, 2, 0> wyzx, agbr, qtps; }; \
union { vec4Swizzle<4, T, 3, 2, 0, 1> wzxy, abrg, qpst; }; \
union { vec4Swizzle<4, T, 3, 2, 1, 0> wzyx, abgr, qpts; }; \
\
union { vec4Swizzle<4, T, 0, 0, 0, 0> xxxx, rrrr, ssss; }; \
union { vec4Swizzle<4, T, 1, 1, 1, 1> yyyy, gggg, tttt; }; \
union { vec4Swizzle<4, T, 2, 2, 2, 2> zzzz, bbbb, pppp; }; \
union { vec4Swizzle<4, T, 3, 3, 3, 3> wwww, aaaa, qqqq; };
