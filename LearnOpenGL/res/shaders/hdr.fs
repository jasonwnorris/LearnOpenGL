#version 330 core

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D sceneTexture;
uniform sampler2D bloomBlurTexture;
uniform float exposure;
uniform float gamma;

void main()
{
  vec4 sceneColor = texture(sceneTexture, fragTexCoords);

  // exposure and gamma correction
  vec3 result = pow(vec3(1.0f) - exp(-sceneColor.rgb * exposure), vec3(1.0f / gamma));

  color = vec4(result, 1.0f);
}
