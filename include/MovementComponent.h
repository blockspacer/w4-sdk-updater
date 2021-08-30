#pragma once

#if 0

#include "Component.h"
#include "W4Math.h"

namespace w4::render {

class MovementComponent final: public core::IComponent
{
    W4_COMPONENT(MovementComponent, core::IComponent);

public:
    enum class State:  uint32_t
    {
        None                = 0x00000000,
        Idle                = 0x00000001,
        MoveForward         = 0x00000002,
        MoveBackward        = 0x00000004,
        MoveUp              = 0x00000008,
        MoveDown            = 0x00000010,
        MoveLeft            = 0x00000020,
        MoveRight           = 0x00000040,

        CrouchIdle          = 0x00000080,
        CrouchForward       = 0x00000100,
        CrouchBackward      = 0x00000200,
        CrouchUp            = 0x00000400,
        CrouchDown          = 0x00000800,
        CrouchLeft          = 0x00001000,
        CrouchRight         = 0x00002000,

        Jump                = 0x00004000,
        Roll                = 0x00008000,
        Pitch               = 0x00010000,
        Yaw                 = 0x00020000
    };

    struct Settings
    {
        struct
        {
            float forward = 10.f; //units per second
            float backward = 5.f; //units per second
            float left = 7.5f; //units per second
            float right = 7.5f; //units per second
            float up = 5.f; //units per second
            float down = 5.f; //units per second

            float roll = w4::math::HALF_PI; //radians per second
            float pitch = w4::math::HALF_PI; //radians per second
            float yaw = w4::math::HALF_PI; //radians per second
        } speed;
        float crouchSpeedMultiplier = 0.5f;
    };

    using StateHandle = size_t;
    using StateCallback = std::function<void(State)>;

    using ColliderHandle = size_t;
    //using ColliderCallback = std::function<void(Collider::cref)>;

public:
    static State combineState(State s1, State s2); //s1 | s2
    static State combineState(State s1, State s2, State s3); //s1 | s2 | s3
    static State combineState(State s1, State s2, State s3, State s4); //s1 | s2 | s3 | s4
    static bool checkState(State s1, State s2); //s1 & s2

public:
    explicit MovementComponent(core::Node&);
    ~MovementComponent();

    void ctorParameters();

    void preInitialize() override;
    void postInitialize() override;
    void update(float dt) override;

    void moveForward(float power);
    void moveRight(float power);
    void moveUp(float power);
    void roll(float power);
    void pitch(float power);
    void yaw(float power);
    void jump();
    void crouch();

    State getState() const;

    void setSettings(const Settings&);
    const Settings& getSettings() const;

    StateHandle subscribeToState(StateCallback);
    void unsubcribeFromState(StateHandle);

    /*ColliderHandle subscribeToCollide(ColliderCallback);
    void unsubscriberFromCollide(ColliderHandle);*/

private:
    void cloneData(const IComponent&) override;

private:
    class Pimpl;
    uptr<Pimpl> m_d;
};

} //namespace w4::render

#endif