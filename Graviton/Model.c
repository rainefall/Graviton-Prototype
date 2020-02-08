#include "Model.h"

Model* LoadModel(const char* path) {
	Model* mdl = (Model*)malloc(sizeof(Model));
	if (mdl == NULL) {
		printf("ERROR::MODELLOAD - could not allocate memory for model\n");
		return NULL;
	}

	// open model file
	FILE* f;
	fopen_s(&f, path, "rb");
	if (f == NULL) {
		printf("ERROR::MODELLOAD - could not load model from %s\n", path);
		return NULL;
	}

	// read
	fread(&mdl->NumMeshes, sizeof(unsigned int), 1, f);
	mdl->Meshes = (Mesh**)calloc(sizeof(Mesh*), mdl->NumMeshes);
	if (mdl->Meshes == NULL) {
		printf("ERROR::MODELLOAD - could not allocate memory for mesh array\n");
		fclose(f);
		return NULL;
	}

	for (unsigned int i = 0; i < mdl->NumMeshes; i++) {
		// allocate memory for mesh
		mdl->Meshes[i] = (Mesh*)malloc(sizeof(Mesh));
		if (mdl->Meshes[i] == NULL) {
			printf("ERROR::MODELLOAD - could not allocate memory for mesh\n");
			fclose(f);
			return NULL;
		}

		// load number of vertices and indices
		fread(&mdl->Meshes[i]->NumVertices, sizeof(unsigned int), 2, f);

		// allocate memory for vertex and index arrays
		mdl->Meshes[i]->Vertices = (Vertex*)malloc(sizeof(Vertex) * mdl->Meshes[i]->NumVertices);
		if (mdl->Meshes[i]->Vertices == NULL) {
			printf("ERROR::MODELLOAD - could not allocate memory for vertices %i\n", i);
			fclose(f);
			return NULL;
		}
		mdl->Meshes[i]->Indices = (unsigned int*)calloc(sizeof(unsigned int), mdl->Meshes[i]->NumIndices);
		if (mdl->Meshes[i]->Indices == NULL) {
			printf("ERROR::MODELLOAD - could not allocate memory for indices %i\n", i);
			fclose(f);
			return NULL;
		}

		// read vertex and index arrays
		fread(&mdl->Meshes[i]->Vertices[0], sizeof(Vertex), mdl->Meshes[i]->NumVertices, f);
		fread(&mdl->Meshes[i]->Indices[0], sizeof(unsigned int), mdl->Meshes[i]->NumIndices, f);
	}

	fclose(f);
	return mdl;
}

void SetupModel(Model* m)
{
	if (m == NULL) {
		printf("ERROR:SETUP_MODEL - NULLPTR - tried to setup model that has not been loaded\n");
		return;
	}
	for (unsigned int i = 0; i < m->NumMeshes; i++) {
		SetupMesh(m->Meshes[i]);
	}
}

void DrawModel(Model* m, mat4 matrix)
{
	if (m == NULL) {
		printf("ERROR:DRAW_MODEL - NULLPTR - tried to draw model that has not been loaded\n");
		return;
	}
	for (unsigned int i = 0; i < m->NumMeshes; i++) {
		DrawMesh(m->Meshes[i], matrix);
	}
}

void DestroyModel(Model* m) {
	// free all the allocated memory
	for (unsigned int i = 0; i < m->NumMeshes; i++) {
		free(m->Meshes[i]->Vertices);
		free(m->Meshes[i]->Indices);
		free(m->Meshes);
	}
	free(m);
}