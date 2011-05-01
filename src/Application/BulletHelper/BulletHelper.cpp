/**
 * Bullet helper implementation.
 */

#include "assert.h"
#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletHelper/BulletHelper.h"
#include "DynamicShape.h"

namespace GameTest {

// Ctor.
BulletHelper::BulletHelper() :    
    m_broadphase(NULL),
    m_dispatcher(NULL),
    m_solver(NULL),
    m_collisionConfiguration(NULL),
    m_world(NULL)
{
}

// Dtor.
BulletHelper::~BulletHelper()
{
}

// Creates the new bullet helper instance.
BulletHelper* BulletHelper::createInstance()
{
    static BulletHelper* instance = NULL;
    assert(NULL == instance);

    instance = new BulletHelper();

    return instance;
}

// Initializes new physics world etc.
void BulletHelper::init()
{
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher             = new btCollisionDispatcher(m_collisionConfiguration);
    m_broadphase             = new btDbvtBroadphase();
    m_solver                 = new btSequentialImpulseConstraintSolver();
    m_world                  = new btSoftRigidDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

    btRigidBody::btRigidBodyConstructionInfo floorConstructionInfo = 
        btRigidBody::btRigidBodyConstructionInfo(0.f, NULL, new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f));
    btRigidBody* floor = new btRigidBody(floorConstructionInfo);
    floor->setRestitution(1.f);

    m_world->setGravity(btVector3(0, -0.05, 0));
    m_world->addRigidBody(floor);
}

// Adds the simplest aab to the world.
btRigidBody* BulletHelper::addBox(const btVector3& origin, const btVector3& extents, btScalar mass)
{
    btRigidBody::btRigidBodyConstructionInfo boxConstructionInfo(0.f, NULL, new btBoxShape(extents));
    boxConstructionInfo.m_startWorldTransform.setOrigin(origin);
    boxConstructionInfo.m_mass = mass;

    btRigidBody* box = new btRigidBody(boxConstructionInfo);
    box->setRestitution(0.9f);

    m_world->addRigidBody(box);

    return box;
}

// Ticks the simulation for given delta time.
void BulletHelper::update(float dTime)
{
    m_world->stepSimulation(dTime);
}

void BulletHelper::debugMethod()
{
    DynamicShape* shape = new DynamicShape();
    shape->addTriangle(btVector3(1, 0, 0),
                       btVector3(0, 0, 1),
                       btVector3(0, 1, 0));
    shape->addTriangle(btVector3(0, 0, 1),
                       btVector3(-1, 0, 0),
                       btVector3(0, 1, 0));
    shape->addTriangle(btVector3(-1, 0, 0),
                       btVector3(0, 0, -1),
                       btVector3(0, 1, 0));
    shape->addTriangle(btVector3(0, 0, -1),
                       btVector3(1, 0, 0),
                       btVector3(0, 1, 0));

    shape->addTriangle(btVector3(-1, 0, 0),
                       btVector3(1, 0, 0),
                       btVector3(0, 0, -1));
    shape->addTriangle(btVector3(-1, 0, 0),
                       btVector3(1, 0, 0),
                       btVector3(0, 0, 1));

    btRigidBody::btRigidBodyConstructionInfo shapeConstructionInfo(0.f, NULL, shape);
    shapeConstructionInfo.m_startWorldTransform.setOrigin(btVector3(0, 1, 0));
    shapeConstructionInfo.m_mass = 1.f;

    btRigidBody* body = new btRigidBody(shapeConstructionInfo);
    body->setRestitution(0.9f);

    m_world->addRigidBody(body);
}

}