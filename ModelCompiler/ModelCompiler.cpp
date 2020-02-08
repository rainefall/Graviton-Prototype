#include <stdio.h>
// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void processMesh(aiMesh* m, const aiScene* scene, FILE* f) {
	// get number of vertices and indices
	unsigned int buf[2] = { m->mNumVertices, m->mNumFaces * 3 };
	// write number of vertices and indices
	fwrite(&buf, sizeof(unsigned int), 2, f);
	// write vertices
	for (unsigned int i = 0; i < m->mNumVertices; i++) {
		// vertex positions
		fwrite(&m->mVertices[i].x, sizeof(ai_real), 1, f);
		fwrite(&m->mVertices[i].y, sizeof(ai_real), 1, f);
		fwrite(&m->mVertices[i].z, sizeof(ai_real), 1, f);
		// normals
		fwrite(&m->mNormals[i].x, sizeof(ai_real), 1, f);
		fwrite(&m->mNormals[i].y, sizeof(ai_real), 1, f);
		fwrite(&m->mNormals[i].z, sizeof(ai_real), 1, f);
		// texture coordinates
		if (m->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			fwrite(&m->mTextureCoords[0][i].x, sizeof(ai_real), 1, f);
			fwrite(&m->mTextureCoords[0][i].y, sizeof(ai_real), 1, f);
		}
		else {
			printf("mesh does not contain texture coordinates!");
		}
	}
	// indices
	for (unsigned int i = 0; i < m->mNumFaces; i++)
	{
		aiFace face = m->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			fwrite(&face.mIndices[j], sizeof(unsigned int), 1, f);
	}
}


int main(int argc, char* argv[])
{
	if (argc == 1) {
		printf("no input file specified!");
		return 1;
	}

	// a few vars
	// file input/output paths
	char* in = argv[1];
	char* out;
	// assimp importer
	Assimp::Importer importer;

	if (argc == 1) {
		out = (char*)malloc(strlen(in) + 4);
		if (out != NULL) { // double check!!! memory management is dangerous territory
			// writing this line of code gave me an aneurysm 
			strcat_s((char*)strcat_s(out, strlen(in), in), 4, ".gmdl");
		}
	}
	else
		out = argv[2];

	// assimp import scene
	const aiScene* scene = importer.ReadFile(in,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_FlipUVs |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph);

	// if scene import failed, tell me why
	if (!scene) {
		printf("ERROR::ASSIMP - %s\n", importer.GetErrorString());
		return 2;
	}
	else {
		// save everything
		FILE* f;
		if (out != NULL) {
			fopen_s(&f, out, "wb+");
			if (f != NULL) {
				fwrite(&scene->mNumMeshes, sizeof(unsigned int), 1, f);
				for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
					processMesh(scene->mMeshes[i], scene, f);
				}
				fclose(f);
			}
			return 0;
		}
	}
}