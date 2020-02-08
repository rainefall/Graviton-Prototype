#include <stdio.h> // for print
#include <SDL.h> // sdl2
#include <GL/glew.h> // opengl wrangler

#include "Graphics.h"
#include "Model.h"

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL initialization failed. SDL Error: %s\n", SDL_GetError());
		return 1;
	}
	else
	{
		printf("SDL initialization succeeded!\n");
	}

	SDL_Window* window = SDL_CreateWindow("Graviton", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);

	// GL setup
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetSwapInterval(1); // vsync

	// Create GL Context
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// init glew
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	InitializeGraphics(1280, 720);

	int running = 1;

	Model* mdl = LoadModel("tree.gmdl");
	SetupModel(mdl);

	// main loop
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) {
				running = 0;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// draw everything i guess
		DrawModel(mdl, GLM_MAT4_IDENTITY);
		SDL_GL_SwapWindow(window);
	}
	
	// shutdown everything
	SDL_GL_DeleteContext(context); // Destroy GL context
	SDL_DestroyWindow(window); // Destroy window
	SDL_Quit(); // Shutdown SDL

	return 0;
}