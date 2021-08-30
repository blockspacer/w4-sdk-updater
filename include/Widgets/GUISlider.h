#pragma once

#include "GUIWidget.h"

POD_STRUCT(SliderData,
           POD_FIELD(float, max)
           POD_FIELD(float, min)
           POD_FIELD(float, step)
           POD_FIELD(float, value)
           POD_FIELD(uint, bgBottom)
           POD_FIELD(uint, bgTop)
           POD_FIELD(uint, fgBottom)
           POD_FIELD(uint, fgTop)
           POD_FIELD(uint, border)
);

namespace w4::gui
{
    class Slider: public Widget
    {
        W4_GUI_OBJECT(Slider, Widget)
    public:
        Slider(float min = 0.f, float max = 1.f, float step = 0.1, float value = 0.f, const w4::math::ivec2& pos = {0, 0}, const std::string& name = defaultName);
        void  setMax(float m);
        float getMax() const;
        void  setMin(float m);
        float getMin() const;
        void  setValue(float v);
        float getValue() const;
        void  setStep(float s);
        float getStep() const;
        void  setForeground(const w4::math::color&, const w4::math::color& = w4::math::color::White);
        void  setBackground(const w4::math::color&, const w4::math::color& = w4::math::color::White);
        void  setBorder (const w4::math::color&);

        void onValueChanged(const std::function<void(float v)>& handler);
    protected:
        static void doValueChanged(Slider*, float);
        virtual void outerCreator() override;
        void changeValue(float);
    private:
        w4::sptr<SliderData_Holder> m_sliderData;
        std::function<void(float v)> m_valueChangeHandler;


    };
}


