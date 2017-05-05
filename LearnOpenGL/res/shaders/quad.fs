#version 330 core

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D sourceTexture;

void main()
{
  color = texture(sourceTexture, fragTexCoords);
}
