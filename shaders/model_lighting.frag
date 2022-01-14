#version 400 core

struct light_t
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
};

struct material_t
{
  sampler2D diffuse;
};

in vec2 v_texture_coordinates;
in vec3 v_normal;
in vec3 v_frag_position;

uniform light_t light;
uniform float texture_portion;
uniform material_t material;

out vec4 out_color;

void main()
{
  vec3 color = mix(vec3(1), vec3(texture(material.diffuse, v_texture_coordinates)),
      texture_portion);

  vec3 frag_to_light = normalize(light.position - v_frag_position);
  float diffuse_strength = max(dot(v_normal, frag_to_light), 0);
  vec3 diffuse = diffuse_strength * light.diffuse;

  out_color = vec4((light.ambient + diffuse) * color, 1);
}