#pragma once

#include <vector>
#include <unordered_map>

#include "W4Common.h"
#include "W4Math.h"
#include "Texture.h"
#include "MaterialInstance.h"
#include "TaskPool.h"
#include "Event.h"
#include "PodGenerator.h"

#define MAX_GAMEPAD_BUTTONS 20
#define MAX_GAMEPAD_AXES    20

POD_STRUCT(GAMEPAD_DATA,
           POD_FIELD(uint  , buttonsCount)
           POD_FIELD(uint  , axesCount)
           POD_ARRAY(bool  , buttonsState, MAX_GAMEPAD_BUTTONS)
           POD_ARRAY(float , axesState,    MAX_GAMEPAD_AXES)
           ,
           GAMEPAD_DATA(): buttonsCount(0),
           axesCount(0)
           {
               std::fill(buttonsState, buttonsState + MAX_GAMEPAD_BUTTONS, 0);
               std::fill(axesState, axesState + MAX_GAMEPAD_AXES, 0.f);
           }
)

namespace w4::render
{
    class Mesh;
}

namespace w4::event
{
    struct Resize : public w4::event::EventImpl<Resize, w4::event::Event>
    {
        W4_TYPEINFO(Resize, "Event", Event)
        Resize(const math::size& sz) : size(sz)   {}
        Resize(uint w, uint h)       : size(w, h) {}
        math::size size;
    };


    struct keyboardProxy : public w4::event::EventImpl<keyboardProxy, w4::event::Event>
    {
        enum class Key
        {
            Digit0 = 0x30,
            Digit1,
            Digit2,
            Digit3,
            Digit4,
            Digit5,
            Digit6,
            Digit7,
            Digit8,
            Digit9,
            A = 0x41,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            F1 = 0x70,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            ShiftLeft = 0xA0,
            ShiftRight = 0xA1,
            CtrlLeft = 0xA2,
            CtrlRight = 0xA3,
            AltLeft = 0xA4,
            AltRight = 0xA5,
            ArrowUp = 0x26,
            ArrowDown = 0x28,
            ArrowLeft = 0x25,
            ArrowRight = 0x27,
            Space = 0x20,
            Enter = 0x0D,
            Escape = 0x1B,
            Insert = 0x45,
            Delete = 0x46,
            CapsLock = 0x14,
            Tab = 0x09,
            Numpad0 = 0x60,
            Numpad1,
            Numpad2,
            Numpad3,
            Numpad4,
            Numpad5,
            Numpad6,
            Numpad7,
            Numpad8,
            Numpad9,
            NumpadMultiply = 0x6A,
            NumpadAdd      = 0x6B,
            NumpadSubtract = 0x6D,
            NumpadDivide   = 0x6F,
            NumpadEnter    = 0x0D
        } key;

        enum Modifier : uint8_t
        {
            None  = 0x00,
            Alt   = 0x01,
            Ctrl  = 0x02,
            Shift = 0x04
        };
        uint modifiers;

        keyboardProxy(Key key, uint modifiers = Modifier::None)
            :key(key), modifiers(modifiers) {}

        keyboardProxy(cref) = default;

        bool getModifierState(Modifier modifier) const;
    };

    struct Keyboard: public keyboardProxy
    {
        W4_TYPEINFO(Keyboard, "Event", Event)
        struct Down : public w4::event::EventImpl<Down, keyboardProxy>
        {
            W4_TYPEINFO(Down, "KeyboardEvent", Keyboard)
            Down(Key key, uint modifiers = Modifier::None)
                :Base(key, modifiers) {}

            Down(cref) = default;
        };

        struct Up : public w4::event::EventImpl<Up, keyboardProxy>
        {
            W4_TYPEINFO(Up, "KeyboardEvent", Keyboard)
            Up(Key key, uint modifiers = Modifier::None)
                :Base(key, modifiers) {}

            Up(cref) = default;
        };
    };

    struct touchProxy : public w4::event::EventImpl<touchProxy, w4::event::Event>
    {
        touchProxy(const math::point& p, int touchId): point(p), touchId(touchId) {}
        touchProxy(int x, int y, int touchId): point(x, y), touchId(touchId) {}
        touchProxy(cref) = default;
        math::point point;
        int touchId = 0;
    };

    struct Touch : public touchProxy
    {
        W4_TYPEINFO(Touch, "Event", Event)

