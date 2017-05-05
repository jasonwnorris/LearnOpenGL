#version 330 core

in VS_OUT {
  vec3 fragPosition;
  vec2 fragTexCoords;
  vec3 lightDirection;
  vec3 viewDirection;
  vec3 halfwayDirection;
} fs_in;

out vec4 color;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;

uniform float heightScale;

const float minLayers = 8;
const float maxLayers = 32;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDirection)
{
  float layerCount = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDirection)));
  float layerDepth = 1.0f / layerCount;

  vec2 deltaTexCoords = (viewDirection.xy * heightScale) / layerCount;

  float currentLayerDepth = 0.0f;
  vec2 currentTexCoords = texCoords;
  vec4 currentDepthColor = texture(depthTexture, currentTexCoords);

  while (currentLayerDepth < currentDepthColor.r)
  {
    currentTexCoords -= deltaTexCoords;
    currentDepthColor = texture(depthTexture, currentTexCoords);
    currentLayerDepth += layerDepth;
  }

  vec2 previousTexCoords = currentTexCoords + deltaTexCoords;
  vec4 previousDepthColor = texture(depthTexture, previousTexCoords);

  float afterDepth  = currentDepthColor.r - currentLayerDepth;
  float beforeDepth = previousDepthColor.r - currentLayerDepth + layerDepth;   
  float weight = afterDepth / (afterDepth - beforeDepth);

  return previousTexCoords * weight + currentTexCoords * (1.0f - weight);
}

void main()
{
  vec2 texCoords = ParallaxMapping(fs_in.fragTexCoords, fs_in.viewDirection);
  if (texCoords.x > 1.0f || texCoords.y > 1.0f || texCoords.x < 0.0f || texCoords.y < 0.0f)
  {
    discard;
  }

  vec4 textureColor = texture(diffuseTexture, texCoords);
  vec4 normalColor = texture(normalTexture, texCoords);
  vec3 normal = normalize(normalColor.rgb * 2.0f - 1.0f);

  vec3 ambientColor = 0.1f * textureColor.rgb;
  vec3 diffuseColor = max(dot(fs_in.lightDirection, normal), 0.0f) * textureColor.rgb;
  vec3 specularColor = vec3(0.2f) * pow(max(dot(normal, fs_in.halfwayDirection), 0.0f), 64.0f);

  color = vec4(ambientColor + diffuseColor + specularColor, 1.0f);
}
