template<typename... Args>
void TaskPool<void (Args...)>::emit(Args... args)
{
    while (!m_addedTasks.empty())
    {
        m_tasks.insert(std::move(m_addedTasks.extract(m_addedTasks.begin())));
    }
    for(auto it = m_tasks.begin(), end = m_tasks.end(); it != end;)
    {
        auto &[handle, task] = *it;
        if (task.m_isValid && task.m_func(args...))
        {
            ++it;
        }
        else
        {
            it = m_tasks.erase(it);
        }
    }
}

template<typename... Args>
typename TaskPool<void (Args...)>::Handle TaskPool<void (Args...)>::addCallable(const Callable& callable)
{
    Handle  hdl;
    do
    {
        hdl = ++m_currentIdx;
    } while (m_tasks.count(hdl) || m_addedTasks.count(hdl));
    m_addedTasks.emplace(hdl, Task{callable, true});
    return hdl;
}

template<typename... Args>
bool TaskPool<void (Args...)>::removeByHndl(Handle hndl)
{
    auto it = m_tasks.find(hndl);
    if (it != m_tasks.end())
    {
        bool result = it->second.m_isValid;
        it->second.m_isValid = false;
        return result;
    }
    it = m_addedTasks.find(hndl);
    if (it != m_addedTasks.end())
    {
        m_addedTasks.erase(it);
        return true;
    }
    return false;
}

template<typename... Args>
bool TaskPool<void (Args...)>::isValidByHndl(Handle hndl) const
{
    auto it = m_tasks.find(hndl);
    if (it == m_tasks.end())
    {
        return m_addedTasks.count(hndl) > 0;
    }
    return it->second.m_isValid;
}

template<typename... Args>
void TaskPool<void (Args...)>::reset()
{
    for (auto &[hndl, task]: m_tasks)
    {
        task.m_isValid = false;
    }
    m_addedTasks.clear();
}

template<typename... Args>
bool TaskPool<void (Args...)>::isEmpty() const
{
    for (auto &[hndl, task]: m_tasks)
    {
        if (task.m_isValid)
        {
            return false;
        }
    }
    return m_addedTasks.empty();
}
