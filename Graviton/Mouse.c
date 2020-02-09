#include "Input.h"

void InitializeInput(GLFWwindow* window)
{
	glfwSetCursorPosCallback(window, MouseCallback);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	Input_MouseX = (int)xpos;
	Input_MouseY = (int)ypos;
}