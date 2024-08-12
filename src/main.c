#include "../include/SDL2/SDL.h"
#include <stdio.h>

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow(
		"Game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		500,
		500,
		SDL_WINDOW_OPENGL // Window usable with OpenGL context.
	);
	SDL_Renderer *renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_PRESENTVSYNC
	);
	SDL_GL_CreateContext(window);

	if (!window) {
		printf("Failed to create window!");
	}

	// Open window.
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
