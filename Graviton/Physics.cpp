#include "Physics.hpp"

void InitPhysics() {
	// setup bullet
	MainPhysicsWorld = CreatePhysicsWorld();
}

void UpdatePhysics() {
	UpdatePhysicsWorld(MainPhysicsWorld);
}

void CleanupPhysics() {
	DestroyPhysicsWorld(MainPhysicsWorld);
}

PhysicsWorld CreatePhysicsWorld() {
	Bullet_PhysicsWorld* p = (Bullet_PhysicsWorld*)malloc(sizeof(Bullet_PhysicsWorld));
	p->collisionConfiguration = new btDefaultCollisionConfiguration();
	p->dispatcher = new btCollisionDispatcher(p->collisionConfiguration);
	p->broadphase = new btDbvtBroadphase();
	p->solver = new btSequentialImpulseConstraintSolver;
	p->dynamicsWorld = new btDiscreteDynamicsWorld(p->dispatcher, p->broadphase, p->solver, p->collisionConfiguration);

	p->collisionShapes = new btAlignedObjectArray<btCollisionShape*>();

	p->dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));

	return (void*)p;
}

void UpdatePhysicsWorld(void* p) {
	((Bullet_PhysicsWorld*)p)->dynamicsWorld->stepSimulation(1.0f / 60.0f);
}

void DestroyPhysicsWorld(void* p) {
	/*
	for (int i = ((Bullet_PhysicsWorld*)p)->dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = ((Bullet_PhysicsWorld*)p)->dynamicsWorld->getCollisionObjectArray().at(i);
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			while (body->getNumConstraintRefs())
			{
				btTypedConstraint* constraint = body->getConstraintRef(0);
				((Bullet_PhysicsWorld*)p)->dynamicsWorld->removeConstraint(constraint);
				delete constraint;
			}
			delete body->getMotionState();
		}
		((Bullet_PhysicsWorld*)p)->dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	for (int j = 0; j < ((Bullet_PhysicsWorld*)p)->collisionShapes->size(); j++)
	{
		btCollisionShape* shape = (btCollisionShape*)(((Bullet_PhysicsWorld*)p)->collisionShapes->at(j));
		//collisionShapes[j] = 0;
		delete shape;
	}
	*/

	delete ((Bullet_PhysicsWorld*)p)->dynamicsWorld;
	delete ((Bullet_PhysicsWorld*)p)->solver;
	delete ((Bullet_PhysicsWorld*)p)->broadphase;
	delete ((Bullet_PhysicsWorld*)p)->dispatcher;
	delete ((Bullet_PhysicsWorld*)p)->collisionConfiguration;
	((Bullet_PhysicsWorld*)p)->collisionShapes->clear();
	delete ((Bullet_PhysicsWorld*)p)->collisionShapes;

	free(p);
}

void Physics_AddObjectToWorld(void* w, void* o)
{
	((Bullet_PhysicsWorld*)w)->dynamicsWorld->addRigidBody(((Bullet_PhysicsObject*)o)->rigidBody);
}

void Physics_SetGravity(void* w, Vector3 g)
{
	((Bullet_PhysicsWorld*)w)->dynamicsWorld->setGravity(btVector3(g.x, g.y, g.z));
}

PhysicsObject CreatePhysicsObject(int kinematic) {
	Bullet_PhysicsObject* p = (Bullet_PhysicsObject*)malloc(sizeof(Bullet_PhysicsObject));
	p->ms = new btDefaultMotionState();
	p->cs = new btSphereShape(1.0f);
	p->rigidBody = new btRigidBody(1.0f * (1 - kinematic), p->ms, p->cs);
	p->rigidBody->setActivationState(DISABLE_DEACTIVATION);

	return (void*)p;
}

void PhysicsObject_SetCollider(void* p, void* col) {
	((Bullet_PhysicsObject*)p)->rigidBody->setCollisionShape((btCollisionShape*)col);
}

void PhysicsObject_GetOpenGlMatrix(void* p, float* f)
{
	((Bullet_PhysicsObject*)p)->rigidBody->getCenterOfMassTransform().getOpenGLMatrix(f);
}

void PhysicsObject_Translate(void* p, Vector3 v)
{
	((Bullet_PhysicsObject*)p)->rigidBody->translate(btVector3(v.x, v.y, v.z));
}

Vector3 PhysicsObject_GetPosition(void* p)
{
	btVector3 vec = ((Bullet_PhysicsObject*)p)->rigidBody->getCenterOfMassTransform().getOrigin();
	return { vec.getX(), vec.getY(), vec.getZ() };
}

void DestroyPhysicsObject(void* p) {
	delete ((Bullet_PhysicsObject*)p)->rigidBody;
	delete ((Bullet_PhysicsObject*)p)->cs;
	delete ((Bullet_PhysicsObject*)p)->ms;
	free(p);
}

void* Physics_Trimesh(Model* model) {
	if (model == NULL) {
		printf("ERROR::PHYSICS - tried to generate collision mesh from null model");
		return (void*)(new btSphereShape(1.0f));
	}
	btTriangleMesh* trimesh = new btTriangleMesh();
	for (unsigned int i = 0; i < model->NumMeshes; i++) {
		for (unsigned int j = 0; j < model->Meshes[i]->NumVertices; j++) {
			trimesh->findOrAddVertex(btVector3(
				model->Meshes[i]->Vertices[j].Position.x,
				model->Meshes[i]->Vertices[j].Position.y,
				model->Meshes[i]->Vertices[j].Position.z), false);
		}
		for (unsigned int j = 0; j < model->Meshes[i]->NumIndices; j += 3) {
			trimesh->addTriangleIndices(model->Meshes[i]->Indices[j], model->Meshes[i]->Indices[j + 1], model->Meshes[i]->Indices[j + 2]);
		}
	}

	return (void*)(new btBvhTriangleMeshShape(trimesh, true));
}