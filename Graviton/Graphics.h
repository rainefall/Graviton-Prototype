#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "cglm/cglm.h"
#include "GL/glew.h"

// initialize graphics. you shouldn't need to call this yourself.
void InitializeGraphics(int w, int h);
// update graphics. you shouldn't need to call this manually.
void UpdateGraphics();
// 3D shader. there is only one because deferred rendering.
extern unsigned int Shader3D;

extern mat4 ProjectionMatrix;
extern mat4 ViewMatrix;

#endif 