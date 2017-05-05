#version 330 core

struct Material
{
  sampler2D diffuseTexture1;
  sampler2D diffuseTexture2;
  sampler2D diffuseTexture3;
  sampler2D specularTexture1;
  sampler2D specularTexture2;
  float shininess;
};

struct DirectionalLight
{
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
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
in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 color;

uniform Material material;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform vec3 cameraPosition;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
  vec3 lightDirection = normalize(-light.direction);
  vec3 reflectionDirection = reflect(-lightDirection, normal);

  float diffuseStrength = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuseColor = vec3(texture(material.diffuseTexture1, fragTexCoords));

  float specularStrength = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
  vec3 specularColor = vec3(texture(material.specularTexture2, fragTexCoords));

  vec3 ambient = light.ambient * diffuseColor;
  vec3 diffuse = light.diffuse * diffuseStrength * diffuseColor;
  vec3 specular = light.specular * specularStrength * specularColor;

  return ambient + diffuse + specular;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection)
{
  vec3 lightDirection = normalize(light.position - fragPosition);
  vec3 reflectionDirection = reflect(-lightDirection, normal);

  float diffuseStrength = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuseColor = vec3(texture(material.diffuseTexture1, fragTexCoords));

  float specularStrength = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
  vec3 specularColor = vec3(texture(material.specularTexture2, fragTexCoords));

  float lightDistance = length(light.position - fragPosition);
  float attenuation = 1.0f / (light.constant + light.linear * lightDistance + light.quadratic * lightDistance * lightDistance);

  float theta = dot(lightDirection, normalize(-light.direction));
  float epsilon = light.innerCutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);

  vec3 ambient = light.ambient * diffuseColor;
  vec3 diffuse = light.diffuse * diffuseStrength * diffuseColor;
  vec3 specular = light.specular * specularStrength * specularColor;

  return (ambient + diffuse + specular) * attenuation * intensity;
}

void main()
{
  vec3 normal = normalize(fragNormal);
  vec3 viewDirection = normalize(cameraPosition - fragPosition);

  vec3 result = CalculateDirectionalLight(directionalLight, normal, viewDirection) + CalculateSpotLight(spotLight, normal, viewDirection);

  color = vec4(result, 1.0f);
}
