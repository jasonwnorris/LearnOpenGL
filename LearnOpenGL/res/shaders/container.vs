#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 normal;
layout (location = 4) in mat4 instanceModel;

out vec3 fragPosition;
out vec3 fragColor;
out vec2 fragTexCoords;
out vec3 fragNormal;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform mat4 transforms[125];

void main()
{
  gl_Position = projection * view * instanceModel * vec4(position, 1.0f);

  fragPosition = vec3(instanceModel * vec4(position, 1.0f));
  fragColor = color;
  fragTexCoords = texCoords;
  fragNormal = mat3(transpose(inverse(instanceModel))) * normal;
}
