#include <stdio.h>
#include "scop/vec2.h"
#include "scop/vec4.h"

void print_vec2(const vec2_t *vec2)
{
	printf("x: %f\n", vec2->x);
	printf("y: %f\n", vec2->y);
}

vec4_t vec2_to_vec4(const vec2_t *vec2)
{
	vec4_t vec4 = {
		.x = vec2->x,
		.y = vec2->y,
		.z = 0,
		.w = 0,
	};
	return vec4;
}