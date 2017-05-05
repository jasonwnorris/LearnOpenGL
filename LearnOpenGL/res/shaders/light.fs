#version 330 core

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

uniform vec3 lightColor;

const vec3 brightnessThreshold = vec3(0.2126f, 0.7152f, 0.0722f);

void main()
{
  fragColor = vec4(lightColor, 1.0);

  if (dot(fragColor.rgb, brightnessThreshold) > 1.0f)
  {
    brightColor = vec4(lightColor, 1.0f);
  }
  else
  {
    brightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }
}
