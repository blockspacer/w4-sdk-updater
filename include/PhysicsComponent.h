#pragma once

#include "Component.h"
#include "Phy.h"

namespace w4::physics {

enum class PhysicsGeometry
{
    Custom,
    AutoCapsule,
    AutoCube,
    AutoSphere
};

class PhysicsComponent: public core::IComponent
    {
    W4_COMPONENT(PhysicsComponent, core::IComponent);
    W4_COMPONENT_DISABLE_CLONING
    public:
        inline operator w4::cref<physics::Body>() const {return m_body;};
        void initialize(const variant::Variant& data) override;
        void finalize() override;
        void update(float dt) override;
        void setup(w4::cref<physics::Simulator> simulator, physics::Body::BodyType bodyType, PhysicsGeometry autoGeometryType = PhysicsGeometry::Custom, float mass = 1.f);

        //Body interface
        inline void setPosition(const w4::math::vec3& position) {m_body->setPosition(position);}
        inline void setRotation(const w4::math::Rotator& rotation) {m_body->setRotation(rotation);}
        inline void setScale(const w4::math::vec3& scale) {m_body->setScale(scale);}
        inline const w4::math::vec3 getPosition() const {return m_body->getPosition();}
        inline const w4::math::Rotator getRotation() const {return m_body->getRotation();}
        inline const w4::math::vec3& getScale() const {return m_body->getScale();}
        inline float getMass() const {return m_body->getMass();}
        inline void setMass(float mass) {m_body->setMass(mass);}
        inline void setInertiaTensor(const w4::math::vec3& tensor) {m_body->setInertiaTensor(tensor);}
        inline float getLinearDamping() const {return m_body->getLinearDamping();}
        inline void setLinearDamping(float damp) {m_body->setLinearDamping(damp);}
        inline float getAngularDamping() const {return m_body->getAngularDamping();}
        inline void setAngularDamping(float damp) {m_body->setAngularDamping(damp);}

        inline void setSleepingParameter(float sleepingParam) {m_body->setSleepingParameter(sleepingParam);}
        inline float getSleepingParameter() const {return m_body->getSleepingParameter();}

        inline w4::math::vec3 getVelocity() const {return m_body->getVelocity();}
        inline void setVelocity(const w4::math::vec3& v) {m_body->setVelocity(v);}

        inline w4::math::vec3 getAngularVelocity() const {return m_body->getAngularVelocity();}
        inline w4::math::vec3 getAngularMomentum() const {return m_body->getAngularMomentum();}
        inline void setAngularMomentum(const w4::math::vec3& am) {m_body->setAngularMomentum(am);}
        inline w4::math::vec3 getVelocityAtPoint(const w4::math::vec3& point) {return m_body->getVelocityAtPoint(point);}

        inline void setForce(const w4::math::vec3& force) {m_body->setForce(force);}
        inline void setTorque(const w4::math::vec3& torque) {m_body->setTorque(torque);}
        inline void setForce(const w4::math::vec3& force, const w4::math::vec3& pos) {m_body->setForce(force, pos);}
        inline w4::math::vec3 getForce() const { return m_body->getForce();}
        inline w4::math::vec3 getTorque() const {return m_body->getTorque();}

        inline void applyImpulse(const w4::math::vec3& impulse) {m_body->applyImpulse(impulse);}
        inline void applyImpulse(const w4::math::vec3& impulse, const w4::math::vec3& pos) {m_body->applyImpulse(impulse, pos);}
        inline void applyTwist(const w4::math::vec3& twist) {m_body->applyTwist(twist);}

        inline void gravityEnable(bool yes) {m_body->gravityEnable(yes);}
        inline bool isGravityEnabled() const {return m_body->isGravityEnabled();}

        inline size_t getUserData() const {return m_body->getUserData();}
        inline void setUserData(size_t userData) {m_body->setUserData(userData);}

        inline const w4::math::vec3& getInertiaTensor() const {return m_body->getInertiaTensor();}

        inline physics::Body::BodyType getType() const {return m_body->getType();}

        inline void enablePhysics() {m_body->enablePhysics();}
        inline void disablePhysics() {m_body->disablePhysics();}
        inline bool isPhysicsEnabled() const {return m_body->isPhysicsEnabled();}

        inline void setMaterial(w4::cref<PhysicsMaterial> mat) {m_body->setMaterial(mat);}

        inline const std::unordered_set<w4::sptr<physics::GeometryBase>>& getAllGeometries() const {return m_body->getAllGeometries();}
        void addGeometry(const w4::sptr<physics::GeometryBase>& geometry);
        inline void removeGeometry(const w4::sptr<physics::GeometryBase>& geometry) {m_body->removeGeometry(geometry);}

        void debugViewForeachSurface(std::function<void(core::Surface &)> visitor) override;

    private:
        void onDebugViewEnabled(bool) override;
private:
        w4::sptr<physics::Body> m_body;
        w4::physics::Simulator* m_simulator = nullptr;
        bool m_insideOwnerTransform = false;

        size_t m_transformsCallbackId = 0;
        sptr<core::Node> m_debugView;
    };

} //namespace w4::render
