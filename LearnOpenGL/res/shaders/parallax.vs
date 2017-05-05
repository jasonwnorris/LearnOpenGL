#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

out VS_OUT {
  vec3 fragPosition;
  vec2 fragTexCoords;
  vec3 lightDirection;
  vec3 viewDirection;
  vec3 halfwayDirection;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.0f);

  mat3 normalMatrix = transpose(inverse(mat3(model)));
  vec3 N = normalize(normalMatrix * normal);
  vec3 T = normalize(normalMatrix * tangent);
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(T, N);
  if (dot(cross(T, N), B) < 0.0f)
  {
    T = -T;
  }
  mat3 TBN = transpose(mat3(T, B, N));

  vs_out.fragPosition = vec3(model * vec4(position, 1.0f));
  vs_out.fragTexCoords = texCoords;
  vs_out.lightDirection = normalize(lightPosition - vs_out.fragPosition) * TBN;
  vs_out.viewDirection = normalize(viewPosition - vs_out.fragPosition) * TBN;
  vs_out.halfwayDirection = normalize(vs_out.lightDirection + vs_out.viewDirection);
}
