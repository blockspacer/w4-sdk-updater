#pragma once

#include <unordered_set>

#include "W4Common.h"
#include "Object.h"
#include "W4Math.h"
#include "IOuterID.h"
#include "PodGenerator.h"
#include "Resource.h"


#define W4_GUI_OBJECT(T, S) W4_OBJECT_IMPL("Widget", T, S)

namespace w4::gui {

enum struct VerticalAlign
{
    Top = 0,
    Center = 1,
    Bottom = 2
};

enum struct HorizontalAlign
{
    Left = 0,
    Center = 1,
    Right = 2
};

enum struct SizePolicy
{
    Auto = 0,
    Fixed = 1
};

}

POD_STRUCT(WidgetData,
    POD_FIELD(w4::math::ivec2, position)
    POD_FIELD(bool, visible)
    POD_FIELD(bool, enabled)
    POD_FIELD(w4::math::ivec2, size)
    POD_FIELD(uint32_t, zOrder)
    POD_FIELD(float, opacity)
    POD_FIELD(uint, verticalAlign)
    POD_FIELD(uint, horizontalAlign)
    POD_FIELD(uint, verticalSize)
    POD_FIELD(uint, horizontalSize),
    WidgetData(): position(0, 0), visible(true), size(1, 1), zOrder(0), opacity(1.f), verticalAlign(0), horizontalAlign(0), verticalSize(0), horizontalSize(0) {}
);

namespace w4::gui
{
    class Widget: public core::Object, public core::IOuterManaged
    {
        W4_GUI_OBJECT(Widget, core::Object)
    public:
        using Callback = std::function<void(Widget&)>;

        virtual ~Widget();

        void setVisible(bool visible);
        bool isVisible() const;

        void setEnabled(bool enable);
        bool isEnabled() const;

        void setIgnoreScreenshot(bool ignore);
        bool getIgnoreScreenshot() const;

        void setPosition(const math::ivec2& pos);
        const math::ivec2& getPosition() const;

        void setOrder(uint32_t z);
        uint32_t getOrder() const;

        void setOpacity(float op);
        float getOpacity() const;

        void setSize(const w4::math::ivec2& sz);
        const w4::math::ivec2& getSize() const;

        void setWidth(uint w);
        void setHeight(uint h);

        uint getWidth() const;
        uint getHeight() const;

        void setVerticalAlign(VerticalAlign align);
        void setHorizontalAlign(HorizontalAlign align);
        VerticalAlign getVerticalAlign() const;
        HorizontalAlign getHorizontalAlign() const;

        void setVerticalPolicy(SizePolicy policy);
        void setHorizontalPolicy(SizePolicy policy);
        void setSizePolicy(SizePolicy vertical, SizePolicy horizontal);

        SizePolicy getVerticalPolicy() const;
        SizePolicy getHorizontalPolicy() const;

        using TapHandler = std::function<void()>;

        void onTap(const TapHandler& handler);
        void onTouchBegin(const TapHandler& handler);
        void onTouchEnd(const TapHandler& handler);

        Widget(const std::string& name = defaultName);

        enum class Event
        {
            Tap   = 0,
            TouchBegin = 1,
            TouchEnd = 2
        };

    protected:

        virtual void updateView();

        void addChild(w4::cref<Widget> w);
        void removeChild(w4::core::OuterID::ndxType childID);

        void update();

        void traversal(const Callback&);
        void foreachChildren(const Callback&);

        void onNameChange() override;

        void onPositionChanged();
        void onVisibleChanged();
        void onOpacityChanged();

        void outerCreator() override;
        void outerDeleter() override;

        void showAtViewport();
        void hideFromViewport();

        std::unordered_set<w4::sptr<Widget>> m_children;

        Widget* m_parent = nullptr;

        w4::sptr<WidgetData_Holder> getWidgetData() const;
        void requestUpdate();

        TapHandler m_tapHandler = nullptr;
        TapHandler m_touchBeginHandler = nullptr;
        TapHandler m_touchEndHandler = nullptr;

        static void doEvent(w4::core::OuterID::ndxType id, Event evt);


        static std::unordered_map<w4::core::OuterID::ndxType, Widget*> m_tapWidgets;

        friend class Viewport;

    private:
        w4::math::ivec2             m_localPosition;

        w4::sptr<WidgetData_Holder> m_widgetData;

        bool                        m_visible = true;
        bool                        m_onViewport = false;
        bool                        m_updateRequested = false;
        bool                        m_onDestroy  = false;
        bool                        m_ignoreScreenshot = false;
    };

    namespace internal
    {
        using OuterID = w4::core::OuterID::ndxType;
        OuterID create_widget(const char* widgetClassName, OuterID podID);
        void    update_widget(const char* widgetClassName, OuterID widgetID);
        void    remove_widget(const char* widgetClassName, OuterID widgetID);
    }
}
