// Texture.cpp

// SOIL Includes
#include <SOIL.h>
// Project Includes
#include "Texture.hpp"

Texture::Texture() :
  m_ID(-1), m_Width(0), m_Height(0)
{
}

Texture::~Texture()
{
  glDeleteTextures(1, &m_ID);
}

TextureType Texture::GetType() const
{
  return m_Type;
}

const std::string& Texture::GetPath() const
{
  return m_Path;
}

bool Texture::Load(const std::string& path, TextureType type)
{
  m_Path = path;
  m_Type = type;

  unsigned char* data = SOIL_load_image(m_Path.c_str(), &m_Width, &m_Height, 0, SOIL_LOAD_RGB);

  if (data == nullptr)
  {
    return false;
  }

  glGenTextures(1, &m_ID);
  glBindTexture(GL_TEXTURE_2D, m_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  SOIL_free_image_data(data);

  return true;
}

void Texture::Bind(GLuint location) const
{
  glActiveTexture(GL_TEXTURE0 + location);
  glBindTexture(GL_TEXTURE_2D, m_ID);
  glActiveTexture(GL_TEXTURE0);
}