        struct Begin : public w4::event::EventImpl<Begin, touchProxy>
        {
            W4_TYPEINFO(Begin, "TouchEvent", Touch)
            Begin(const math::point& p, int touchId): Base(p, touchId) {}
            Begin(int x, int y, int touchId): Base(x, y, touchId) {}
            Begin(cref) = default;
        };

        struct End : public w4::event::EventImpl<End, touchProxy>
        {
            W4_TYPEINFO(End, "TouchEvent", Touch)
            End(const math::point& p, int touchId): Base(p, touchId) {}
            End(int x, int y, int touchId): Base(x, y, touchId) {}
            End(cref) = default;
        };

        struct Move : public w4::event::EventImpl<Move, touchProxy>
        {
            W4_TYPEINFO(Move, "TouchEvent", Touch)
            Move(const math::point& p, int touchId): Base(p, touchId) {}
            Move(int x, int y, int touchId): Base(x, y, touchId) {}
            Move(cref) = default;
        };
    };

    struct Tap : public w4::event::EventImpl<Tap, w4::event::Event>
    {
        W4_TYPEINFO(Tap, "Event", Event)

        Tap(const math::point& p): point(p) {}
        Tap(int x, int y): point(x, y) {}
        Tap(cref) = default;
        math::point point;
    };

    struct swipeProxy : public w4::event::EventImpl<swipeProxy, w4::event::Event>
    {
        W4_TYPEINFO(swipeProxy, "Event", Event)
        swipeProxy(const math::point& b, const math::point& e): begin(b), end(e) {}
        swipeProxy(cref) = default;

        math::point begin;
        math::point end;
    };

    struct Swipe : public swipeProxy
    {
        W4_TYPEINFO(Swipe, "Event", swipeProxy)
        struct Left : public w4::event::EventImpl<Left, swipeProxy>
        {
            W4_TYPEINFO(Left, "SwipeEvent", Swipe)
            Left(const math::point& b, const math::point& e): Base(b, e) {}
            Left(cref) = default;
        };

        struct Right : public w4::event::EventImpl<Right, swipeProxy>
        {
            W4_TYPEINFO(Right, "SwipeEvent", Swipe)
            Right(const math::point& b, const math::point& e): Base(b, e) {}
            Right(cref) = default;
        };

        struct Up : public w4::event::EventImpl<Up, swipeProxy>
        {
            W4_TYPEINFO(Up, "SwipeEvent", Swipe)
            Up(const math::point& b, const math::point& e): Base(b, e) {}
            Up(cref) = default;
        };

        struct Down : public w4::event::EventImpl<Down, swipeProxy>
        {
            W4_TYPEINFO(Down, "SwipeEvent", Swipe)
            Down(const math::point& b, const math::point& e): Base(b, e) {}
            Down(cref) = default;
        };
    };

    struct joystickProxy : public w4::event::EventImpl<joystickProxy, w4::event::Event>
    {
        joystickProxy(const math::vec2& direction): direction(direction) {}
        joystickProxy(cref) = default;

        math::vec2 direction;
    };

    struct Joystick : public joystickProxy
    {
        W4_TYPEINFO(Joystick, "Event", Event)
        struct LeftThumbstickMove : public w4::event::EventImpl<LeftThumbstickMove, joystickProxy>
        {
            W4_TYPEINFO(LeftThumbstickMove, "JoystickEvent", Joystick)
            LeftThumbstickMove(const math::vec2& direction): Base(direction) {}
            LeftThumbstickMove(cref) = default;
        };

        struct RightThumbstickMove : public w4::event::EventImpl<RightThumbstickMove, joystickProxy>
        {
            W4_TYPEINFO(RightThumbstickMove, "JoystickEvent", Joystick)
            RightThumbstickMove(const math::vec2& direction): Base(direction) {}
            RightThumbstickMove(cref) = default;
        };

        struct LeftThumbstickPress : public w4::event::EventImpl<LeftThumbstickPress, joystickProxy>
        {
            W4_TYPEINFO(LeftThumbstickPress, "JoystickEvent", Joystick)
            LeftThumbstickPress(): Base(math::vec2(0, 0)) {}
            LeftThumbstickPress(cref) = default;
        };

        struct LeftThumbstickRelease : public w4::event::EventImpl<LeftThumbstickRelease, joystickProxy>
        {
            W4_TYPEINFO(LeftThumbstickRelease, "JoystickEvent", Joystick)
            LeftThumbstickRelease(): Base(math::vec2(0, 0)) {}
            LeftThumbstickRelease(cref) = default;
        };

