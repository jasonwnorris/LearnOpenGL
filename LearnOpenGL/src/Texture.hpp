// Texture.hpp

#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

// GLEW Includes
#include <GL/glew.h>
// STL Includes
#include <string>

enum class TextureType
{
  Diffuse,
  Specular,
  Normal
};

class Texture
{
public:
  Texture();
  ~Texture();

  TextureType GetType() const;
  const std::string& GetPath() const;

  bool Load(const std::string& filename, TextureType type = TextureType::Diffuse);

  void Bind(GLuint location = 0) const;

private:
  TextureType m_Type;
  std::string m_Path;
  GLuint m_ID;
  int m_Width;
  int m_Height;
};

#endif
