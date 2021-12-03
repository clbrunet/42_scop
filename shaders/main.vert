#version 400 core

layout (location = 0) in vec3 position;

uniform mat4 projection_view_model;

void main()
{
  gl_Position = projection_view_model * vec4(position, 1);
}
