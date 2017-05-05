#version 330 core

in VS_OUT {
  vec3 fragPosition;
  vec2 fragTexCoords;
  vec3 fragNormal;
} fs_in;

//layout (location = 0) out vec4 gPosition;
//layout (location = 1) out vec4 gNormal;
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

void main()
{
  vec4 diffuseColor = texture(diffuseTexture, fs_in.fragTexCoords);
  vec4 specularColor = texture(specularTexture, fs_in.fragTexCoords);

  //gPosition = vec4(fs_in.fragPosition, 1.0f);
  gPosition = fs_in.fragPosition;

  //gNormal = vec4(fs_in.fragNormal, 1.0f);
  gNormal = fs_in.fragNormal;

  gAlbedo.rgb = diffuseColor.rgb;
  gAlbedo.a = specularColor.a;
}
