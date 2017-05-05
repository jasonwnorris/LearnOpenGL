#version 330 core

in vec3 fragTexCoords;

out vec4 color;

uniform samplerCube cubemap;

void main()
{
  color = texture(cubemap, fragTexCoords);
}
