#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include "Model.h"

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

// Object
// Create physics world
PhysicsObject CreatePhysicsObject(int kinematic);
// Destroy physics object
void DestroyPhysicsObject(void* p);
// Set collider
void Physics_SetCollider(void* p, void* col);
// Get opengl transformation matrix
void Physics_GetOpenGlMatrix(void* p, float* f);

// Misc
void* Physics_Trimesh(Model* model);

#endif