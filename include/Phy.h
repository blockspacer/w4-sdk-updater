#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>
#include <optional>

#include "W4Logger.h"
#include "W4Math.h"
#include "Object.h"
#include "FatalError.h"

class neSimulator;
class neAnimatedBody;
class neRigidBody;
class neGeometry;
struct neCollisionInfo;
class neJoint;
class PhysicsTransform;
typedef PhysicsTransform neT3;
class CTokSpring;
struct SFixationInfo;

namespace w4::physics {

w4::math::vec3 calculateBoxInertiaTensor(float width, float height, float depth, float mass);
w4::math::vec3 calculateBoxInertiaTensor(const w4::math::vec3& boxSize, float mass);
w4::math::vec3 calculateSphereInertiaTensor(float diameter, float mass);
w4::math::vec3 calculateCapsuleInertiaTensor(float diameter, float height, float mass);

class Body;
struct IBodyImpl;

class Simulator: public w4::core::Object
{
    W4_OBJECT(Simulator, w4::core::Object);

    friend class RigidBodyImpl;
    friend class AnimatedBodyImpl;
    friend class Joint;
public:
    using CollisionCallback = std::function<void(Body&, Body&)>;

    Simulator(size_t rigidBodiesCount,
              size_t animatedBodiesCount,
              size_t constraintCount,
              const w4::math::vec3& gravity, size_t nStepsPerSecond = 45);

    void onUpdate(float dt) override;

    void setCollisionCallback(const std::vector<w4::sptr<Body>>& obj1,
                              const std::vector<w4::sptr<Body>>& obj2,
                              bool rebound,
                              CollisionCallback);

    void removeFromCollidingTable(w4::cref<Body> obj);

    void registerBody(const w4::sptr<Body>& obj);
    void unregisterBody(const w4::sptr<Body>& obj);

    void setMaterial(int materialIdCounter, float friction, float restitution);
private:
    std::unordered_map<std::pair<int, int>, CollisionCallback> m_collisionCallbacks;
    std::unordered_map<int, w4::sptr<Body>> m_bodies;

    std::unordered_map<w4::sptr<Body>, w4::sptr<Body>> m_hardLinkProxies;
    std::unordered_map<std::pair<void*, neGeometry*>, w4::sptr<Body>> m_bodyGeometryDict;

    std::unique_ptr<neSimulator, std::function<void(neSimulator *)>> m_phisSim;

    void collisionCallback(neCollisionInfo &info);
    void breakageCallback(void* originalBody, neGeometry* brokenGeometry, neRigidBody* newBody);

    void hardLink(w4::cref<Body> master, w4::cref<Body> slave);
    w4::sptr<Body> registerMultiplexed(void* multiplexed, std::vector<neGeometry*> geometries, const w4::sptr<Body>& body);
    void unregisterMultiplexed(void* multiplexed, std::vector<neGeometry*> geometries);
    w4::sptr<Body> findMultiplexed(void* multiplexed, neGeometry* geometry);

    neRigidBody* createRigidBody(bool isParticle = false);
    neAnimatedBody* createAnimatedBody();
    void destroyBody(neAnimatedBody* body);
    void destroyBody(neRigidBody* body);

public:
    neJoint* createJoint(neRigidBody* body1, neRigidBody* body2);
    neJoint* createJoint(neRigidBody* body1, neAnimatedBody* body2);
    void destroyJoint(neJoint *joint);

private:
    size_t m_nStepsPerSecond;
};

class PhysicsMaterial: public w4::core::Object
{
public:
    W4_OBJECT(PhysicsMaterial, w4::core::Object)

    PhysicsMaterial(float friction, float restitution); //0.5, 0.4 - default values
    int getId(Simulator& physics) const;
private:
    float m_friction = .5f;
    float m_restitution = .4f;
    int m_materialId;
    static int materialIdCounter;
};

class SimulatorCollector
{
public:
    static void onUpdate(float dt);
    static void addSimulator(w4::cref<Simulator> sim);
    static void removeSimulator(w4::cref<Simulator> sim);
private:
    static std::vector<w4::sptr<Simulator>> m_simulators;
};

class JointEndpoint
{
public:
    using sptr = w4::sptr<JointEndpoint>;
    using cref = const w4::sptr<JointEndpoint>&;

