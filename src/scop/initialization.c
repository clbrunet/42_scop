#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "scop/app.h"
#include "scop/events.h"

static int initialize_glfw(app_t *app)
{
	app->window = NULL;
	if (glfwInit() == GLFW_FALSE) {
		fprintf(stderr, "GLFW initialization failed\n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	app->window = glfwCreateWindow(app->window_width, app->window_height, "SCOP", NULL, NULL);
	if (app->window == NULL) {
		fprintf(stderr, "Window or OpenGL context creation failed failed\n");
		glfwTerminate();
		return -1;
	}
	glfwSetWindowUserPointer(app->window, app);
	glfwMakeContextCurrent(app->window);
	glfwSetFramebufferSizeCallback(app->window, &framebuffer_size_callback);
	glfwSetKeyCallback(app->window, &key_callback);

	glfwSwapInterval(1);
	return 0;
}

static int initialize_gl(app_t *app)
{
	if (gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress) == 0) {
		fprintf(stderr, "Glad initialization failed\n");
		return -1;
	}

	printf("OpenGL version : %s\n", glGetString(GL_VERSION));
	glViewport(0, 0, app->window_width, app->window_height);
	return 0;
}

char *get_file_content(const char *path)
{
	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	char *content = (char *)malloc((file_size + 1) * sizeof(char));
	if (content == NULL) {
		fclose(file);
		return NULL;
	}
	fseek(file, 0, SEEK_SET);
	fread(content, sizeof(char), file_size, file);
	content[file_size] = '\0';
	fclose(file);
	return content;
}

static GLuint create_shader(GLenum shader_type, const char *path)
{
	GLchar *shader_src = get_file_content(path);
	if (shader_src == NULL) {
		return 0;
	}
	GLuint shader = glCreateShader(shader_type);
	assert(shader != 0);
	glShaderSource(shader, 1, (const GLchar *const *)&shader_src, NULL);
	free(shader_src);
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		char info_log[512];
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		fprintf(stderr, "Vertex shader compilation error\n%s\n", info_log);
		return 0;
	}
	return shader;
}

static GLuint create_program(const char *vertex_shader_path, const char *fragment_shader_path)
{
	GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_path);
	GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_path);
	if (vertex_shader == 0 || fragment_shader == 0) {
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		glfwTerminate();
		return 0;
	}

	GLuint program = glCreateProgram();
	assert(program != 0);
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		char info_log[512];
		glGetProgramInfoLog(program, 512, NULL, info_log);
		fprintf(stderr, "Program link error\n%s\n", info_log);
		return 0;
	}
	return program;
}

int initialization(app_t *app)
{
	app->window_width = 1280;
	app->window_height = 720;
	if (initialize_glfw(app) == -1) {
		return -1;
	}

	if (initialize_gl(app) == -1) {
		glfwTerminate();
		return -1;
	}

	app->program = create_program("./shaders/vertex.glsl", "./shaders/fragment.glsl");
	if (app->program == 0) {
		glfwTerminate();
		return -1;
	}
	glUseProgram(app->program);

	// BASIC
	// int vertices_count = 5;
	// GLfloat vertices[5 * 3] = {
	// 	-0.5f, 0.5f, 0.0f,
	// 	0.5f, 0.5f, 0.0f,
	// 	0.5f, -0.5f, 0.0f,
	// 	-0.5f, -0.5f, 0.0f,
	// 	-1, 1, 0.0f,
	// };
	// app->triangle_count = 4;
	// GLuint indices[4 * 3] = {
	// 	0, 1, 2,
	// 	2, 3, 0,
	// 	4, 3, 0,
	// 	4, 3, 1,
	// };

	// X rotation only
	// int vertices_count = 6;
	// GLfloat vertices[6 * 3] = {
	// 	1.000000, 1.366025, -0.366025,
	// 	1.000000, 0.366025, 1.366025,
	// 	1.000000, -1.366025, 0.366025,
	// 	-1.000000, 1.366025, -0.366025,
	// 	-1.000000, 0.366025, 1.366025,
	// 	-1.000000, -1.366025, 0.366025,
	// };
	// app->triangle_count = 4;
	// GLuint indices[4 * 3] = {
	// 	3, 1, 0,
	// 	1, 5, 2,
	// 	1, 4, 5,
	// 	3, 4, 1,
	// };

	// new 3 faces X rotation, Y rotation
	int vertices_count = 7;
	GLfloat vertices[7 * 3] = {
		0.707030, 0.408425, 0.288613,
		0.707182, -0.407108, -0.290098,
		0.000911, -0.001444, 0.866024,
		0.001063, -0.816976, 0.287313,
		-0.001063, 0.816976, -0.287313,
		-0.707182, 0.407108, 0.290098,
		-0.707030, -0.408425, -0.288613,
	};
	app->triangle_count = 6;
	GLuint indices[6 * 3] = {
		4, 2, 0,
		2, 6, 3,
		0, 3, 1,
		0, 2, 3,
		2, 5, 6,
		4, 5, 2,
	};


	int vertices_dimension = 3;
	int indices_dimension = 3;

	glGenVertexArrays(1, &app->vertex_array);
	glGenBuffers(1, &app->vertex_buffer);
	glGenBuffers(1, &app->element_buffer);

	glBindVertexArray(app->vertex_array);

	glBindBuffer(GL_ARRAY_BUFFER, app->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices_count * vertices_dimension * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, app->triangle_count * indices_dimension * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, vertices_dimension, GL_FLOAT, GL_FALSE, vertices_dimension * sizeof(GLfloat), (const GLvoid *)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	return 0;
}