#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <string.h>

static const int WIDTH = 1280;
static const int HEIGHT = 720;

static float vertices[] = {
	// position        // colour
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-right corner
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-left corner
    0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f // Top corner
};

static const char *vertex_shader_source =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 ourColor;\n"
	"uniform mat4 trans_mat;\n"
	"void main() {\n"
	"	gl_Position = trans_mat * vec4(aPos, 1.0);\n"
	"	ourColor = aColor;\n"
	"}\0";
static const char *fragment_shader_source =
	"#version 330 core\n"
	"in vec3 ourColor;\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
		"FragColor = vec4(ourColor, 1.0);\n"
	"}\0";

unsigned int get_shader_program() {
	// Error handling.
	int success, log_length;
	char *log;

	// Vertex shader.
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		printf("Vertex shader could not be compiled!\n");
	}

	// Fragment shader.
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);
	log = malloc(log_length);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, log_length, NULL, log);
		printf("Fragment shader could not be compiled!\n");
		printf("Fragment shader ERROR: %s\n", log);
	}
	// Link shaders.
	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader); glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success); 
	glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
	log = realloc(log, log_length);
	if (!success) {
		glGetShaderInfoLog(shader_program, log_length, NULL, log); printf("Error linking shaders with shader_program\n");
		printf("LINKER ERROR: %s\n", log);
	}

	// Cleanup.
	free(log);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program;
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
	// Window creation.
	SDL_Window *window = window_init(WIDTH, HEIGHT);
	if (!window) {
		printf("Could not create window!");
		exit(1);
	}
	SDL_GL_CreateContext(window);

	// Load GLAD. (OpenGL functions)
	int version = gladLoadGLLoader(SDL_GL_GetProcAddress);
	if (version == 0) {
		printf("Failed to initialize OpenGL context\n");
		return -1;
	} else {
		printf("Initialized OpenGL! Version: %d\n", version);
	}

	// Shader program.
	unsigned int shader_program = get_shader_program();

	// Vertex buffer and array setup.
	unsigned int vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Global draw state.
	glUseProgram(shader_program);
	glViewport(0, 0, WIDTH, HEIGHT);

	// rotation (transform) matrix.
	vec3 axis = {0.0f, 0.0f, 1.0f};
	vec3 scale = {0.5f, 0.5f, 0.5f};
	mat4 trans_mat = GLM_MAT4_IDENTITY_INIT;
	glm_mat4_print(trans_mat, stdout);
	glm_rotate(trans_mat, glm_rad(90.0f), axis);
	glm_mat4_print(trans_mat, stdout);

	// send rotation matrix to shader.
	unsigned int transform_location= glGetUniformLocation(shader_program, "trans_mat");
	glUniformMatrix4fv(transform_location, 1, GL_FALSE, trans_mat);

	// Main loop.
	int running = 1;
	SDL_Event event;
	while (running) {
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		SDL_GL_SwapWindow(window); // Swap window to update current frame.

		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					close_on_esc(&event.key, &running);
			}
		}
	}

	// Cleanup.
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(shader_program);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
