#pragma once

namespace w4::resources {

    enum class Easing
    {
        Linear,
        QuadIn,
        QuadOut,
        QuadInOut,
        CubicIn,
        CubicOut,
        CubicInOut,
        SineIn,
        SineOut,
        SineInOut,
        BounceIn,
        BounceOut,
        BounceInOut,
        BackIn,
        BackOut,
        BackInOut,
        CircIn,
        CircOut,
        CircInOut,
        ElasticIn,
        ElasticOut,
        ElasticInOut,
        ExpoIn,
        ExpoOut,
        ExpoInOut,
        QuartIn,
        QuartOut,
        QuartInOut,
        QuintIn,
        QuintOut,
        QuintInOut
    };

    template<typename T>
    struct TweenPoint
    {
        float time;
        T value;
    };

    namespace easing::penner {
        template<typename T>
        T Linear(float t, T b, T c, float d)
        {
            auto x = t / d;
            return c * x + b;
        }

        template<typename T>
        T QuadIn(float t, T b, T c, float d)
        {
            auto x = t / d;
            return c * x * x + b;
        }

        template<typename T>
        T QuadOut(float t, T b, T c, float d)
        {
            auto x = t / d;
            return -1 * c * x * (x - 2) + b;
        }

        template<typename T>
        T QuadInOut(float t, T b, T c, float d)
        {
            if ((t /= d / 2) < 1) return ((c / 2) * (t * t)) + b;
            return -1 * c / 2 * (((t - 2) * (t - 1)) - 1) + b;
        }

        template<typename T>
        T CubicIn(float t, T b, T c, float d)
        {
            t /= d;
            return c * t * t * t + b;
        }

        template<typename T>
        T CubicOut(float t, T b, T c, float d)
        {
            t = t / d - 1;
            return c * (t * t * t + 1) + b;
        }

        template<typename T>
        T CubicInOut(float t, T b, T c, float d)
        {
            if ((t /= d / 2) < 1.f) return c / 2 * t * t * t + b;
            return c / 2 * ((t - 2) * (t - 2) * (t - 2) + 2) + b;
        }

        template<typename T>
        T SineIn(float t, T b, T c, float d)
        {
            return c * -1 * cos(t / d * (w4::math::PI / 2)) + c + b;
        }

        template<typename T>
        T SineOut(float t, T b, T c, float d)
        {
            return c * sin(t / d * (w4::math::PI / 2)) + b;
        }

        template<typename T>
        T SineInOut(float t, T b, T c, float d)
        {
            return c * -1 / 2 * (cos(w4::math::PI * t / d) - 1) + b;
        }

        template<typename T>
        T BounceOut(float t, T b, T c, float d) {
            if ((t /= d) < (1 / 2.75f))
            {
                return c * (7.5625f * t * t) + b;
            }
            else if (t < (2 / 2.75f))
            {
                float postFix = t -= (1.5f / 2.75f);
                return c * (7.5625f * (postFix)*t + .75f) + b;
            }
            else if (t < (2.5 / 2.75))
            {
                float postFix = t -= (2.25f / 2.75f);
                return c * (7.5625f * (postFix)*t + .9375f) + b;
            }
            else
            {
                float postFix = t -= (2.625f / 2.75f);
                return c * (7.5625f * (postFix)*t + .984375f) + b;
            }
        }

        template<typename T>
        T BounceIn(float t, T b, T c, float d)
        {
            return c - BounceOut(d - t, T(), c, d) + b;
        }

        template<typename T>
        T BounceInOut(float t, T b, T c, float d)
        {
            if (t < d / 2.f) return BounceIn(t * 2, T(), c, d) * .5f + b;
            else return BounceOut(t * 2 - d, T(), c, d) * .5f + c * .5f + b;
        }

        template<typename T>
        T BackIn(float t, T b, T c, float d)
        {
            float s = 1.70158f;
            float postFix = t /= d;
            return c * (postFix)*t * ((s + 1) * t - s) + b;
        }

