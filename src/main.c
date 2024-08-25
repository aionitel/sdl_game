#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void draw_triangle() {
	float vertices[] = {
	    -0.5f, -0.5f, 0.0f,
     	0.5f, -0.5f, 0.0f,
     	0.0f,  0.5f, 0.0f
	};

	// Create vertex buffer object and store in graphics card memory.
	unsigned int VBO; // Vertex buffer object.
	glGenBuffers(1, &VBO); // Allocate space for buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind newely created VBO buffer object to GL_ARRAY_BUFFER target type.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create vertex shader.
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	assert(vertex_shader != 0); // glCreateShader returns 0 if an error has occured.

	const char *vertex_shader_source= "#version 330 core\n"
    	"layout (location = 0) in vec3 aPos;\n"
    	"void main()\n"
    	"{\n"
    	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    	"}\0";
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	// Check if shader compiled properly.
	int success;
	char info_log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		printf("ERROR OCCURED COMPILING SHADER, ERROR: %s\n", info_log);
	} else {
		printf("COMPILED SHADER");
	}
}

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

	// Set necessary attributes. (Hints)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG, GL_TRUE);


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
	draw_triangle();

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
			}
		}
	}

	return 0;
}