    enum class BodyState{
        RIGID,
        ANIMATED,
        UNRELATED
    };

    using StateCallback = std::function<void(BodyState)>;

    JointEndpoint(Body& body, const StateCallback& stateCb);
    void onBodyStateChanged(BodyState state);
    void onJointDestroyed();

    neAnimatedBody* getAnimatedBody() const;
    neRigidBody* getRigidBody() const;

    BodyState getState() const;

private:
    std::function<void(BodyState)> m_stateCb;
    Body* m_pBody = nullptr;
    BodyState m_lastState = BodyState::UNRELATED;
};

class GeometryBase;

struct IBodyImpl
{
    IBodyImpl(int collisionId): m_collisionId(collisionId) {}
    virtual ~IBodyImpl() = default;

    virtual void createBody(Simulator* simulator) = 0;
    virtual void destroyBody() = 0;

    virtual void setPosition(const w4::math::vec3& position) = 0;
    virtual void setRotation(const w4::math::Rotator& rotation) = 0;
    virtual const w4::math::vec3& getPosition() const = 0;
    virtual const w4::math::Rotator getRotation() const = 0;
    virtual float getMass() const = 0;
    virtual void setMass(float mass) = 0;
    virtual void setInertiaTensor(const w4::math::vec3& tensor) = 0;
    virtual float getLinearDamping() const = 0;
    virtual void setLinearDamping(float damp) = 0;
    virtual float getAngularDamping() const = 0;
    virtual void setAngularDamping(float damp) = 0;

    virtual void setSleepingParameter(float sleepingParam) = 0;
    virtual float getSleepingParameter() const = 0;

    virtual w4::math::vec3 getVelocity() const = 0;
    virtual void setVelocity(const w4::math::vec3& v) = 0;

    virtual w4::math::vec3 getAngularVelocity() const = 0;
    virtual w4::math::vec3 getAngularMomentum() const = 0;
    virtual void setAngularMomentum(const w4::math::vec3& am) = 0;
    virtual w4::math::vec3 getVelocityAtPoint(const w4::math::vec3& point) = 0;

    virtual void setForce(const w4::math::vec3& force) = 0;
    virtual void setTorque(const w4::math::vec3& torque) = 0;
    virtual void setForce(const w4::math::vec3& force, const w4::math::vec3& pos) = 0;
    virtual w4::math::vec3 getForce() const = 0;
    virtual w4::math::vec3 getTorque() const = 0;

    virtual void applyImpulse(const w4::math::vec3& impulse) = 0;
    virtual void applyImpulse(const w4::math::vec3& impulse, const w4::math::vec3& pos) = 0;
    virtual void applyTwist(const w4::math::vec3& twist) = 0;

    virtual void gravityEnable(bool yes) = 0;
    virtual bool isGravityEnabled() const = 0;

    virtual size_t getUserData() const = 0;
    virtual void setUserData(size_t userData) = 0;
    virtual int getCollisionId() const = 0;
    virtual void setCollisionId(int collisionId) = 0;

