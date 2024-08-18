#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void resize_opengl_viewport(SDL_Window *window) {
	int h = SDL_GetWindowSurface(window)->h;
	int w = SDL_GetWindowSurface(window)->w;
	assert(h);
	assert(w);

	// Resize OpenGL viewport with new window size.
	glViewport(0, 0, w, h);
}

void close_on_esc(SDL_KeyboardEvent *key, int *running) {
	if (strcmp(SDL_GetKeyName(key->keysym.sym), "15695182")) {
		printf("Escape pressed. Closing...\n");
		*running = 0;
	}
}

void print_keyboard_event(SDL_KeyboardEvent *key) {
	// Key up or key down?
	if (key->type == SDL_KEYUP) {
		printf("Released - ");
	} else {
		printf("Pressed - ");
	} 

	// Print name of key.
	printf("%s\n", SDL_GetKeyName(key->keysym.sym));
}

SDL_Window *window_init(int height, int width) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// Init SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not init, error: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Window *window = SDL_CreateWindow(
		"Game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		height,
		width,
		SDL_WINDOW_OPENGL // Window usable with OpenGL context.
	);
	SDL_SetWindowResizable(window, SDL_TRUE);

	// Print and crash program if unable to open window.
	if (!window) {
		printf("Failed to init window, closing now");
		exit(1);
	}

	return window;
}

int main() {
	SDL_Window *window = window_init(1280, 720);
	if (!window) {
		printf("Could not create window!");
		exit(1);
	}

	SDL_GL_CreateContext(window);
	int version = gladLoadGLLoader(SDL_GL_GetProcAddress);
	if (version == 0) {
		printf("Failed to initialize OpenGL context\n");
		return -1;
	} else {
		printf("Initialized OpenGL! Version: %d\n", version);
	}
	glViewport(0, 0, 1280, 720);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	// Main loop.
	int running = 1;
	SDL_Event event;
	while (running) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // State-SETTING function.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(window); // Swap the buffers to display the current frame

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					print_keyboard_event(&event.key);
					close_on_esc(&event.key, &running);
					break;
				//case SDL_WINDOWEVENT:
					//resize_opengl_viewport(window);
			}
		}
	}

	return 0;
}
