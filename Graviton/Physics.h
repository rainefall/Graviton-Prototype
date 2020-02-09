#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include "Model.h"
#include "Types.h"

typedef struct Vector3 Vector3;

typedef void* PhysicsWorld;
typedef void* PhysicsObject;

typedef struct Model Model;

// Core
// Initialize physics engine
void InitPhysics();
// Update physics engine
void UpdatePhysics();
// Cleanup Physics engine
void CleanupPhysics();

// main physics world
PhysicsWorld MainPhysicsWorld;

// World
// Create physics world
PhysicsWorld CreatePhysicsWorld();
// Update Physics world
void UpdatePhysicsWorld(void* p);
// Destroy physics world
void DestroyPhysicsWorld(void* p);
// Add physics object to world
void Physics_AddObjectToWorld(void* w, void* o);
// Remove physics object from world
// Set gravity
void Physics_SetGravity(void* w, Vector3 g);

// Object
// Create physics world
PhysicsObject CreatePhysicsObject(int kinematic);
// Destroy physics object
void DestroyPhysicsObject(void* p);
// Set collider
void PhysicsObject_SetCollider(void* p, void* col);
// Get opengl transformation matrix
void PhysicsObject_GetOpenGlMatrix(void* p, float* f);
// Translate
void PhysicsObject_Translate(void* p, Vector3 v);
// Get position vector
Vector3 PhysicsObject_GetPosition(void* p);

// Misc
void* Physics_Trimesh(Model* model);

#endif