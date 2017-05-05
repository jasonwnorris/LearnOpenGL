#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out VS_OUT {
  vec3 fragPosition;
  vec2 fragTexCoords;
  vec3 fragNormal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  vec4 worldPosition = model * vec4(position, 1.0f);

  gl_Position = projection * view * worldPosition;

  vs_out.fragPosition = worldPosition.xyz;
  vs_out.fragTexCoords = texCoords;
  vs_out.fragNormal = normalize(transpose(inverse(mat3(model))) * normal);
}
