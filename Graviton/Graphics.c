#include "Graphics.h"

mat4 ProjectionMatrix;
mat4 ViewMatrix = GLM_MAT4_IDENTITY_INIT;

int Shader3D;

const char* vertexShader3dSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNorm;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 Normal;\n"
"out vec2 TexCoord;\n"
"out vec3 FragPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"	Normal = aNorm;\n"
"	TexCoord = aTexCoord;\n"
"	FragPos = vec3(model * vec4(aPos, 1.0));\n"
"}\0";

const char* fragmentShader3dSource = "#version 330 core\n"
"in vec3 Normal;\n"
"in vec2 TexCoord;\n"
"in vec3 FragPos;\n"
"out vec4 FragColor;\n"
"uniform sampler2D tex;\n"
"uniform vec3 lightPos;\n"
"void main()\n"
"{\n"""
"	vec3 norm = normalize(Normal);\n"
"	vec3 lightDir = normalize(lightPos - FragPos);\n"
"	float diffuse = max(dot(norm, lightDir), 0.5);\n"
"	FragColor = diffuse * texture(tex, TexCoord);\n"
"}\0";

void InitializeGraphics(int w, int h) {
	// generate shaders
	unsigned int vertexShader, fragmentShader;

	int  success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShader3dSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}
	else {
		printf("vertex shader compiled ok\n");
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShader3dSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
	}
	else {
		printf("fragment shader compiled ok\n");
	}

	Shader3D = glCreateProgram();
	glAttachShader(Shader3D, vertexShader);
	glAttachShader(Shader3D, fragmentShader);
	glLinkProgram(Shader3D);
	glGetProgramiv(Shader3D, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Shader3D, 512, NULL, infoLog);
		printf("ERROR::SHADER::LINKING::FAILED\n%s\n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set texture uniform because it will never not be 0
	glUseProgram(Shader3D);
	glUniform1i(glGetUniformLocation(Shader3D, "tex"), 0);
	glUniform3f(glGetUniformLocation(Shader3D, "lightPos"), 0.0f, 5.0f, 2.5f);
	glUseProgram(0);

	// make the projection matrix
	glm_perspective(glm_rad(90.0f), (float)w / (float)h, 0.1f, 1000.0f, ProjectionMatrix);
	glm_translate_z(ViewMatrix, -5.0f);
	glm_translate_y(ViewMatrix, -2.5f);
}