#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out VS_OUT {
  vec3 fragPosition;
  vec2 fragTexCoord;
  vec3 fragNormal;
  vec4 fragLightSpacePosition;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.0f);

  vs_out.fragPosition = vec3(model * vec4(position, 1.0f));
  vs_out.fragTexCoord = texCoord;
  vs_out.fragNormal = transpose(inverse(mat3(model))) * normal;
  vs_out.fragLightSpacePosition = lightSpaceMatrix * vec4(vs_out.fragPosition, 1.0f);
}
