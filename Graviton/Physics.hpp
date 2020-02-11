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

typedef struct Vector3 Vector3;

extern "C" {
#include "Physics.h"
}

// cpp only functions that you should not call from the main program!
// Helper functions to convert between engine Vector3 and btVector3
btVector3 Vector3ToBullet(Vector3 vec);
Vector3 btVector3ToNative(btVector3 vec);