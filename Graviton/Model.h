#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "cglm/cglm.h"
#include "GL/glew.h"
#include "Graphics.h"
#include "Types.h"

typedef struct Vertex {
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoords;
} Vertex;

typedef struct Mesh {
	// data
	unsigned int NumVertices;
	unsigned int NumIndices;
	Vertex* Vertices;
	unsigned int* Indices;
	// opengl objects
	unsigned int VAO, VBO, EBO, Tex;
} Mesh;


typedef struct Model {
	unsigned int NumMeshes;
	Mesh** Meshes;
} Model;

// Setup mesh
void SetupMesh(Mesh* m);
// Draw mesh. You shouldn't need to call this manually
void DrawMesh(Mesh* m, mat4 matrix);

// Load model from specified path, if it has not already been loaded
Model* LoadModel(const char* path);
// Setup OpenGL objects for model
void SetupModel(Model* m);
// Draw model
void DrawModel(Model* m, mat4 matrix);
// Destroy model safely to prevent memory leaks
void DestroyModel(Model* m);

#endif