    virtual void updateBoundingInfo() = 0;
    virtual neGeometry* addGeometry() = 0;
    virtual void removeGeometry(neGeometry* geometry) = 0;
    virtual neAnimatedBody* getAnimatedBody() const = 0;
    virtual neRigidBody* getRigidBody() const = 0;
    virtual w4::sptr<Body> getRealBody() const = 0;

protected:
    Simulator* m_pPhysSimulator = nullptr;
    int m_collisionId = 0;
};

class RigidBodyImpl: public IBodyImpl
{
public:
    RigidBodyImpl(int collisionId);
    ~RigidBodyImpl();
    void createBody(Simulator* simulator) override;
    void destroyBody() override;
    void setPosition(const w4::math::vec3& position) override;
    void setRotation(const w4::math::Rotator& rotation) override;
    const w4::math::vec3& getPosition() const override;
    const w4::math::Rotator getRotation() const override;
    float getMass() const override;
    void setMass(float mass) override;
    void setInertiaTensor(const w4::math::vec3& tensor) override;
    float getLinearDamping() const override;
    void setLinearDamping(float damp) override;
    float getAngularDamping() const override;
    void setAngularDamping(float damp) override;
    void setSleepingParameter(float sleepingParam) override;
    float getSleepingParameter() const override;
    w4::math::vec3 getVelocity() const override;
    void setVelocity(const w4::math::vec3& v) override;
    w4::math::vec3 getAngularVelocity() const override;
    w4::math::vec3 getAngularMomentum() const override;
    void setAngularMomentum(const w4::math::vec3& am) override;
    w4::math::vec3 getVelocityAtPoint(const w4::math::vec3& point) override;
    void setForce(const w4::math::vec3& force) override;
    void setTorque(const w4::math::vec3& torque) override;
    void setForce(const w4::math::vec3& force, const w4::math::vec3& pos) override;
    w4::math::vec3 getForce() const override;
    w4::math::vec3 getTorque() const override;
    void applyImpulse(const w4::math::vec3& impulse) override;
    void applyImpulse(const w4::math::vec3& impulse, const w4::math::vec3& pos) override;
    void applyTwist(const w4::math::vec3& twist) override;
    void gravityEnable(bool yes) override;
    bool isGravityEnabled() const override;
    size_t getUserData() const override;
    void setUserData(size_t userData) override;
    int getCollisionId() const override;
    void setCollisionId(int collisionId) override;
    void updateBoundingInfo() override;
    neGeometry* addGeometry() override;
    void removeGeometry(neGeometry* geometry) override;
    neAnimatedBody* getAnimatedBody() const override;
    neRigidBody* getRigidBody() const override;
    w4::sptr<Body> getRealBody() const override;
private:
    neRigidBody* m_rigidBody = nullptr;
};

class AnimatedBodyImpl: public IBodyImpl
{
public:
    AnimatedBodyImpl(int collisionId);
    ~AnimatedBodyImpl();
    void createBody(Simulator* simulator) override;
    void destroyBody() override;
    void setPosition(const w4::math::vec3& position) override;
    void setRotation(const w4::math::Rotator& rotation) override;
    const w4::math::vec3& getPosition() const override;
    const w4::math::Rotator getRotation() const override;
    float getMass() const override;
    void setMass(float mass) override;
    void setInertiaTensor(const w4::math::vec3& tensor) override;
    float getLinearDamping() const override;
    void setLinearDamping(float damp) override;
    float getAngularDamping() const override;
    void setAngularDamping(float damp) override;
    void setSleepingParameter(float sleepingParam) override;
    float getSleepingParameter() const override;
    w4::math::vec3 getVelocity() const override;
    void setVelocity(const w4::math::vec3& v) override;
    w4::math::vec3 getAngularVelocity() const override;
    w4::math::vec3 getAngularMomentum() const override;
    void setAngularMomentum(const w4::math::vec3& am) override;
    w4::math::vec3 getVelocityAtPoint(const w4::math::vec3& point) override;
    void setForce(const w4::math::vec3& force) override;
    void setTorque(const w4::math::vec3& torque) override;
    void setForce(const w4::math::vec3& force, const w4::math::vec3& pos) override;
    w4::math::vec3 getForce() const override;
    w4::math::vec3 getTorque() const override;
    void applyImpulse(const w4::math::vec3& impulse) override;
    void applyImpulse(const w4::math::vec3& impulse, const w4::math::vec3& pos) override;
    void applyTwist(const w4::math::vec3& twist) override;
    void gravityEnable(bool yes) override;
    bool isGravityEnabled() const override;
    size_t getUserData() const override;
    void setUserData(size_t userData) override;
    int getCollisionId() const override;
    void setCollisionId(int collisionId) override;
    void updateBoundingInfo() override;
    neGeometry* addGeometry() override;
    void removeGeometry(neGeometry* geometry) override;
    neAnimatedBody* getAnimatedBody() const override;
    neRigidBody* getRigidBody() const override;
    w4::sptr<Body> getRealBody() const override;
private:
    struct
    {
        w4::math::vec3 angularMoment = {0, 0, 0};
        float mass = 1.f;
        float angularDamping = 0.f;
        w4::math::vec3 force = {0, 0, 0};
        float sleepingParameter = .2f;
        float linearDamping = 0.f;
        w4::math::vec3 torque = {0, 0, 0};
        bool gravityEnabled = true;
    }m_rigidDataStorage;

