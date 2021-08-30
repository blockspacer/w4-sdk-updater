#pragma once

#include "GUIWidget.h"
#include "GUIFont.h"
#include "W4Math.h"

POD_STRUCT(LabelData,
    POD_FIELD(utf8CharPtr, text)
    POD_FIELD(utf8CharPtr, font)
    POD_FIELD(uint, fontSize)
    POD_FIELD(uint, fontColor)
    POD_FIELD(uint, bgColor)
    POD_FIELD(w4::math::vec2, padding)
    POD_FIELD(uint, lineHeight)
    POD_FIELD(uint, widgetPOD)
    POD_FIELD(bool, fontBold)
    POD_FIELD(bool, fontItalic)
    POD_FIELD(uint, vertTextAlign)
    POD_FIELD(uint, horzTextAlign)
    POD_FIELD(w4::math::ivec2, shadowOffset)
    POD_FIELD(uint, shadowColor)
);

namespace w4::gui
{

class Label : public Widget
{
    W4_GUI_OBJECT(Label, Widget)
public:
    Label(const std::string& text = "Label", const w4::math::ivec2& pos = {0, 0});

    void setText(const std::string& text);
    const std::string& getText() const;

    // Values of padding setup in "em". It's relative to the font-size of the element (2em means 2 times the size of the current font)
    void setPadding(const w4::math::vec2& padding);
    const w4::math::vec2& getPadding() const;

    void setBold(bool bold);
    bool isBold() const;

    void setItalic(bool italic);
    bool isItalic() const;

    void setFontSize(uint size);
    uint getFontSize() const;

    void setFont(Font font);
    void setFont(const std::string& font);
    void setFont(const std::string& uri, const std::string& font);
    const std::string& getFont() const;

    void setTextColor(const w4::math::color& color);
    const w4::math::color& getTextColor() const;

    void setBgColor(const w4::math::color& color);
    const w4::math::color& getBgColor() const;

    void setTextAlign(HorizontalAlign h, VerticalAlign v = VerticalAlign::Top);
    HorizontalAlign getHorizontalTextAlign() const;
    VerticalAlign getVerticalTextAlign() const;

    void setShadowOffset(const w4::math::ivec2& o);
    const math::ivec2& getShadowOffset() const;

    void setShadowColor(const w4::math::color& color);
    const w4::math::color& getShadowColor() const;
protected:
    virtual void outerCreator() override;
private:
    w4::sptr<LabelData_Holder> m_labelData;
    std::string     m_text;
    w4::math::color m_bgColor;
    w4::math::color m_textColor;
    std::string     m_font;
    w4::math::ivec2 m_shadowOffset;
    w4::math::color m_shadowColor;
};

}
