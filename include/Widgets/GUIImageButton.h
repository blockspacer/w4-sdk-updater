#pragma once

#include "GUIImage.h"

POD_STRUCT(ImageButtonData,
    POD_FIELD(utf8CharPtr, normalImage)
    POD_FIELD(utf8CharPtr, pressedImage)
    POD_FIELD(w4::math::ivec2, pressedOffset)
);


namespace w4::gui
{


class ImageButton: public Widget
{
    W4_GUI_OBJECT(ImageButton, Widget)
public:
    ImageButton(const std::string& normal, const std::string& pressed, const w4::math::ivec2& sz, const w4::math::ivec2& pos = {0, 0}, const std::string& name = defaultName);
    ImageButton(const std::string& normal, const w4::math::ivec2& offset, const w4::math::ivec2& sz, const w4::math::ivec2& pos = {0, 0}, const std::string& name = defaultName);

    void setImage(const std::string& path);

    void setNormalImage(const std::string& path);
    const std::string& getNormalImage() const;

    void setPressedImage(const std::string& path);
    const std::string& getPressedImage() const;

    void setPressedOffset(const math::ivec2& pos);
    const math::ivec2& getPressedOffset() const;

protected:
    void outerCreator() override;
private:
    w4::sptr<ImageButtonData_Holder> m_imgBtnData;
    std::string          m_normal_img;
    std::string          m_pressed_img;
};

}