    neAnimatedBody* m_animatedBody = nullptr;
};

class ProxyBodyImpl: public IBodyImpl
{
public:
    ProxyBodyImpl(int collisionId);
    ~ProxyBodyImpl();
    void createBody(Simulator* simulator) override;
    void destroyBody() override;
    void setPosition(const w4::math::vec3& position) override;
    void setRotation(const w4::math::Rotator& rotation) override;
    const w4::math::vec3& getPosition() const override;
    const w4::math::Rotator getRotation() const override;
    float getMass() const override;
    void setMass(float mass) override;
    void setInertiaTensor(const w4::math::vec3& tensor) override;
    float getLinearDamping() const override;
    void setLinearDamping(float damp) override;
    float getAngularDamping() const override;
    void setAngularDamping(float damp) override;
    void setSleepingParameter(float sleepingParam) override;
    float getSleepingParameter() const override;
    w4::math::vec3 getVelocity() const override;
    void setVelocity(const w4::math::vec3& v) override;
    w4::math::vec3 getAngularVelocity() const override;
    w4::math::vec3 getAngularMomentum() const override;
    void setAngularMomentum(const w4::math::vec3& am) override;
    w4::math::vec3 getVelocityAtPoint(const w4::math::vec3& point) override;
    void setForce(const w4::math::vec3& force) override;
    void setTorque(const w4::math::vec3& torque) override;
    void setForce(const w4::math::vec3& force, const w4::math::vec3& pos) override;
    w4::math::vec3 getForce() const override;
    w4::math::vec3 getTorque() const override;
    void applyImpulse(const w4::math::vec3& impulse) override;
    void applyImpulse(const w4::math::vec3& impulse, const w4::math::vec3& pos) override;
    void applyTwist(const w4::math::vec3& twist) override;
    void gravityEnable(bool yes) override;
    bool isGravityEnabled() const override;
    size_t getUserData() const override;
    void setUserData(size_t userData) override;
    int getCollisionId() const override;
    void setCollisionId(int collisionId) override;
    void updateBoundingInfo() override;
    neGeometry* addGeometry() override;
    void removeGeometry(neGeometry* geometry) override;
    neAnimatedBody* getAnimatedBody() const override;
    neRigidBody* getRigidBody() const override;
    w4::sptr<Body> getRealBody() const override;
private:
    int m_collisionId = 0;
    struct
    {
        w4::math::vec3 positionOffset = {0, 0, 0};
        w4::math::Rotator rotationOffset;
        float mass = 1.f;
        w4::math::vec3 inertialTensor = {1.f, 1.f, 1.f};
        size_t userData = 0;
    }m_dataStorage;

    mutable struct
    {
        w4::math::vec3 position;
    }m_dataCache;

    w4::sptr<Body> m_realBody;
};

class Body: public w4::enable_from_this<Body>, public core::Object
{
    W4_OBJECT(Body, core::Object);

    friend class Simulator;
    friend class GeometryBase;
    friend class Joint;
    friend class JointEndpoint;
    friend class ProxyBodyImpl;

public:

    enum class BodyType
    {
        Rigid,
        Animated,
        Proxy
    };

    Body(BodyType rbType);
    virtual ~Body();

    int getCollisionId();

    void setPosition(const w4::math::vec3& position);
    void setRotation(const w4::math::Rotator& rotation);
    void setScale(const w4::math::vec3& scale);
    const w4::math::vec3 getPosition() const;
    const w4::math::Rotator getRotation() const;
    const w4::math::vec3& getScale() const;
    float getMass() const;
    void setMass(float mass);
    void setInertiaTensor(const w4::math::vec3& tensor);
    float getLinearDamping() const;
    void setLinearDamping(float damp);
    float getAngularDamping() const;
    void setAngularDamping(float damp);

    void setSleepingParameter(float sleepingParam);
    float getSleepingParameter() const;

    w4::math::vec3 getVelocity() const;
    void setVelocity(const w4::math::vec3& v);

    w4::math::vec3 getAngularVelocity() const;
    w4::math::vec3 getAngularMomentum() const;
    void setAngularMomentum(const w4::math::vec3& am);
    w4::math::vec3 getVelocityAtPoint(const w4::math::vec3& point);

