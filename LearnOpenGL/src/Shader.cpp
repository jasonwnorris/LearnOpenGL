// Shader.cpp

// GLM Includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// STL Includes
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
// Project Includes
#include "Shader.hpp"

Shader::Shader() :
  m_ID(-1)
{
}

Shader::~Shader()
{
  glDeleteProgram(m_ID);
}

bool Shader::Load(const std::string& vertexPath, const std::string& fragmentPath)
{
  const std::string vertexCode = ReadSource(vertexPath);
  const std::string fragmentCode = ReadSource(fragmentPath);

  GLuint vertexShader = CompileSource(vertexCode, ShaderType::Vertex);
  GLuint fragmentShader = CompileSource(fragmentCode, ShaderType::Fragment);

  m_ID = glCreateProgram();
  glAttachShader(m_ID, vertexShader);
  glAttachShader(m_ID, fragmentShader);
  Link();

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return true;
}

bool Shader::Load(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
  const std::string vertexCode = ReadSource(vertexPath);
  const std::string fragmentCode = ReadSource(fragmentPath);
  const std::string geometryCode = ReadSource(geometryPath);

  GLuint vertexShader = CompileSource(vertexCode, ShaderType::Vertex);
  GLuint fragmentShader = CompileSource(fragmentCode, ShaderType::Fragment);
  GLuint geometryShader = CompileSource(geometryCode, ShaderType::Geometry);

  m_ID = glCreateProgram();
  glAttachShader(m_ID, vertexShader);
  glAttachShader(m_ID, fragmentShader);
  glAttachShader(m_ID, geometryShader);
  Link();

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(geometryShader);

  return true;
}

void Shader::SetUniform(const std::string& attributeName, GLboolean value)
{
  GLint location;
  if (GetUniformLocation(attributeName, location))
  {
    glUniform1i(location, value);
  }
}

void Shader::SetUniform(const std::string& attributeName, GLint value)
{
  GLint location;
  if (GetUniformLocation(attributeName, location))
  {
    glUniform1i(location, value);
  }
}

void Shader::SetUniform(const std::string& attributeName, GLuint value)
{
  GLint location;
  if (GetUniformLocation(attributeName, location))
  {
    glUniform1ui(location, value);
  }
}

void Shader::SetUniform(const std::string& attributeName, GLfloat value)
{
  GLint location;
  if (GetUniformLocation(attributeName, location))
  {
    glUniform1f(location, value);
  }
}

void Shader::SetUniform(const std::string& attributeName, const glm::vec2& vector)
{
  GLint location;
  if (GetUniformLocation(attributeName, location))
  {
    glUniform2fv(location, 1, glm::value_ptr(vector));
  }
}

void Shader::SetUniform(const std::string& attributeName, const glm::vec3& vector)
{
  GLint location;
  if (GetUniformLocation(attributeName, location))
  {
    glUniform3fv(location, 1, glm::value_ptr(vector));
  }
}

void Shader::SetUniform(const std::string& attributeName, const glm::vec4& vector)
{
  GLint location;
  if (GetUniformLocation(attributeName, location))
  {
    glUniform4fv(location, 1, glm::value_ptr(vector));
  }
}

void Shader::SetUniform(const std::string& attributeName, const glm::mat3& matrix)
{
  GLint location;
  if (GetUniformLocation(attributeName, location))
  {
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }
}

void Shader::SetUniform(const std::string& attributeName, const glm::mat4& matrix)
{
  GLint location;
  if (GetUniformLocation(attributeName, location))
  {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }
}

void Shader::Use()
{
  glUseProgram(m_ID);
}

const std::string Shader::ReadSource(const std::string& path)
{
  std::stringstream stream;
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    file.open(path);
    stream << file.rdbuf();
    file.close();
  }
  catch (std::ifstream::failure e)
  {
    std::cout << "Error! Shader file reading failed: " << e.what() << std::endl;
  }

  return stream.str();
}

GLuint Shader::CompileSource(const std::string& sourceCode, ShaderType type)
{
  const GLchar* source = sourceCode.c_str();

  GLuint shader = glCreateShader(static_cast<int>(type));
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint isSuccess;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isSuccess);
  if (!isSuccess)
  {
    GLchar infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cout << "Error! Shader compilation failed: " << infoLog << std::endl;
  }

  return shader;
}

void Shader::Link()
{
  glLinkProgram(m_ID);

  GLint isSuccess;
  glGetProgramiv(m_ID, GL_LINK_STATUS, &isSuccess);
  if (!isSuccess)
  {
    GLchar infoLog[512];
    glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
    std::cout << "Error! Shader linking failed: " << infoLog << std::endl;
  }
}

bool Shader::GetUniformLocation(const std::string& attributeName, GLint& location)
{
  location = glGetUniformLocation(m_ID, attributeName.c_str());

  return location != -1;
}
