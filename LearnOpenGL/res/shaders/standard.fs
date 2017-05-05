#version 330 core

in VS_OUT {
  vec3 fragPosition;
  vec2 fragTexCoords;
  vec3 fragNormal;
} fs_in;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

uniform sampler2D diffuseTexture;
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 viewPosition;

const float constant = 1.0f;
const float linear = 0.09f;
const float quadratic = 0.032f;

const vec3 brightnessThreshold = vec3(0.2126f, 0.7152f, 0.0722f);

void main()
{
  vec4 textureColor = texture(diffuseTexture, fs_in.fragTexCoords);
  vec3 normal = normalize(fs_in.fragNormal);

  vec3 viewDirection = normalize(viewPosition - fs_in.fragPosition);

  vec3 ambientColor = vec3(0.1f);
  vec3 diffuseColor = vec3(0.0f);
  vec3 specularColor = vec3(0.0f);

  for (int i = 0; i < 4; ++i)
  {
    vec3 lightDirection = normalize(lightPositions[i] - fs_in.fragPosition);
    vec3 halfwayDir = normalize(lightDirection + viewDirection);

    float lightDistance = length(lightPositions[i] - fs_in.fragPosition);
    float attenuation = 1.0f / (constant + (linear * lightDistance) + (quadratic * lightDistance * lightDistance));

    diffuseColor += max(dot(lightDirection, normal), 0.0f) * lightColors[i] * attenuation;
    specularColor += pow(max(dot(normal, halfwayDir), 0.0f), 64.0f) * lightColors[i] * attenuation;
  }

  vec3 result = (ambientColor + diffuseColor + specularColor) * textureColor.rgb;

  fragColor = vec4(result, 1.0f);

  if (dot(result, brightnessThreshold) > 1.0f)
  {
    brightColor = vec4(result, 1.0f);
  }
  else
  {
    brightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }
}