    void setForce(const w4::math::vec3& force);
    void setTorque(const w4::math::vec3& torque);
    void setForce(const w4::math::vec3& force, const w4::math::vec3& pos);
    w4::math::vec3 getForce() const;
    w4::math::vec3 getTorque() const;

    void applyImpulse(const w4::math::vec3& impulse);
    void applyImpulse(const w4::math::vec3& impulse, const w4::math::vec3& pos);
    void applyTwist(const w4::math::vec3& twist);

    void gravityEnable(bool yes);
    bool isGravityEnabled() const;

    size_t getUserData() const;
    void setUserData(size_t userData);

    const w4::math::vec3& getInertiaTensor() const;

    BodyType getType() const;

    void enablePhysics();
    void disablePhysics();
    bool isPhysicsEnabled() const;

    const std::unordered_set<w4::sptr<GeometryBase>>& getAllGeometries() const;
    void addGeometry(const w4::sptr<GeometryBase>& geometry);
    void removeGeometry(const w4::sptr<GeometryBase>& geometry);

    void setMaterial(w4::cref<PhysicsMaterial> mat);

    Simulator* getPhysicsSimulator();

private:
    BodyType m_type;
    std::unique_ptr<IBodyImpl> m_impl;

    w4::math::vec3 m_scale = {1, 1, 1};

    Simulator* m_pPhysSimulator = nullptr;

    w4::math::vec3 m_inertialTensor = {1.f, 1.f, 1.f}; //default mass

    struct
    {
        w4::math::vec3 position = {0, 0, 0};
        w4::math::Rotator rotation;
        w4::math::vec3 velocity = {0, 0, 0};
        w4::math::vec3 angularMoment = {0, 0, 0};
        float mass = 1.f;
        float angularDamping = 0.f;
        w4::math::vec3 force = {0, 0, 0};
        float sleepingParameter = .2f;
        float linearDamping = 0.f;
        w4::math::vec3 torque = {0, 0, 0};
        bool gravityEnabled = true;
    }m_tempDataStorage;

    bool m_isDirty = false;

    std::unordered_set<w4::sptr<GeometryBase>> m_geometries;

    int m_collisionId = 0;
    static int collidersIdCounter;

    void createBody(Simulator* simulator);
    void destroyBody();

    void updateBoundingInfo();
    void setDirtyFlag();

    virtual void doPostProcess(float dt) {}

    std::vector<std::function<void()>> m_initStack;

    void backupToStack();
    void restoreFromStack();

    std::unordered_set<const JointEndpoint*> m_jointEndpoints;
    w4::sptr<JointEndpoint> createJointEndpoint(const JointEndpoint::StateCallback& stateCb);
    void removeJointEndpoint(const JointEndpoint* const);

    neAnimatedBody* getAnimatedBody() const;
    neRigidBody* getRigidBody() const;
    w4::sptr<Body> getRealBody() const;

    void setType(BodyType bodyType);

    void setupAsRigidBody(neRigidBody* newBody, neGeometry* brokenGeometry);

    void merge(w4::cref<Body> body);
    void suckIn(w4::cref<Body> body);
};

enum class BreakageMode
{
    Disabled,
    Normal,
    All,
    Neighbour
};

struct BreakInfo
{
    w4::math::vec3 inertiaTensor = {2.f/3, 2.f/3, 2.f/3}; //inertial tensor of box 1x1x1 mass 1
    w4::math::vec3 breakPlane = {0, 0, 0};
    float mass = 1.f;
    float breakMagnitude = 0;
    float breakAbsorb = .5f;
    float neighbourRadius = 0;
    BreakageMode mode = BreakageMode::Disabled;
};

class GeometryBase: public w4::core::Object
{
    friend class Body;
public:
    W4_OBJECT(GeometryBase, w4::core::Object);

    GeometryBase();
    virtual ~GeometryBase() = 0;

    void setPosition(const w4::math::vec3& pos);
    void setRotation(const w4::math::Rotator& rot);
    void setMaterial(w4::cref<PhysicsMaterial> mat);
    const w4::math::vec3& getPosition() const;
    const w4::math::Rotator getRotation() const;
    void setUserData(size_t userData);
    size_t getUserData();
    void setBreakageInfo(const BreakInfo& breakinfo);
    const BreakInfo& getBreakInfo() const;

protected:
    Body* m_pBody = nullptr;
    neGeometry* m_geometry = nullptr;

