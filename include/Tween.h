#pragma once

#include <vector>
#include <functional>
#include "Easing.h"
#include "Track.h"
#include "Utils.h"

namespace w4::resources {

template<typename T>
class Tween
{
public:
    using Uid = size_t;
    using Points = std::vector<TweenPoint<T>>;
    using TweenFunction = std::function<T (float, const TweenPoint<T>&, const TweenPoint<T>&)>;
    using Functions = std::vector<TweenFunction>;

    Tween(T value, float time = 0.0f);

    Tween& add(T value, float time, Easing easing);
    Tween& add(T value, float time, const TweenFunction& func);

    w4::sptr<Track<T>>& buildTrack(float fps);
    float getDuration() const;
    T getValue(float t);

private:
    size_t getHash();

    Uid m_uid;
    Points m_points;
    Functions m_functions;
    w4::sptr<Track<T>> m_track;
};

}

#include "impl/Tween.inl"
