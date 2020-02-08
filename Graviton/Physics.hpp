#pragma once
#pragma once

// bullet physics
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

// C-api only sees this as a void pointer like bullet class pointers
typedef struct {
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broadphase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*>* collisionShapes;
} Bullet_PhysicsWorld;

typedef struct {
	btMotionState* ms;
	btCollisionShape* cs;
	btRigidBody* rigidBody;
} Bullet_PhysicsObject;

extern "C" {
#include "Physics.h"
}

// cpp only functions that you should not call from the main program!
