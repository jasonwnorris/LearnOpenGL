#version 330 core

in VS_OUT {
  vec3 fragPosition;
  vec2 fragTexCoord;
  vec3 lightPosition;
  vec3 viewPosition;
  vec3 tangentFragmentPosition;
  vec3 tangentLightPosition;
  vec3 tangentViewPosition;
} fs_in;

out vec4 color;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform samplerCube shadowCubemap;
uniform float farPlane;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float ShadowCalculation(vec3 position, float bias)
{
  vec3 fragDirection = position - fs_in.lightPosition;
  float currentDepth = length(fragDirection) - bias;
  float viewDistance = length(fs_in.viewPosition - position);

  int sampleCount = 20;
  float radius = (1.0f + (viewDistance / farPlane)) / 25.0f;

  float shadow = 0.0f;
  for (int i = 0; i < sampleCount; ++i)
  {
    vec4 depthColor = texture(shadowCubemap, fragDirection + sampleOffsetDirections[i] * radius);
    float closestDepth = depthColor.r * farPlane;

    if (currentDepth > closestDepth)
    {
      shadow += 1.0f;
    }
  }

  return shadow / float(sampleCount);
}

void main()
{
  vec4 textureColor = texture(diffuseTexture, fs_in.fragTexCoord);
  vec4 normalColor = texture(normalTexture, fs_in.fragTexCoord);
  vec3 normal = normalize(normalColor.rgb * 2.0f - 1.0f);

  vec3 ambientColor = 0.25f * textureColor.rgb;

  vec3 lightColor = vec3(1.0f);
  vec3 lightDirection = normalize(fs_in.tangentLightPosition - fs_in.tangentFragmentPosition);
  vec3 diffuseColor = max(dot(lightDirection, normal), 0.0f) * lightColor;

  vec3 viewDirection = normalize(fs_in.tangentViewPosition - fs_in.tangentFragmentPosition);
  vec3 halfwayDir = normalize(lightDirection + viewDirection);
  vec3 specularColor = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f) * lightColor;

  float bias = max(0.05f * (1.0f - dot(normal, normalize(fs_in.lightPosition - fs_in.fragPosition))), 0.005f);
  float shadow = ShadowCalculation(fs_in.fragPosition, bias);
  vec3 lighting = (ambientColor + (1.0f - shadow) * (diffuseColor + specularColor)) * textureColor.rgb;

  color = vec4(lighting, 1.0f);
}