        struct RightThumbstickPress : public w4::event::EventImpl<RightThumbstickPress, joystickProxy>
        {
            W4_TYPEINFO(RightThumbstickPress, "JoystickEvent", Joystick)
            RightThumbstickPress(): Base(math::vec2(0, 0)) {}
            RightThumbstickPress(cref) = default;
        };

        struct RightThumbstickRelease : public w4::event::EventImpl<RightThumbstickRelease, joystickProxy>
        {
            W4_TYPEINFO(RightThumbstickRelease, "JoystickEvent", Joystick)
            RightThumbstickRelease(): Base(math::vec2(0, 0)) {}
            RightThumbstickRelease(cref) = default;
        };

        struct LeftThumbstickHorizontalAxis : public w4::event::EventImpl<LeftThumbstickHorizontalAxis, joystickProxy>
        {
            W4_TYPEINFO(LeftThumbstickHorizontalAxis, "JoystickEvent", Joystick)
            LeftThumbstickHorizontalAxis(const float& value): Base(math::vec2(value, 0)), value(value) {}
            LeftThumbstickHorizontalAxis(cref) = default;

            float value = 0;
        };

        struct LeftThumbstickVerticalAxis : public w4::event::EventImpl<LeftThumbstickVerticalAxis, joystickProxy>
        {
            W4_TYPEINFO(LeftThumbstickVerticalAxis, "JoystickEvent", Joystick)
            LeftThumbstickVerticalAxis(const float& value): Base(math::vec2(0, value)), value(value) {}
            LeftThumbstickVerticalAxis(cref) = default;

            float value = 0;
        };

        struct RightThumbstickHorizontalAxis : public w4::event::EventImpl<RightThumbstickHorizontalAxis, joystickProxy>
        {
            W4_TYPEINFO(RightThumbstickHorizontalAxis, "JoystickEvent", Joystick)
            RightThumbstickHorizontalAxis(const float& value): Base(math::vec2(value, 0)), value(value) {}
            RightThumbstickHorizontalAxis(cref) = default;

            float value = 0;
        };

        struct RightThumbstickVerticalAxis : public w4::event::EventImpl<RightThumbstickVerticalAxis, joystickProxy>
        {
            W4_TYPEINFO(RightThumbstickVerticalAxis, "JoystickEvent", Joystick)
            RightThumbstickVerticalAxis(const float& value): Base(math::vec2(0, value)), value(value) {}
            RightThumbstickVerticalAxis(cref) = default;

            float value = 0;
        };

        struct XButtonPress : public w4::event::EventImpl<XButtonPress, joystickProxy>
        {
            W4_TYPEINFO(XButtonPress, "JoystickEvent", Joystick)
            XButtonPress(): Base(math::vec2(0, 0)) {}
            XButtonPress(cref) = default;
        };

        struct XButtonRelease : public w4::event::EventImpl<XButtonRelease, joystickProxy>
        {
            W4_TYPEINFO(XButtonRelease, "JoystickEvent", Joystick)
            XButtonRelease(): Base(math::vec2(0, 0)) {}
            XButtonRelease(cref) = default;
        };

        struct OButtonPress : public w4::event::EventImpl<OButtonPress, joystickProxy>
        {
            W4_TYPEINFO(OButtonPress, "JoystickEvent", Joystick)
            OButtonPress(): Base(math::vec2(0, 0)) {}
            OButtonPress(cref) = default;
        };

        struct OButtonRelease : public w4::event::EventImpl<OButtonRelease, joystickProxy>
        {
            W4_TYPEINFO(OButtonRelease, "JoystickEvent", Joystick)
            OButtonRelease(): Base(math::vec2(0, 0)) {}
            OButtonRelease(cref) = default;
        };

        struct SButtonPress : public w4::event::EventImpl<SButtonPress, joystickProxy>
        {
            W4_TYPEINFO(SButtonPress, "JoystickEvent", Joystick)
            SButtonPress(): Base(math::vec2(0, 0)) {}
            SButtonPress(cref) = default;
        };

        struct SButtonRelease : public w4::event::EventImpl<SButtonRelease, joystickProxy>
        {
            W4_TYPEINFO(SButtonRelease, "JoystickEvent", Joystick)
            SButtonRelease(): Base(math::vec2(0, 0)) {}
            SButtonRelease(cref) = default;
        };

