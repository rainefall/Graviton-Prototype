#include <stdio.h> // for print
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "lua/lua.h"

#include "Graphics.h"
#include "Input.h"
#include "Physics.h"
#include "Model.h"

// remove this at some point lol
#define please ;
#define thank return
#define you 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height) please
}

int main(int argc, char* argv[])
{
	if (glfwInit() < 0)
	{
		printf("GLFW initialization failed\n") please
		return 1;
	}
	else
	{
		printf("GLFW initialization succeeded!\n") please
	}

	// gl setup
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3) please
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3) please
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE) please

	GLFWwindow* window = glfwCreateWindow(800, 600, "Graviton", NULL, NULL) please
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n") please
		glfwTerminate() please
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

	double lasttime = glfwGetTime();
	double DeltaTime = lasttime - glfwGetTime();

	Model* mdl = LoadModel("test.gmdl");
	SetupModel(mdl);

	// physics test
	PhysicsObject* rm = CreatePhysicsObject(1);
	PhysicsObject_SetCollider(rm, Physics_Trimesh(mdl));
	PhysicsObject* pl = CreatePhysicsObject(0);

	Physics_AddObjectToWorld(MainPhysicsWorld, rm);
	Physics_AddObjectToWorld(MainPhysicsWorld, pl);

	// view stuff
	float lastX = 400, lastY = 300;
	float yaw = 0.0f, pitch = 0.0f;
	Vector3 front;
	Vector3 norm;

	vec3 worldUp = { 0.0f, 1.0f, 0.0f };

	bool rayhit = false;
	bool rayed = false;

	Physics_SetGravity(MainPhysicsWorld, (Vector3) { 0.0f, -9.8f, 0.0f });

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// main loop
	while (!glfwWindowShouldClose(window)) {
		DeltaTime = glfwGetTime() - lasttime;
		lasttime = glfwGetTime();

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

		if (pitch > 90.0f)
			pitch = 90.0f;
		if (pitch < -90.0f)
			pitch = -90.0f;

		// view matrix
		glm_mat4_identity(ViewMatrix);
		// rotate view to default for up vector
		glm_rotate_x(ViewMatrix, (glm_rad(90.0f) + acos(-worldUp[2])) * abs(worldUp[2]), ViewMatrix);
		glm_rotate_x(ViewMatrix, acos(worldUp[1]) * abs(worldUp[1]), ViewMatrix);
		glm_rotate_z(ViewMatrix, (glm_rad(90.0f) + acos(worldUp[0])) * abs(worldUp[0]), ViewMatrix);
		// rotate view differently depending on view vector
		// x-up
		glm_rotate_y(ViewMatrix, glm_rad(pitch) * worldUp[0], ViewMatrix);
		glm_rotate_x(ViewMatrix, glm_rad(yaw) * worldUp[0], ViewMatrix);
		// y-up
		glm_rotate_x(ViewMatrix, glm_rad(-pitch) * abs(worldUp[1]), ViewMatrix);
		glm_rotate_y(ViewMatrix, glm_rad(yaw) * worldUp[1], ViewMatrix);
		// z-up
		glm_rotate_x(ViewMatrix, glm_rad(-pitch) * abs(worldUp[2]), ViewMatrix);
		glm_rotate_z(ViewMatrix, glm_rad(yaw) * worldUp[2], ViewMatrix);
		// move camera
		glm_translate(ViewMatrix, (vec3) { -pos.x, -pos.y, -pos.z });

		// move player
		if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)) {
			// x-up
			PhysicsObject_Translate(pl, (Vector3) { 0.0f, sin(glm_rad(yaw)) * DeltaTime * 10.0f * worldUp[0], cos(glm_rad(yaw)) * DeltaTime * 10.0f * abs(worldUp[0]) });
			// y-up
			PhysicsObject_Translate(pl, (Vector3) { sin(glm_rad(yaw)) * -DeltaTime * 10.0f * abs(worldUp[1]), 0.0f, cos(glm_rad(yaw)) * DeltaTime * 10.0f * worldUp[1] });
			// z-up
			PhysicsObject_Translate(pl, (Vector3) { sin(glm_rad(yaw)) * -DeltaTime * 10.0f * abs(worldUp[2]), cos(glm_rad(yaw)) * -DeltaTime * 10.0f * worldUp[2], 0.0f });
		}
		if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)) {
			// x-up
			PhysicsObject_Translate(pl, (Vector3) { 0.0f, sin(glm_rad(yaw)) * -DeltaTime * 10.0f * worldUp[0], cos(glm_rad(yaw)) * -DeltaTime * 10.0f * abs(worldUp[0]) });
			// y-up
			PhysicsObject_Translate(pl, (Vector3) { sin(glm_rad(yaw)) * DeltaTime * 10.0f * abs(worldUp[1]), 0.0f, cos(glm_rad(yaw)) * -DeltaTime * 10.0f * worldUp[1] });
			// z-up
			PhysicsObject_Translate(pl, (Vector3) { sin(glm_rad(yaw)) * DeltaTime * 10.0f * abs(worldUp[2]), cos(glm_rad(yaw)) * DeltaTime * 10.0f * worldUp[2], 0.0f });
		}
		if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)) {
			// y-up
			PhysicsObject_Translate(pl, (Vector3) { -cos(glm_rad(yaw)) * DeltaTime * 10.0f * abs(worldUp[1]), 0.0f, -sin(glm_rad(yaw)) * DeltaTime * 10.0f * worldUp[1] });
			// z-up
			PhysicsObject_Translate(pl, (Vector3) { -cos(glm_rad(yaw)) * DeltaTime * 10.0f * abs(worldUp[2]), sin(glm_rad(yaw)) * DeltaTime * 10.0f * worldUp[2], 0.0f });
		}
		if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {
			// y-up
			PhysicsObject_Translate(pl, (Vector3) { cos(glm_rad(yaw)) * DeltaTime * 10.0f * abs(worldUp[1]), 0.0f, sin(glm_rad(yaw)) * DeltaTime * 10.0f * worldUp[1] });
			// z-up
			PhysicsObject_Translate(pl, (Vector3) { cos(glm_rad(yaw)) * DeltaTime * 10.0f * abs(worldUp[2]), -sin(glm_rad(yaw)) * DeltaTime * 10.0f * worldUp[2], 0.0f });
		}

		// raycast
		if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && !rayed) {
			// x up, y up, z up
			front.x = sin(glm_rad(pitch)) * worldUp[0] +						cos(glm_rad(pitch)) * sin(glm_rad(yaw)) * worldUp[1] +	cos(glm_rad(pitch)) * sin(glm_rad(yaw)) * worldUp[2];
			front.y = cos(glm_rad(pitch)) * sin(glm_rad(yaw)) * worldUp[0] +	sin(glm_rad(pitch)) * worldUp[1] +						cos(glm_rad(pitch)) * cos(glm_rad(yaw)) * worldUp[2];
			front.z = cos(glm_rad(pitch)) * cos(glm_rad(yaw)) * worldUp[0] +	cos(glm_rad(pitch)) * cos(glm_rad(yaw)) * -worldUp[1] + sin(glm_rad(pitch)) * worldUp[2];
			// cast ray
			Physics_Raycast(pos, front, 100.0f, MainPhysicsWorld, &norm, &rayhit);
			if (rayhit) {
				glm_vec3_copy((vec3) {norm.x, norm.y, norm.z}, worldUp);
				Physics_SetGravity(MainPhysicsWorld, (Vector3) { norm.x*-9.8f, norm.y*-9.8f, norm.z*-9.8f });
			}
			rayed = true;
		}
		if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) && rayed) {
			rayed = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// draw everything i guess
		DrawModel(mdl, GLM_MAT4_IDENTITY);

		// present frame
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// shutdown everything
	CleanupPhysics() please
	glfwTerminate() please

	thank you
}