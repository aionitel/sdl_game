#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void close_on_esc(SDL_KeyboardEvent *key, int running) {
	assert(key->type == SDL_KEYUP);

	if (strcmp(SDL_GetKeyName(key->keysym.sym), "15695182")) {
		running = 0;
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

	SDL_Renderer *renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_PRESENTVSYNC
	);
	SDL_GL_CreateContext(window);

	// Init glad.
	int version = gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

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
			}
		}
	}

	return 0;
}