        struct TButtonPress : public w4::event::EventImpl<TButtonPress, joystickProxy>
        {
            W4_TYPEINFO(TButtonPress, "JoystickEvent", Joystick)
            TButtonPress(): Base(math::vec2(0, 0)) {}
            TButtonPress(cref) = default;
        };

        struct TButtonRelease : public w4::event::EventImpl<TButtonRelease, joystickProxy>
        {
            W4_TYPEINFO(TButtonRelease, "JoystickEvent", Joystick)
            TButtonRelease(): Base(math::vec2(0, 0)) {}
            TButtonRelease(cref) = default;
        };
    };

}

namespace w4::platform {

class IGestureFilter
{
public:
    virtual ~IGestureFilter() {}

    virtual void process(const event::Touch::Begin&) {}
    virtual void process(const event::Touch::Move&) {}
    virtual void process(const event::Touch::End&) {}
    virtual void update(float dt) {}
};

class GestureRecognizer
{
public:
    GestureRecognizer();
    ~GestureRecognizer();

    void addGesture(const sptr<IGestureFilter>& gesture);
private:
    void onTouchBegin(const event::Touch::Begin& evt);
    void onTouchMove(const event::Touch::Move& evt);
    void onTouchEnd(const event::Touch::End& evt);
    void update(float dt);

    std::vector<sptr<IGestureFilter>> m_filters;
    w4::event::Touch::Begin::Handle::sptr tHdl1;
    w4::event::Touch::Move::Handle::sptr tHdl2;
    w4::event::Touch::End::Handle::sptr tHdl3;
};

class TapGestureFilter : public IGestureFilter
{
public:
    void process(const event::Touch::Begin&) override;
    void process(const event::Touch::End&) override;
    void update(float dt) override;
private:
    math::point  begin;
    float        maxDistance = 10;
    float        timeout = 0.2f;
    float        time = 0.f;
};

class SwipeGestureFilter : public IGestureFilter
{
public:
    void process(const event::Touch::Begin&) override;
    void process(const event::Touch::End&) override;
    void update(float dt) override;
private:
    math::point begin;
    float        timeout = 0.4f;
    float        time = 0.f;
    uint        manhattanDist = 7;
};

class Input
{
public:
    class Gamepad
    {
    public:
        struct Info
        {
            uint buttons;
            uint axes;
        };

        using sptr = w4::sptr<Gamepad>;
        using wptr = w4::wptr<Gamepad>;

        Gamepad();

        const Info getInfo() const;

        bool getButton(size_t btn) const;
        float getAxis(size_t axis) const;

        void update(float dt);

        core::OuterID::ndxType getGamepadDataPod();

    private:
        w4::sptr<GAMEPAD_DATA_Holder> m_gamepadDataPod;
        bool m_lastButtonsState[MAX_GAMEPAD_BUTTONS] = {false};
        float m_lastAxesState[MAX_GAMEPAD_AXES] = {0};
        bool m_isResetted[MAX_GAMEPAD_AXES] = {true};

        void onGamepadAxis(uint axisIndex, float pos);
        void onGamepadButton(uint btnIndex, bool val);
    };

    static inline Gamepad& gamepad()
    {
        static Gamepad gamepad;
        return gamepad;
    }

    enum class VirtualJoystickType
    {
        LEFT=0,
        RIGHT=1
    };

    static void createVirtualJoystick(VirtualJoystickType joystick, const math::vec2& position, float radius, float stickRadius, float zeroRadius,
            w4::cref<resources::Texture> joystickTexture, w4::cref<resources::Texture> stickTexture);
    static void setThumbstickPos(VirtualJoystickType joystick, const math::vec2& direction);
    static void setPosition(VirtualJoystickType joystick, const math::vec2& direction);
    static void destroyVirtualJoystick(VirtualJoystickType joystick);
    static void showVirtualJoystick(VirtualJoystickType joystick);
    static void hideVirtualJoystick(VirtualJoystickType joystick);

    static void createVirtualButton(w4::event::Keyboard::Key key,
                                    const math::vec2& position,
                                    const math::vec2& size,
                                    w4::cref<resources::Texture> unpressedTexture,
                                    w4::cref<resources::Texture> pressedTexture,
                                    w4::cref<resources::Texture> disabledTexture = nullptr,
                                    w4::cref<resources::MaterialInst> matInst = nullptr);

