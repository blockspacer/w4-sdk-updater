#pragma once

#include "Platform.h"

namespace w4::gui {

class Viewport
{
public:
    static void init();
    static void update(float dt);

    template<typename T, typename ... _ARGS>
    static w4::sptr<T> createWidget(w4::sptr<Widget> parent, _ARGS&& ... args);
    template<typename T, typename ... _ARGS>
    static w4::sptr<T> createWidget(Widget* parent, _ARGS&& ... args);

    static void removeWidget(w4::sptr<Widget> w);
    static void removeAllWidgets();

    template<typename T>
    static w4::sptr<T> findWidget(const std::string& name);

protected:
    static void onResize(w4::event::Resize::cref evt);

    static w4::sptr<Widget> findWidget(const std::string& name);
    static void addWidget(w4::sptr<Widget> w);
    static void renameWidget(const std::string& name, w4::core::OuterID::ndxType widgetID);

    static inline w4::event::Resize::Handle::sptr  m_resizeHandler;
    static inline std::unordered_multimap<std::string, w4::sptr<Widget>> m_widgets;

    friend class Widget;
};

}