
template <typename T, typename ... _ARGS>
w4::sptr<T> createWidget(w4::sptr<Widget> parent, _ARGS&& ... args)
{
    return Viewport::createWidget<T>(parent, std::forward<_ARGS>(args)...);
}
template <typename T, typename ... _ARGS>
w4::sptr<T> createWidget(Widget* parent, _ARGS&& ... args)
{
    return Viewport::createWidget<T>(parent, std::forward<_ARGS>(args)...);
}

template<typename T>
w4::sptr<T> findWidget(const std::string& name)
{
    return Viewport::findWidget<T>(name);
}


template <typename T, typename ... _ARGS>
w4::sptr<T> Viewport::createWidget(w4::sptr<Widget> parent, _ARGS&& ... args) {
    auto w = w4::make::sptr<T>(std::forward<_ARGS>(args)...);
    if (parent)
        parent->addChild(w);
    addWidget(w);
    return w;
}

template <typename T, typename ... _ARGS>
w4::sptr<T> Viewport::createWidget(Widget* parent, _ARGS&& ... args) {
    auto w = w4::make::sptr<T>(std::forward<_ARGS>(args)...);
    if (parent)
        parent->addChild(w);
    addWidget(w);
    return w;
}

template<typename T>
w4::sptr<T> Viewport::findWidget(const std::string& name)
{
    auto w = findWidget(name);
    if (w)
    {
        return std::static_pointer_cast<T>(w);
    }
    return nullptr;
}

#ifdef __EMSCRIPTEN__

W4_JS_IMPORT
{
    void w4_gui_load_css_font(const char* link);
}
#endif