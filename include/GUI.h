#pragma once

#include "GUIWidget.h"
#include "GUIViewport.h"

namespace w4::gui
{

template <typename T, typename ... _ARGS>
w4::sptr<T> createWidget(w4::sptr<Widget> parent, _ARGS&& ... args);

template <typename T, typename ... _ARGS>
w4::sptr<T> createWidget(Widget* parent, _ARGS&& ... args);

template<typename T>
w4::sptr<T> findWidget(const std::string& name);

void removeWidget(w4::sptr<Widget> w);
void removeAllWidgets();

void setVirtualResolution(const w4::math::size& sz);
w4::math::size getVirtualResolution();


#include "impl/GUI.inl"

}