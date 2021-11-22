#include <stdio.h>

#include "scop/vec4.h"
#include "scop/mat4x1.h"

void print_vec4(vec4_t vec4)
{
	printf("x: %f", vec4.x);
	printf("y: %f", vec4.y);
	printf("z: %f", vec4.z);
	printf("w: %f", vec4.w);
	printf("\n");
}

void vec4_to_mat4x1(vec4_t vec4, mat4x1_t mat4x1)
{
	mat4x1[0][0] = vec4.x;
	mat4x1[1][0] = vec4.y;
	mat4x1[2][0] = vec4.z;
	mat4x1[3][0] = vec4.w;
}
