#pragma once

#include "GUIWidget.h"

POD_STRUCT(ImageData,
    POD_FIELD(utf8CharPtr, image)
    POD_FIELD(bool, imgUpdated)
    POD_FIELD(bool, flipX)
);

namespace w4::gui
{

class Image : public Widget
{
    W4_GUI_OBJECT(Image, Widget)
public:
    Image(const std::string& path, const w4::math::ivec2& sz, const w4::math::ivec2& pos = {0, 0},  const std::string& name = defaultName);
    Image(const std::string& path, int32_t width, int32_t height, int32_t x = 0, int32_t y = 0, const std::string& name = defaultName);

    Image(const std::string& path, const std::function<void (gui::Image& obj)>& onLoadCallback, const std::string& name = defaultName);

    void setImage(const std::string& path);
    const std::string& getImage() const;

    void setVerticalFlip(bool f);
protected:
    void outerCreator() override;
private:
    w4::sptr<ImageData_Holder> m_imageData;
    std::string                m_src;
    std::function<void (gui::Image& obj)>     m_onLoadCallback;
};

}