        template<typename T>
        T BackOut(float t, T b, T c, float d)
        {
            float s = 1.70158f;
            t /= d - 1;
            return c * (t * t * ((s + 1) * t + s) + 1) + b;
        }

        template<typename T>
        T BackInOut(float t, T b, T c, float d)
        {
            float s = 1.70158f;
            t /= d / 2;
            s *= 1.525f;
            if (t < 1) return c / 2 * (t * t * ((s + 1) * t - s)) + b;
            float postFix = t -= 2;
            return c / 2 * ((postFix)*t * ((s + 1) * t + s) + 2) + b;
        }

        template<typename T>
        T CircIn(float t, T b, T c, float d)
        {
            t /= d;
            return c * -1 * (sqrt(1 - t * t) - 1) + b;
        }

        template<typename T>
        T CircOut(float t, T b, T c, float d)
        {
            t = t / d - 1;
            return c * sqrt(1 - t * t) + b;
        }

        template<typename T>
        T CircInOut(float t, T b, T c, float d)
        {
            t /= d / 2;
            if (t < 1) return c * -1 / 2 * (sqrt(1 - t * t) - 1) + b;
            return c / 2 * (sqrt(1 - t * (t - 2)) + 1) + b;
        }

        template<typename T>
        T ElasticIn(float t, T b, T c, float d)
        {
            if (t == 0) return b;
            if ((t /= d) == 1) return b + c;
            float p = d * .3f;
            T a = c;
            float s = p / 4;
            T postFix = a * pow(2, 10 * (t -= 1)); // this is a fix, again, with post-increment operators
            return -1 * (postFix * sin((t * d - s) * (2 * w4::math::PI) / p)) + b;
        }

        template<typename T>
        T ElasticOut(float t, T b, T c, float d) {
            if (t == 0) return b;
            if ((t /= d) == 1) return b + c;
            float p = d * .3f;
            T a = c;
            float s = p / 4;
            return (a * pow(2, -10 * t) * sin((t * d - s) * (2 * w4::math::PI) / p) + c + b);
        }

        template<typename T>
        T ElasticInOut(float t, T b, T c, float d) {
            if (t == 0) return b;
            if ((t /= d / 2) == 2) return b + c;
            float p = d * (.3f * 1.5f);
            T a = c;
            float s = p / 4;

            if (t < 1)
            {
                T postFix = a * pow(2, 10 * (t -= 1));
                return -.5f * (postFix * sin((t * d - s) * (2 * w4::math::PI) / p)) + b;
            }

            T postFix = a * pow(2, -10 * (t -= 1));
            return postFix * sin((t * d - s) * (2 * w4::math::PI) / p) * .5f + c + b;
        }

        template<typename T>
        T ExpoIn(float t, T b, T c, float d)
        {
            return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
        }

        template<typename T>
        T ExpoOut(float t, T b, T c, float d)
        {
            return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
        }

        template<typename T>
        T ExpoInOut(float t, T b, T c, float d)
        {
            if (t == 0) return b;
            if (t == d) return b + c;
            if ((t /= d / 2) < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;
            return c / 2 * (-pow(2, -10 * --t) + 2) + b;
        }



        template<typename T>
        T QuartIn(float t, T b, T c, float d)
        {
            t /= d;
            return c * t * t * t * t + b;
        }

        template<typename T>
        T QuartOut(float t, T b, T c, float d)
        {
            t = t / d - 1;
            return -1 * c * (t * t * t * t - 1) + b;
        }

        template<typename T>
        T QuartInOut(float t, T b, T c, float d)
        {
            t = t / d / 2;
            if (t < 1) return c / 2 * t * t * t * t + b;
            t = t - 2;
            return -1 * c / 2 * (t * t * t * t - 2) + b;
        }

        template<typename T>
        T QuintIn(float t, T b, T c, float d)
        {
            t /= d;
            return c * t * t * t * t * t + b;
        }

        template<typename T>
        T QuintOut(float t, T b, T c, float d)
        {
            t = t / d - 1;
            return c * (t * t * t * t * t + 1) + b;
        }

        template<typename T>
        T QuintInOut(float t, T b, T c, float d)
        {
            t = t / d / 2;
            if (t < 1) return c / 2 * t * t * t * t * t + b;
            t -= 2;
            return c / 2 * (t * t * t * t * t + 2) + b;
        }
    } // namespace easing::penner

