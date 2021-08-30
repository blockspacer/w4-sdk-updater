namespace std
{
    template<typename T>
    struct hash<w4::resources::TweenPoint<T>>
    {
        size_t operator()(const w4::resources::TweenPoint<T>& v) const noexcept
        {
            size_t h1 = std::hash<T>{}(v.value);
            size_t h2 = std::hash<float>{}(v.time);
            return h1 ^ (h2 << 1);
        }
    };
}

namespace w4::resources {

template<typename T>
Tween<T>::Tween(T value, float time)
    : m_uid(utils::UIDGenerator::get())
{
    m_points.push_back({time, value});
}

template<typename T>
w4::sptr<Track<T>>& Tween<T>::buildTrack(float fps)
{
    fps = w4::math::equals(fps, 0.f) ? 60.f : fps; // 60 fps is da best! ^_^
    //m_track = w4::make::sptr<Track<T>>(std::to_string(getHash()));
    using Type = typename TrackType<T>::type;
    m_track = w4::make::sptr<Type>(Type::typeInfo.name());
    auto dt = 1.f / fps;
    for(auto t = 0.f; t <= getDuration(); t+= dt)
    {
        TrackValue<T> value;
        value.time = t;
        value.interp = InterpType::LERP;
        value.value = getValue(t);
        m_track->addValue(value);
    }
    return m_track;
}

template<typename T>
float Tween<T>::getDuration() const
{
    return m_points.size() ? m_points.back().time : 0.f;
}

template<typename T>
Tween<T>& Tween<T>::add(T value, float time, Easing easing)
{
    return add(value, time, getEasingFunction<T>(easing));
}

template<typename T>
Tween<T>& Tween<T>::add(T value, float time, const TweenFunction& func)
{
    W4_ASSERT(time >= m_points.back().time);
    m_points.push_back({time, value});
    m_functions.push_back(func);
    return *this;
}

template<typename T>
T Tween<T>::getValue(float t)
{
    if(m_points.size() == 1)
    {
        return m_points.front().value;
    }

    if(t <= m_points.front().time)
    {
        return m_points.front().value;
    }

    if(t >= m_points.back().time)
    {
        return m_points.back().value;
    }

    auto it = std::lower_bound(m_points.begin(), m_points.end(), TweenPoint<T>{t},
            [](const auto& v1, const auto& v2)
            {
                return v1.time < v2.time;
            }
    );
    size_t idx = it - m_points.begin();
    return m_functions[idx - 1](t, m_points[idx - 1], m_points[idx]);
}

template<typename T>
size_t Tween<T>::getHash()
{
    size_t result = std::hash<size_t>{}(m_uid);
    for(const auto& p : m_points)
    {
        size_t h2 = std::hash<TweenPoint<T>>{}(p);
        result = result ^ (h2 << 1);
    }
    return result;
}

} // namespace w4::resources
