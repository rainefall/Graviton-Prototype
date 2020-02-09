#include "Model.h"

void SetupMesh(Mesh* m) {
	// generate objects
	glGenVertexArrays(1, &m->VAO);
	glGenBuffers(1, &m->VBO);
	glGenBuffers(1, &m->EBO);

	// bind vertex array object
	glBindVertexArray(m->VAO);

	// generate vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, m->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m->NumVertices, &m->Vertices[0], GL_STATIC_DRAW);

	// generate element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m->NumIndices, &m->Indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);

	// load texture (temporary!)
	int w, h;
	glActiveTexture(0);
	glGenTextures(1, &m->Tex);
	glBindTexture(GL_TEXTURE_2D, m->Tex);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load and generate the texture
	int nrChannels;
	unsigned char* data = stbi_load("test.png", &w, &h, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("ERROR::TEXTURE_LOADING\n");
	}
	stbi_image_free(data);
	
}

void DrawMesh(Mesh* m, mat4 matrix) {
	// bind texture
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, m->Tex);

	// use the 3d shader
	glUseProgram(Shader3D);
	glUniformMatrix4fv(glGetUniformLocation(Shader3D, "model"), 1, GL_FALSE, *matrix);
	glUniformMatrix4fv(glGetUniformLocation(Shader3D, "view"), 1, GL_FALSE, ViewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(Shader3D, "projection"), 1, GL_FALSE, ProjectionMatrix[0]);
	// bind & draw mesh
	glBindVertexArray(m->VAO);
	glDrawElements(GL_TRIANGLES, m->NumIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}