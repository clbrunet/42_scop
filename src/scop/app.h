#ifndef SCOP_APP_H
#define SCOP_APP_H

#include <stdbool.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "scop/vectors/vec2.h"
#include "scop/vectors/vec3.h"
#include "scop/vectors/vec2_double.h"

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
	GLint projection_view_model;
	GLint texture_portion;
}	uniforms_t;

typedef struct range_s
{
	GLfloat min;
	GLfloat max;
}	range_t;

typedef struct bounding_box_s
{
	range_t x;
	range_t y;
	range_t z;
	GLfloat max_distance;
}	bounding_box_t;

typedef enum texture_animation_phase_e
{
	TO_COLOR,
	TO_TEXTURE,
}	texture_animation_phase_t;

typedef struct app_s
{
	GLFWwindow *window;
	GLsizei window_width;
	GLsizei window_height;
	GLuint program;
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint texture_map;
	GLsizei triangle_count;
	uniforms_t uniforms;
	update_time_t time;
	GLfloat fov;
	camera_t camera;
	bounding_box_t model_bounding_box;
	bool is_entering_free_flight;
	vec2_double_t cursor_last_pos;
	GLfloat model_y_rotation;
	bool should_model_rotate;
	bool should_use_orthographic;
	texture_animation_phase_t texture_animation_phase;
	GLfloat texture_portion;
}	app_t;

#endif
