

template<typename T, typename D>
typename EventImpl<T, D>::Handle::sptr EventImpl<T, D>::subscribe(const EventImpl<T, D>::Handler& h)
{
    auto& internal = Internal::instance();
    auto ndx = internal.currentIdx++;
    internal.handlers[ndx] = h;
    auto handle = w4::make::sptr<typename EventImpl<T, D>::Handle>(ndx);
    return handle;
}

template<typename T, typename D>
void EventImpl<T, D>::unsubscribe(typename Handle::id_t id)
{
    Internal::instance().handlerErased.emplace(id);
}

template <typename T, typename D>
void EventImpl<T, D>::fire(cref e)
{
    auto& internal = Internal::instance();
    internal.events.push(e);
    Event::emitterQueue.push(internal.emitterIdx);
}

template <typename T, typename D>
void EventImpl<T, D>::performEmit(cref e)
{
    auto& internal = Internal::instance();

    for (auto it = internal.handlers.begin(); it != internal.handlers.end(); )
    {
        if (internal.handlerErased.count(it->first))
        {
            it = internal.handlers.erase(it);
        }
        else
        {
            it->second(e);
            ++it;
        }
    }
    internal.handlerErased.clear();

    Super::performEmit(static_cast<const Super&>(e));
}

template <typename T, typename D>
void EventImpl<T, D>::performEmitFirst()
{
    auto& events = Internal::instance().events;
    if(!events.empty())
    {
        performEmit(events.front());
        events.pop();
    }
}

template <typename T, typename D>
void EventImpl<T, D>::Handle::unsubscribe()
{
    EventImpl<T, D>::unsubscribe(m_id);
}
