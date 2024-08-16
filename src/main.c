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

void close_on_esc(SDL_KeyboardEvent *key, int running) {
	if (strcmp(SDL_GetKeyName(key->keysym.sym), "15695182")) {
		printf("Escape pressed. Closing...\n");
		exit(1);
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

SDL_Window *init_window(int height, int width) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not init, error: %s", SDL_GetError());
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

	// Print and crash program if unable to open window.
	if (!window) {
		printf("Failed to init window, closing now");
		exit(1);
	}

	return window;
}

int main() {
	SDL_Window *window = init_window(1280, 720);
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

	// Main loop.
	int running = 1;
	SDL_Event event;
	while (running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					print_keyboard_event(&event.key);
					close_on_esc(&event.key, running);
					break;
				case SDL_WINDOWEVENT:
					resize_opengl_viewport(window);
			}
		}
	}

	return 0;
}
