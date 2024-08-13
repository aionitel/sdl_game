#include "../include/glad/glad.h"
#include "../include/SDL2/SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

SDL_Window *init_window() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not init, error: %s", SDL_GetError());
		exit(1);
	}

	SDL_Window *window = SDL_CreateWindow(
		"Game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		500,
		500,
		SDL_WINDOW_OPENGL // Window usable with OpenGL context.
	);

	// Print and crash program if unable to open window.
	if (!window) {
		printf("Failed to init window, closing now");
		exit(1);
	}

	return window;
}

int main() {
	SDL_Window *window = init_window();
	SDL_Renderer *renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_PRESENTVSYNC
	);
	SDL_GL_CreateContext(window);

	if (!window) {
		printf("Failed to create window!");
	}

	// Main loop.
	SDL_Event event;
	int running = 1;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			}
		}
	}

	return 0;
}
