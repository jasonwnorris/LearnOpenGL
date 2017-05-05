#version 330 core

layout (location = 0) in vec3 position;

out vec3 fragTexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
  vec4 normalized = projection * view * vec4(position, 1.0);

  gl_Position = normalized.xyww;

  fragTexCoords = position;
}
