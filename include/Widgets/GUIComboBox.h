#pragma once

#include <initializer_list>

#include "GUIWidget.h"
#include "GUIFont.h"


POD_STRUCT(ComboBoxData,
    POD_FIELD(utf8CharPtr, text)
    POD_FIELD(utf8CharPtr, font)
    POD_FIELD(uint, fontSize)
    POD_FIELD(uint, fontColor)
    POD_FIELD(uint, bgColor)
    POD_FIELD(bool, fontBold)
    POD_FIELD(bool, fontItalic)
);


namespace w4::gui
{
    class ComboBox : public Widget
    {
        W4_GUI_OBJECT(ComboBox, Widget)
    public:
        // Combobox section

        ComboBox(const std::string& text = "ComboBox", const w4::math::ivec2& pos = {0, 0});

        ComboBox(const std::initializer_list<std::string>& items, const std::string& text = "ComboBox", const w4::math::ivec2& pos = {0, 0});

        void addItem(const std::string& text);

        template<class ForwardIter>
        void addItems(ForwardIter first, ForwardIter last);

        void addItems(const std::initializer_list<std::string>& items);

        void insertItem(int32_t index, const std::string& text);

        template<class ForwardIter>
        void insertItems(int32_t index, ForwardIter first, ForwardIter last);

        void insertItems(int32_t index, const std::initializer_list<std::string>& items);

        void setCurrentIndex(int32_t index);

        int32_t getCurrentIndex() const;

        std::string getCurrentText() const;

        uint32_t getCount() const;

        void removeItem(int32_t index);

        // set default text
        void setText(const std::string& text);
        const std::string& getText() const;

        // setup visual style section
        void setFontSize(uint size);
        uint getFontSize() const;

        void setFont(const std::string& uri, const std::string& font);
        void setFont(const std::string& font);
        void setFont(Font font);
        const std::string& getFont() const;


        void setBold(bool bold);
        bool isBold() const;

        void setItalic(bool italic);
        bool isItalic() const;

        void                   setBgColor(const w4::math::color& color);
        const w4::math::color& getBgColor() const;

        void                   setTextColor(const w4::math::color& color);
        const w4::math::color& getTextColor() const;

        void onCurrentIndexChanged(const std::function<void(int)>& index);

    protected:
        static void doCurrentIndexChanged(ComboBox* slider, int32_t value);
        void changeCurrentIndex(int32_t value);
        virtual void outerCreator() override;
    private:
        std::vector<std::string> m_items;
        std::function<void(int)> m_indexChangeHandler;
        int32_t m_currentItem = -1;

        w4::sptr<ComboBoxData_Holder> m_data;
        std::string m_text;
        std::string m_font;
        w4::math::color m_bgColor;
        w4::math::color m_textColor;
        w4::math::color m_selectedColor;
    };

#include "impl/GUIComboBox.inl"
}