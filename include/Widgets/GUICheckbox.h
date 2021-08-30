#pragma once

#include "GUIWidget.h"

POD_STRUCT(CheckboxData,
           POD_FIELD(bool, checked)
           POD_FIELD(uint, bgBottom)
           POD_FIELD(uint, bgTop)
           POD_FIELD(uint, fgBottom)
           POD_FIELD(uint, fgTop)
);

namespace w4::gui
{
class Checkbox: public Widget
{
W4_GUI_OBJECT(Checkbox, Widget)
public:
    Checkbox(const w4::math::ivec2& pos = {0, 0}, const std::string& name = defaultName);

    void setBgColor(const w4::math::color& bottom, const w4::math::color& top = w4::math::color::White);
    std::pair<w4::math::color, w4::math::color> getBgColor() const;

    void setFgColor(const w4::math::color& bottom, const w4::math::color& top = w4::math::color::White);
    std::pair<w4::math::color, w4::math::color> getFgColor() const;

    void onStateChanged(const std::function<void(bool v)>& handler);
    void setChecked(bool state);
    bool isChecked() const;
protected:
    static void doStateChanged(Checkbox*, bool);
    virtual void outerCreator() override;
    void changeState(bool);
private:
    w4::sptr<CheckboxData_Holder> m_data;
    std::function<void(bool v)> m_stateChangeHandler;

};
}