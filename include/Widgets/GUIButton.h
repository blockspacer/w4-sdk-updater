#pragma once

#include "GUIWidget.h"
#include "GUIFont.h"

POD_STRUCT(ButtonData,
           POD_FIELD(utf8CharPtr, text)
           POD_FIELD(utf8CharPtr, font)
           POD_FIELD(uint, fontSize)
           POD_FIELD(uint, fontColor)
           POD_FIELD(uint, bgColor)
           POD_FIELD(bool, fontBold)
           POD_FIELD(bool, fontItalic)
           POD_FIELD(uint, lineHeight)
);


namespace w4::gui
{

class Button : public Widget
{
    W4_GUI_OBJECT(Button, Widget)
public:
    Button(const std::string& text = "Button", const w4::math::ivec2& pos = {0, 0});

    void setText(const std::string& text);
    const std::string& getText() const;

    void setFont(const std::string& uri, const std::string& font);
    void setFont(const std::string& font);
    void setFont(Font font);
    const std::string& getFont() const;

    void setBold(bool bold);
    bool isBold() const;

    void setItalic(bool italic);
    bool isItalic() const;

    void setFontSize(uint size);
    uint getFontSize() const;

    void                   setBgColor(const w4::math::color& color);
    const w4::math::color& getBgColor() const;

    void                   setTextColor(const w4::math::color& color);
    const w4::math::color& getTextColor() const;

protected:
    virtual void outerCreator() override;
private:
    w4::sptr<ButtonData_Holder> m_buttonData;
    std::string m_text;
    std::string m_font;
    w4::math::color m_bgColor;
    w4::math::color m_textColor;
};

}
