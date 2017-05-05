#version 330 core

const int LIGHT_COUNT = 32;

struct Light {
  vec3 Position;
  vec3 Color;
  float Constant;
  float Linear;
  float Quadratic;
};

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform Light lights[LIGHT_COUNT];
uniform vec3 viewPosition;

void main()
{
  vec4 positionColor = texture(gPosition, fragTexCoords);
  vec4 normalColor = texture(gNormal, fragTexCoords);
  vec4 albedoColor = texture(gAlbedo, fragTexCoords);

  vec3 viewDirection = normalize(viewPosition - positionColor.rgb);

  vec3 result = albedoColor.rgb * 0.1f;

  for (int i = 0; i < LIGHT_COUNT; ++i)
  {
    vec3 lightDirection = normalize(lights[i].Position - positionColor.rgb);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);

    vec3 diffuseColor = max(dot(normalColor.rgb, lightDirection), 0.0f) * albedoColor.rgb * lights[i].Color;
    vec3 specularColor = pow(max(dot(normalColor.rgb, halfwayDirection), 0.0f), 16.0f) * albedoColor.a * lights[i].Color;

    float lightDistance = length(lights[i].Position - positionColor.rgb);
    float attenuation = 1.0f / (lights[i].Constant + (lights[i].Linear * lightDistance) + (lights[i].Quadratic * lightDistance * lightDistance));

    //result += ((diffuseColor + specularColor) * attenuation);
    result += (diffuseColor + specularColor);
  }

  color = vec4(result, 1.0f);
}