    static void destroyVirtualButton(w4::event::Keyboard::Key key);
    static void enableVirtualButton(w4::event::Keyboard::Key key);
    static void disableVirtualButton(w4::event::Keyboard::Key key);
    static void toggleVirtualButton(w4::event::Keyboard::Key key);
    static void hideVirtualButton(w4::event::Keyboard::Key key);
    static void showVirtualButton(w4::event::Keyboard::Key key);

    static void setVirtualButtonMaterialInst(w4::event::Keyboard::Key key, w4::cref<w4::resources::MaterialInst> mat);
    static w4::cref<w4::resources::MaterialInst> getVirtualButtonMaterialInst(w4::event::Keyboard::Key key);

    static GestureRecognizer& getGestureRecognizer();

    static bool update(float dt);
private:
    static void onTouch(int, int, int, int);
    static void onKeyboard(int, int, uint);

    static inline GestureRecognizer m_gestureRecognizer;

    static std::vector<float> axes;

    class VirtualJoystick
    {
    public:
        VirtualJoystick(
            VirtualJoystickType type,
            const math::vec2& position,
            float radius,
            float stickRadius,
            float zeroRadius,
            w4::cref<resources::Texture> joystickTexture,
            w4::cref<resources::Texture> stickTexture);

        ~VirtualJoystick();

        VirtualJoystick(const VirtualJoystick&) = delete;
        VirtualJoystick(VirtualJoystick&&) = delete;

        void setThumbstickPos(const math::vec2& dir);
        void setPosition(const math::vec2& dir);
        void setVisible(bool);
        bool isVisible();
    private:
        enum class JoystickMode
        {
            None,
            Press,
            Move
        }m_joystickMode = JoystickMode::None;
        int m_touchId = 0;

        VirtualJoystickType  m_type;
        math::vec2 m_position;

        float m_jRadius;
        float m_sRadius;
        float m_zRadius;
        float m_aspect;
        float m_tcepsa;

        w4::sptr<w4::render::Mesh> m_backPlane;
        w4::sptr<w4::render::Mesh> m_stickPlane;

        w4::event::Touch::Begin::Handle::sptr m_touchBeginHandler;
        w4::event::Touch::Move::Handle::sptr m_touchMoveHandler;
        w4::event::Touch::End::Handle::sptr m_touchEndHandler;

        w4::event::Resize::Handle::sptr  m_resizeHandler;

        void onResize();

        math::vec2 screenSpaceToJoystick(const math::point& point) const;

        void fireThumbstickPress();
        void fireThumbstickRelease();
        void fireThumbstickMove(const math::vec2& direction);

    };

    class VirtualButton
    {
    public:
        enum class State
        {
            Pressed,
            Unpressed,
            Disabled
        };
        VirtualButton(w4::event::Keyboard::Key key,
            const math::vec2& position,
            const math::vec2& size,
            w4::cref<resources::Texture> unpressedTexture,
            w4::cref<resources::Texture> pressedTexture,
            w4::cref<resources::Texture> disabledTexture = nullptr,
            w4::cref<resources::MaterialInst> matInst = nullptr);
       ~VirtualButton();
        VirtualButton(const VirtualButton&) = delete;
        VirtualButton(VirtualButton&&);

        inline State getState() const {return m_buttonState;}
        void setState(const State state);
        void toggle();
        void setMaterialInst(w4::cref<resources::MaterialInst>& matInst);
        w4::cref<resources::MaterialInst> getMaterialInst() const;
        void setVisible(bool);
        bool isVisible();

    private:
        int m_touchId = 0;
        State m_buttonState = State::Unpressed;

        w4::event::Keyboard::Key m_key;
        math::vec2 m_position;

        math::vec2  m_size;

        w4::sptr<w4::render::Mesh> m_buttonPlane;

        w4::math::point m_minPoint;
        w4::math::point m_maxPoint;

        std::unordered_map<State, w4::sptr<resources::Texture>> m_textures;

        w4::event::Touch::Begin::Handle::sptr m_touchBeginHandler;
        w4::event::Touch::End::Handle::sptr m_touchEndHandler;
        w4::event::Resize::Handle::sptr  m_resizeHandler;
        void onResize(w4::event::Resize::cref evt);

        void fireButtonPress();
        void fireButtonRelease();
    };

    static inline w4::sptr<VirtualJoystick> m_virtualJoysticks[2];
    static inline std::unordered_map<w4::event::Keyboard::Key, w4::sptr<VirtualButton>> m_virtualButtons;

    friend class Platform;
};

}
