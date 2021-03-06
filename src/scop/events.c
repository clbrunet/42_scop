#include <stdio.h>
#include <stdbool.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "scop/app.h"
#include "scop/matrices/mat4.h"
#include "scop/vectors/vec3.h"
#include "scop/vectors/vec4.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	app_t *app = (app_t *)glfwGetWindowUserPointer(window);
	app->window.width = width;
	app->window.height = height;
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
	app_t *app = (app_t *)glfwGetWindowUserPointer(window);

	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;

			case GLFW_KEY_M:
				if (app->polygon_mode == GL_FILL) {
					app->polygon_mode = GL_LINE;
				} else if (app->polygon_mode == GL_LINE) {
					app->polygon_mode = GL_POINT;
				} else {
					app->polygon_mode = GL_FILL;
				}
				glPolygonMode(GL_FRONT_AND_BACK, app->polygon_mode);
				break;

			case GLFW_KEY_R:
				if (app->model_info.should_rotate == true) {
					app->model_info.should_rotate = false;
				} else {
					app->model_info.should_rotate = true;
				}
				break;

			case GLFW_KEY_O:
				if (app->should_use_orthographic == false) {
					app->should_use_orthographic = true;
				} else {
					app->should_use_orthographic = false;
				}
				break;

			case GLFW_KEY_N:
				if (app->should_display_normals == false) {
					app->should_display_normals = true;
				} else {
					app->should_display_normals = false;
				}
				break;

			case GLFW_KEY_L:
				if (app->should_use_lighting == false) {
					app->should_use_lighting = true;
				} else {
					app->should_use_lighting = false;
				}
				break;

			case GLFW_KEY_T:
				if (app->texture_animation_phase == TO_COLOR) {
					app->texture_animation_phase = TO_TEXTURE;
				} else {
					app->texture_animation_phase = TO_COLOR;
				}
				break;

			case GLFW_KEY_X:
				app->selected_axis = X;
				break;
			case GLFW_KEY_Y:
				app->selected_axis = Y;
				break;
			case GLFW_KEY_Z:
				app->selected_axis = Z;
				break;

			case GLFW_KEY_C:
				if (app->model_info.color.r == 1 && app->model_info.color.g == 1
					&& app->model_info.color.b == 1) {
					app->model_info.color = (vec3_t){
						.r = 1,
						.g = 0,
						.b = 0,
					};
				} else if (app->model_info.color.r == 1) {
					app->model_info.color = (vec3_t){
						.r = 0,
						.g = 1,
						.b = 0,
					};
				} else if (app->model_info.color.g == 1) {
					app->model_info.color = (vec3_t){
						.r = 0,
						.g = 0,
						.b = 1,
					};
				} else {
					app->model_info.color = (vec3_t){
						.r = 1,
						.g = 1,
						.b = 1,
					};
				}
				break;
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	(void)mods;
	app_t *app = (app_t *)glfwGetWindowUserPointer(window);

	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			app->is_entering_free_flight = true;
		} else if (action == GLFW_RELEASE) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	(void)xoffset;
	app_t *app = (app_t *)glfwGetWindowUserPointer(window);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		app->fov -= yoffset * 3;
		if (app->fov < 60) {
			app->fov = 60;
		} else if (app->fov > 120) {
			app->fov = 120;
		}
	} else {
		app->camera.speed += yoffset;
		if (app->camera.speed < 1) {
			app->camera.speed = 1;
		} else if (app->camera.speed > 100) {
			app->camera.speed = 100;
		}
	}
}
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		return;
	}

	app_t *app = (app_t *)glfwGetWindowUserPointer(window);

	if (app->is_entering_free_flight) {
		app->cursor_last_pos.x = xpos;
		app->cursor_last_pos.y = ypos;
		app->is_entering_free_flight = false;
		return;
	}
	app->camera.rotation.y -= (xpos - app->cursor_last_pos.x) / 5;
	app->camera.rotation.x -= (ypos - app->cursor_last_pos.y) / 5;

	if (app->camera.rotation.y < -360) {
		app->camera.rotation.y += 360;
	} else if (app->camera.rotation.y > 360) {
		app->camera.rotation.y -= 360;
	}
	if (app->camera.rotation.x < -89) {
		app->camera.rotation.x = -89;
	} else if (app->camera.rotation.x > 89) {
		app->camera.rotation.x = 89;
	}

	app->cursor_last_pos.x = xpos;
	app->cursor_last_pos.y = ypos;
}
