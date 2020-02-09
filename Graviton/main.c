#include <stdio.h> // for print
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Graphics.h"
#include "Input.h"
#include "Physics.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{
	if (glfwInit() < 0)
	{
		printf("GLFW initialization failed\n");
		return 1;
	}
	else
	{
		printf("GLFW initialization succeeded!\n");
	}

	// gl setup
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Graviton", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// set window resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// initialise engine
	InitializeGraphics(1280, 720);
	InitializeInput(window);
	InitPhysics();

	float mat[16];

	Model* mdl = LoadModel("testroom.gmdl");
	SetupModel(mdl);

	// physics test
	PhysicsObject* rm = CreatePhysicsObject(1);
	PhysicsObject_SetCollider(rm, Physics_Trimesh(mdl));
	PhysicsObject* pl = CreatePhysicsObject(0);

	Physics_AddObjectToWorld(MainPhysicsWorld, rm);
	Physics_AddObjectToWorld(MainPhysicsWorld, pl);

	// view direction
	Vector3 front = { 0.0f, 0.0f, -1.0f };

	// view stuff
	float lastX = 400, lastY = 300;
	float yaw = 0.0f, pitch = 0.0f;

	vec3 worldUp = { 0.0f, 1.0f, 0.0f };

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// main loop
	while (!glfwWindowShouldClose(window)) {

		// update physics engine
		UpdatePhysics();

		Vector3 pos = PhysicsObject_GetPosition(pl);

		float xoffset = Input_MouseX - lastX;
		float yoffset = lastY - Input_MouseY; // reversed since y-coordinates range from bottom to top
		lastX = Input_MouseX;
		lastY = Input_MouseY;

		const float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 180.0f)
			pitch = 180.0f;
		if (pitch < -180.0f)
			pitch = -180.0f;

		front.x =  sin(glm_rad(yaw));
		front.y =  sin(glm_rad(pitch));
		front.z = -cos(glm_rad(yaw));

		vec3 f = { front.x, front.y, front.z };
		glm_normalize(f);

		vec3 glmpos = { pos.x, pos.y, pos.z };

		vec3 cameraFront;
		glm_vec3_add(glmpos, f, cameraFront);

		glm_lookat(glmpos, cameraFront, worldUp, ViewMatrix);

		// move player
		if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS))
			PhysicsObject_Translate(pl, (Vector3) { f[0] * -0.1f, 0.0f, f[2] * -0.1f });
		if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS))
			PhysicsObject_Translate(pl, (Vector3) { f[0] * 0.1f, 0.0f, f[2] * 0.1f });
		if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS))
			PhysicsObject_Translate(pl, (Vector3) { -0.1f, 0.0f, 0.0f });
		if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS))
			PhysicsObject_Translate(pl, (Vector3) { 0.1f, 0.0f, 0.0f });

		if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)) {
			glm_vec3_copy(f, worldUp);
			Physics_SetGravity(MainPhysicsWorld, front);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// draw everything i guess
		DrawModel(mdl, GLM_MAT4_IDENTITY);

		// present frame
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// shutdown everything
	CleanupPhysics();
	glfwTerminate();

	return 0;
}