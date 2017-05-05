#version 330 core

#define MAX_POINT_LIGHTS 4

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  vec3 color;
  float shininess;
  float transparency;
  float reflectivity;
};

struct DirectionalLight
{
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
};

struct SpotLight
{
  vec3 position;
  vec3 direction;
  float innerCutoff;
  float outerCutoff;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
};

in vec3 fragPosition;
in vec3 fragColor;
in vec2 fragTexCoords;
in vec3 fragNormal;

out vec4 color;

uniform samplerCube cubemap;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 cameraPosition;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
  vec3 lightDirection = normalize(-light.direction);
  vec3 halfwayDirection = normalize(lightDirection + viewDirection);

  float diffuseStrength = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuseColor = vec3(texture(material.diffuse, fragTexCoords));

  float specularStrength = pow(max(dot(normal, halfwayDirection), 0.0f), material.shininess);
  vec3 specularColor = vec3(texture(material.specular, fragTexCoords));

  vec3 ambient = light.ambient * diffuseColor;
  vec3 diffuse = light.diffuse * diffuseStrength * diffuseColor;
  vec3 specular = light.specular * specularStrength * specularColor;

  return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection)
{
  vec3 lightDirection = normalize(light.position - fragPosition);
  vec3 halfwayDirection = normalize(lightDirection + viewDirection);

  float diffuseStrength = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuseColor = vec3(texture(material.diffuse, fragTexCoords));

  float specularStrength = pow(max(dot(normal, halfwayDirection), 0.0f), material.shininess);
  vec3 specularColor = vec3(texture(material.specular, fragTexCoords));

  float lightDistance = length(light.position - fragPosition);
  float attenuation = 1.0f / (light.constant + light.linear * lightDistance + light.quadratic * lightDistance);

  vec3 ambient = light.ambient * diffuseColor;
  vec3 diffuse = light.diffuse * diffuseStrength * diffuseColor;
  vec3 specular = light.specular * specularStrength * specularColor;

  return (ambient + diffuse + specular) * attenuation;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection)
{
  vec3 lightDirection = normalize(light.position - fragPosition);
  vec3 halfwayDirection = normalize(lightDirection + viewDirection);

  float diffuseStrength = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuseColor = vec3(texture(material.diffuse, fragTexCoords));

  float specularStrength = pow(max(dot(normal, halfwayDirection), 0.0f), material.shininess);
  vec3 specularColor = vec3(texture(material.specular, fragTexCoords));

  float lightDistance = length(light.position - fragPosition);
  float attenuation = 1.0f / (light.constant + light.linear * lightDistance + light.quadratic * lightDistance);

  float theta = dot(lightDirection, normalize(-light.direction));
  float epsilon = light.innerCutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);

  vec3 ambient = light.ambient * diffuseColor;
  vec3 diffuse = light.diffuse * diffuseStrength * diffuseColor;
  vec3 specular = light.specular * specularStrength * specularColor;

  return (ambient + diffuse + specular) * attenuation * intensity;
}

float near = 0.1f;
float far = 100.0f;

void main()
{
  vec3 normal = normalize(fragNormal);
  vec3 viewDirection = normalize(cameraPosition - fragPosition);

  vec3 result;
  result += CalculateDirectionalLight(directionalLight, normal, viewDirection);
  for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
  {
    result += CalculatePointLight(pointLights[i], normal, viewDirection);
  }
  result += CalculateSpotLight(spotLight, normal, viewDirection);

  float gamma = 2.2f;
  color = vec4(pow(result, vec3(1.0f / gamma)), 1.0f);
}
