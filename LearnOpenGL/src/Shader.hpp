// Shader.hpp

#ifndef __SHADER_HPP__
#define __SHADER_HPP__

// GLEW Includes
#include <GL/glew.h>
// GLM Includes
#include <glm/glm.hpp>

enum class ShaderType
{
  Vertex = GL_VERTEX_SHADER,
  Fragment = GL_FRAGMENT_SHADER,
  Geometry = GL_GEOMETRY_SHADER
};

class Shader
{
public:
  Shader();
  ~Shader();

  bool Load(const std::string& vertexPath, const std::string& fragmentPath);
  bool Load(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);

  void SetUniform(const std::string& attributeName, GLboolean value);
  void SetUniform(const std::string& attributeName, GLint value);
  void SetUniform(const std::string& attributeName, GLuint value);
  void SetUniform(const std::string& attributeName, GLfloat value);
  void SetUniform(const std::string& attributeName, const glm::vec2& vector);
  void SetUniform(const std::string& attributeName, const glm::vec3& vector);
  void SetUniform(const std::string& attributeName, const glm::vec4& vector);
  void SetUniform(const std::string& attributeName, const glm::mat3& matrix);
  void SetUniform(const std::string& attributeName, const glm::mat4& matrix);
  void Use();

private:
  const std::string ReadSource(const std::string& path);
  GLuint CompileSource(const std::string& sourceCode, ShaderType type);
  void Link();
  bool GetUniformLocation(const std::string& attributeName, GLint& location);

  GLuint m_ID;
};

#endif
