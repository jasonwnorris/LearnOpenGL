#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

out VS_OUT {
  vec3 fragPosition;
  vec2 fragTexCoord;
  vec3 lightPosition;
  vec3 viewPosition;
  vec3 tangentFragmentPosition;
  vec3 tangentLightPosition;
  vec3 tangentViewPosition;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.0f);

  mat3 normalMatrix = transpose(inverse(mat3(model)));
  vec3 T = normalize(normalMatrix * tangent);
  vec3 N = normalize(normalMatrix * normal);
  vec3 B = cross(T, N);
  mat3 TBN = transpose(mat3(T, B, N));

  vs_out.fragPosition = vec3(model * vec4(position, 1.0f));
  vs_out.fragTexCoord = texCoord;
  vs_out.lightPosition = lightPosition;
  vs_out.viewPosition = viewPosition;
  vs_out.tangentFragmentPosition = TBN * vec3(model * vec4(position, 0.0f));
  vs_out.tangentLightPosition = TBN * lightPosition;
  vs_out.tangentViewPosition = TBN * viewPosition;
}