    math::vec3 getRotatedScale(const math::vec3& scale) const;
private:
    std::optional<w4::math::vec3> m_pos;
    std::optional<w4::math::Rotator> m_rot;
    w4::sptr<PhysicsMaterial> m_material;
    std::optional<size_t> m_userData;
    BreakInfo m_breakageInfo;

    void link(Body* pBody, neGeometry* geometry, const w4::math::vec3& scale);
    neGeometry* unlink();

    virtual void setGeometryParams(const w4::math::vec3& scale) = 0;
};

class CapsuleGeometry: public GeometryBase
{
public:
    W4_OBJECT(CapsuleGeometry, GeometryBase);

    CapsuleGeometry(float diameter, float height);
    float getDiameter() const;
    float getHeight() const;
private:
    float m_diameter = 0;
    float m_height = 0;

    void setGeometryParams(const w4::math::vec3& scale) override;
};

class CubeGeometry: public GeometryBase
{
public:
    W4_OBJECT(CubeGeometry, GeometryBase);

    CubeGeometry(float width, float height, float depth);
    CubeGeometry(const w4::math::vec3& size);
    w4::math::vec3 getSize() const;
private:
    w4::math::vec3 m_size;

    void setGeometryParams(const w4::math::vec3& scale) override;
};

class SphereGeometry: public GeometryBase
{
public:
    W4_OBJECT(SphereGeometry, GeometryBase);

    SphereGeometry(float diameter);
    float getDiameter() const;
private:
    float m_diameter;

    void setGeometryParams(const w4::math::vec3& scale) override;
};

class ConvexGeometry: public GeometryBase
{
public:
    W4_OBJECT(ConvexGeometry, GeometryBase);

    ConvexGeometry(const std::vector<uint8_t>& convexData);
    const std::vector<uint8_t>& getConvexData() const;
private:
    std::vector<uint8_t> m_convexData;

    void setGeometryParams(const w4::math::vec3& scale) override;
};

class Joint
{
public:
    using sptr = w4::sptr<Joint>;
    using cref = const w4::sptr<Joint>&;

    virtual ~Joint();

    enum class JointType
    {
        Hard,
        HardDestroyableJoint,
        Spring,
        BallSocket,
        Hinge,
        Slide
    };

    enum class JointState
    {
        Disabled,
        Rigid2Rigid,
        Rigid2Animated
    };
    JointType getType() const;
    void enable(bool onOff);
    bool isEnabled() const;
protected:
    Joint(w4::cref<Simulator> physSimulator, JointType linkType, w4::cref<Body> node1, w4::cref<Body> node2);

    w4::sptr<Simulator> m_phisSim;
    w4::sptr<JointEndpoint> m_endpoints[2];
private:
    JointType m_type;
    bool m_isEnabled = false;
    JointState m_currentState = JointState::Disabled;
//    virtual void onUpdate(float dt) = 0;
    void onEndpointStateChanged();
    virtual void onJointTypeChanged(JointState state) = 0;
};

class HardJoint;
class HardJointBreakable;

class InternalJointMultiProxy
{
public:
    using sptr = w4::sptr<InternalJointMultiProxy>;
/*
struct BreakageSettings
{
    float absorption;
//        callback//void(neByte * originalBody, neBodyType bodyType, neGeometry * brokenGeometry, neRigidBody * newBody)
    neGeometry::neBreakFlag flag;
    w4::math::vec3 inertiaTensor;
    float magnitude;
    float mass;
    float neighbourRadius;
    w4::math::vec3 plane;
};
*/
public:
    void registerJoint(Joint::cref link);
    void unregisterJoint(Joint::cref link);

private:
    std::vector<HardJoint*> m_hardJoints;
    std::vector<HardJointBreakable*> m_hardJointsBreakable;

    std::unordered_map<GeometryBase*, HardJoint*> m_geometryMap;

