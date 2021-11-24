#ifndef SCOP_APP_H
#define SCOP_APP_H

#include <stdbool.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "scop/vec2.h"
#include "scop/vec3.h"
#include "scop/vec2_double.h"

typedef struct camera_s
{
	vec3_t position;
	vec2_t rotation;
}	camera_t;

typedef struct update_time_s
{
	GLdouble current;
	GLdouble delta;
}	update_time_t;

typedef struct uniforms_s
{
	GLuint projection_view_model;
	GLuint color;
}	uniforms_t;

typedef struct app_s
{
	GLFWwindow *window;
	GLsizei window_width;
	GLsizei window_height;
	GLuint program;
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint element_buffer;
	GLsizei triangle_count;
	uniforms_t uniforms;
	update_time_t time;
	GLfloat fov;
	camera_t camera;
	bool is_entering_free_flight;
	vec2_double_t cursor_last_pos;
	bool should_rotate;
}	app_t;

#endif
