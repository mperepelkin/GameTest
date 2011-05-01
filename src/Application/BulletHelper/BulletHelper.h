/**
 * Bullet helper declaration.
 */

#ifndef __BULLET_HELPER_H__
#define __BULLET_HELPER_H__

#include "LinearMath/btScalar.h"
#include "LinearMath/btVector3.h"

class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btCollisionConfiguration;
class btDynamicsWorld;
class btRigidBody;

namespace GameTest {

class BulletHelper {
public:

    // Creates the new instance of BulletHelper. Checks being called only once.
    static BulletHelper* createInstance();

    // Creates and initializes new physics world.
    void init();

    // Ticks the simulation for given delta time.
    void update(float dTime);

    // Gets the physics simulation world.
    btDynamicsWorld* getWorld() { return m_world; };

    // Adds the simplest aab to the world.
    btRigidBody* addBox(const btVector3& origin, const btVector3& extents, btScalar mass);

    void debugMethod();

protected:

    // Ctor.
    BulletHelper();

    // Dtor.
    ~BulletHelper();

private:

    // Broadphase interface.
	btBroadphaseInterface*	m_broadphase;

    // Collision dispatcher.
	btCollisionDispatcher*	m_dispatcher;

    // Constraint solver.
	btConstraintSolver*	m_solver;

    // Collision configuration.
	btCollisionConfiguration* m_collisionConfiguration;

    // The physics world itself.
    btDynamicsWorld* m_world;
};


}

#endif