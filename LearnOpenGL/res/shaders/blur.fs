#version 330 core

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D sourceTexture;
uniform bool isHorizontalBlur;

uniform float weights[5] = float[](
  0.2270270270f,
  0.1945945946f,
  0.1216216216f,
  0.0540540541f,
  0.0162162162f
);

void main()
{
     vec2 texelSize = 1.0f / textureSize(sourceTexture, 0);
     vec3 result = vec3(0.0f);

     if (isHorizontalBlur)
     {
         for (int i = 0; i < 5; ++i)
         {
            result += texture(sourceTexture, fragTexCoords + vec2(texelSize.x * i, 0.0f)).rgb * weights[i];
            result += texture(sourceTexture, fragTexCoords - vec2(texelSize.x * i, 0.0f)).rgb * weights[i];
         }
     }
     else
     {
         for (int i = 0; i < 5; ++i)
         {
             result += texture(sourceTexture, fragTexCoords + vec2(0.0f, texelSize.y * i)).rgb * weights[i];
             result += texture(sourceTexture, fragTexCoords - vec2(0.0f, texelSize.y * i)).rgb * weights[i];
         }
     }

     color = vec4(result, 1.0);
}