    namespace easing {
        template<typename T>
        T Linear(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::Linear(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T QuadIn(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::QuadIn(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T QuadOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::QuadOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T QuadInOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::QuadInOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T CubicIn(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::CubicIn(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T CubicOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::CubicOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T CubicInOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::CubicInOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T SineIn(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::SineIn(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T SineOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::SineOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T SineInOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::SineInOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T BounceOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::BounceOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T BounceIn(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::BounceIn(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T BounceInOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::BounceInOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T BackIn(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::BackIn(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T BackOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::BackOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T BackInOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::BackInOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T CircIn(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::CircIn(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T CircOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::CircOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T CircInOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::CircInOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T ElasticIn(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::ElasticIn(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T ElasticOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::ElasticOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T ElasticInOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::ElasticInOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T ExpoIn(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::ExpoIn(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T ExpoOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::ExpoOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T ExpoInOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::ExpoInOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T QuartIn(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::QuartIn(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T QuartOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::QuartOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T QuartInOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::QuartInOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T QuintIn(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::QuintIn(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T QuintOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::QuintOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }

        template<typename T>
        T QuintInOut(float t, const TweenPoint<T>& b, const TweenPoint<T>& e) {
            return penner::QuintInOut(t - b.time, b.value, e.value - b.value, e.time - b.time);
        }
    }

    template<typename T>
    typename std::function<T(float, const TweenPoint<T>&, const TweenPoint<T>&)> getEasingFunction(Easing easing)
    {
        switch (easing)
        {
        case Easing::Linear: return &easing::Linear<T>;
        case Easing::QuadIn: return &easing::QuadIn<T>;
        case Easing::QuadOut: return &easing::QuadOut<T>;
        case Easing::QuadInOut: return &easing::QuadInOut<T>;
        case Easing::CubicIn: return &easing::CubicIn<T>;
        case Easing::CubicOut: return &easing::CubicOut<T>;
        case Easing::CubicInOut: return &easing::CubicInOut<T>;
        case Easing::SineIn: return &easing::SineIn<T>;
        case Easing::SineOut: return &easing::SineOut<T>;
        case Easing::SineInOut: return &easing::SineInOut<T>;
        case Easing::BounceIn: return &easing::BounceIn<T>;
        case Easing::BounceOut: return &easing::BounceOut<T>;
        case Easing::BounceInOut: return &easing::BounceInOut<T>;
        case Easing::BackIn: return &easing::BackIn<T>;
        case Easing::BackOut: return &easing::BackOut<T>;
        case Easing::BackInOut: return &easing::BackInOut<T>;
        case Easing::CircIn: return &easing::CircIn<T>;
        case Easing::CircOut: return &easing::CircOut<T>;
        case Easing::CircInOut: return &easing::CircInOut<T>;
        case Easing::ElasticIn: return &easing::ElasticIn<T>;
        case Easing::ElasticOut: return &easing::ElasticOut<T>;
        case Easing::ElasticInOut: return &easing::ElasticInOut<T>;
        case Easing::ExpoIn: return &easing::ExpoIn<T>;
        case Easing::ExpoOut: return &easing::ExpoOut<T>;
        case Easing::ExpoInOut: return &easing::ExpoInOut<T>;
        case Easing::QuartIn: return &easing::QuartIn<T>;
        case Easing::QuartOut: return &easing::QuartOut<T>;
        case Easing::QuartInOut: return &easing::QuartInOut<T>;
        case Easing::QuintIn: return &easing::QuintIn<T>;
        case Easing::QuintOut: return &easing::QuintOut<T>;
        case Easing::QuintInOut: return &easing::QuintInOut<T>;
        default:
            FATAL_ERROR("getEasingFunction(): easing function %d isn't supported", static_cast<int>(easing));
            break;
        }
    }

} // namespace w4::resources