    w4::sptr<Body> m_realBody;
};

class HardJoint: public Joint
{
public:
    HardJoint(w4::cref<Simulator> physSimulator, w4::cref<Body> node1,  w4::cref<Body> node2);
private:
    InternalJointMultiProxy::sptr m_multyProxy;
    void onJointTypeChanged(JointState state) override;
    w4::cref<Body> m_body1;
    w4::cref<Body> m_body2;
};

class HardBreakableJoint: public Joint
{
public:
    HardBreakableJoint(w4::cref<Simulator> physSimulator,  w4::cref<Body> node1,  w4::cref<Body> node2);
private:
    InternalJointMultiProxy::sptr m_multyProxy;
    void onJointTypeChanged(JointState state) override;
};

class SpringJoint: public Joint
{
public:
    SpringJoint(w4::cref<Simulator> Simulator,
                w4::cref<Body> node1,
                w4::cref<Body> node2,
                float length,
                const w4::math::vec3& anchorPointOffset1 = {0, 0, 0},
                const w4::math::vec3& anchorPointOffset2 = {0, 0, 0},
                float springTension = 1/40.f,
                float dampingFactor = .0001f,
                float tangentForceFactor = 15.f, //required only if spring is fixed
                float tangentDamping = 1.5f,     //required only if spring is fixed
                float torqueFactor = 500.f,      //required only if spring is fixed
                float angularDamping = 10.f      //required only if spring is fixed
                );
    void fixSpringConnection1(const w4::math::vec3& springDirection);
    void unfixSpringConnection1();
    void fixSpringConnection2(const w4::math::vec3& springDirection);
    void unfixSpringConnection2();

    float getCurrentLength() const;

private:
    std::shared_ptr<SFixationInfo> m_fixationInfo;
    std::shared_ptr<CTokSpring> m_tokSpring;
    w4::math::vec3 m_offset1;
    w4::math::vec3 m_offset2;
    float m_length;
    float m_springTension;
    float m_dampingFactor;

    void onJointTypeChanged(JointState state) override;
};

class BallSocketJoint: public Joint
{
public:
    BallSocketJoint(w4::cref<Simulator> simulator, w4::cref<Body> node1, w4::cref<Body> node2, const w4::math::vec3& position, float dampingFactor = 0.f, std::optional<w4::math::vec2> limits = std::nullopt, std::optional<w4::math::vec2> twistLimits = std::nullopt);

private:
    std::unique_ptr<neJoint, std::function<void(neJoint*)>> m_joint;

    w4::math::vec3 m_position;
    float m_dampingFactor = 0;
    std::optional<w4::math::vec2> m_limits;
    std::optional<w4::math::vec2> m_twistLimits;

    void onJointTypeChanged(JointState state) override;
};

class HingeJoint: public Joint
{
public:
    HingeJoint(w4::cref<Simulator> simulator, w4::cref<Body> node1, w4::cref<Body> node2, const w4::math::vec3& position, const w4::math::vec3& axis, float dampingFactor, std::optional<w4::math::vec2> limits = std::nullopt);
    void setMotor(float desiredVelocity, float maxForce);
    void enableMotor(bool yes);
    bool isMotorEnabled() const;
private:
    std::unique_ptr<neJoint, std::function<void(neJoint*)>> m_joint;

    w4::math::vec3 m_position;
    w4::math::vec3 m_axis;
    float m_dampingFactor = 0;
    std::optional<w4::math::vec2> m_limits;

    bool m_motorEnabled = false;
    float m_motorDesiredVelocity = 0;
    float m_motorMaxForce = 0;

    void onJointTypeChanged(JointState state) override;
};

class SlideJoint: public Joint
{
public:
    SlideJoint(w4::cref<Simulator> simulator, w4::cref<Body> node1, w4::cref<Body> node2, float dampingFactor = 0.f, std::optional<w4::math::vec2> limits = std::nullopt);
    void setMotor(float desiredVelocity, float maxForce);
    void enableMotor(bool yes);
    bool isMotorEnabled() const;
private:
    std::unique_ptr<neJoint, std::function<void(neJoint*)>> m_joint;

    float m_dampingFactor = 0;
    std::optional<w4::math::vec2> m_limits;

    bool m_motorEnabled = false;
    float m_motorDesiredVelocity = 0;
    float m_motorMaxForce = 0;

    void onJointTypeChanged(JointState state) override;
};

} //w4::physics
