#version 330 core

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300;

void main()
{
  // Default
  color = texture(screenTexture, fragTexCoords);

  // Invert
  //color = vec4(vec3(1.0f - texture(screenTexture, fragTexCoords)), 1.0f);

  // Kernel
  /*
  vec2 offsets[9] = vec2[](
    vec2(-offset, offset),  // top-left
    vec2(0.0f,    offset),  // top-center
    vec2(offset,  offset),  // top-right
    vec2(-offset, 0.0f),    // center-left
    vec2(0.0f,    0.0f),    // center-center
    vec2(offset,  0.0f),    // center-right
    vec2(-offset, -offset), // bottom-left
    vec2(0.0f,    -offset), // bottom-center
    vec2(offset,  -offset)  // bottom-right
  );

  // Sharpen
  float kernel[9] = float[](
    -1.0f, -1.0f, -1.0f,
    -1.0f,  9.0f, -1.0f,
    -1.0f, -1.0f, -1.0f
  );

  // Blur
  float kernel[9] = float[](
    1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
    2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
    1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
  );

  // Edge Detection
  float kernel[9] = float[](
    1.0f,  1.0f, 1.0f,
    1.0f, -8.0f, 1.0f,
    1.0f,  1.0f, 1.0f
  );

  vec3 samples[9];
  for(int i = 0; i < 9; ++i)
  {
    samples[i] = vec3(texture(screenTexture, fragTexCoords.st + offsets[i]));
  }

  vec3 result = vec3(0.0f);
  for(int i = 0; i < 9; ++i)
  {
    result += samples[i] * kernel[i];
  }

  color = vec4(result, 1.0f);
  */
}
