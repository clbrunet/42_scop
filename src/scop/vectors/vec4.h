#ifndef SCOP_VECTORS_VEC4_H
#define SCOP_VECTORS_VEC4_H

#include "glad/glad.h"

typedef struct vec3_s vec3_t;

typedef GLfloat mat4x1_t[4][1];

typedef struct vec4_s {
	union {
		struct {
			GLfloat x;
			GLfloat y;
			GLfloat z;
			GLfloat w;
		};
		struct {
			GLfloat r;
			GLfloat g;
			GLfloat b;
			GLfloat a;
		};
		GLfloat array[4];
	};
}	vec4_t;

vec4_t vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void print_vec4(const vec4_t *vec4);

vec3_t vec4_to_vec3(const vec4_t *vec4);
void vec4_to_mat4x1(vec4_t vec4, mat4x1_t mat4x1);

#endif
