#version 330 core

in VS_OUT {
  vec3 fragPosition;
  vec2 fragTexCoord;
  vec3 fragNormal;
  vec4 fragLightSpacePosition;
} fs_in;

out vec4 color;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowTexture;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

float ShadowCalculation(vec4 lightSpacePosition, float bias)
{
  vec3 projectionCoords = (lightSpacePosition.xyz / lightSpacePosition.w) * 0.5f + 0.5f;
  if (projectionCoords.z > 1.0f)
  {
    return 0.0f;
  }

  float shadow = 0.0f;
  vec2 texelSize = 1.0f / textureSize(shadowTexture, 0);
  for (int x = -1; x <= 1; ++x)
  {
      for (int y = -1; y <= 1; ++y)
      {
          vec2 texCoord = projectionCoords.xy + vec2(x, y) * texelSize;
          vec4 depthColor = texture(shadowTexture, texCoord);

          shadow += ((projectionCoords.z - bias) > depthColor.r) ? 1.0f : 0.1f;
      }
  }
  shadow /= 9.0f;

  return shadow;
}

void main()
{
  vec4 textureColor = texture(diffuseTexture, fs_in.fragTexCoord);
  vec3 normal = normalize(fs_in.fragNormal);

  vec3 ambientColor = 0.25f * textureColor.rgb;

  vec3 lightColor = vec3(1.0f);
  vec3 lightDirection = normalize(lightPosition - fs_in.fragPosition);
  vec3 diffuseColor = max(dot(lightDirection, normal), 0.0f) * lightColor;

  vec3 viewDirection = normalize(viewPosition - fs_in.fragPosition);
  vec3 halfwayDir = normalize(lightDirection + viewDirection);
  vec3 specularColor = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f) * lightColor;

  float bias = max(0.05f * (1.0f - dot(normal, lightDirection)), 0.005f);
  float shadow = ShadowCalculation(fs_in.fragLightSpacePosition, bias);
  vec3 lighting = (ambientColor + (1.0f - shadow) * (diffuseColor + specularColor)) * textureColor.rgb;

  color = vec4(lighting, 1.0f);
}
