#pragma once
#ifndef MOUSE_H
#define MOUSE_H

#include "GLFW/glfw3.h"

int Input_MouseX;
int Input_MouseY;

void InitializeInput();
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void UpdateInput();

#endif