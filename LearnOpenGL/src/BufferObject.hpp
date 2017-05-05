// BufferObject.hpp

#ifndef __BUFFEROBJECT_HPP__
#define __BUFFEROBJECT_HPP__

// STL Includes
#include <vector>
// Project Includes
#include "Shader.hpp"

class BufferObject
{
public:
  BufferObject();
  ~BufferObject();

  GLuint GetID() const;

  void Construct(GLuint vertexSize, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);

  void Draw() const;

  void EnableAttribute(GLuint index, GLuint size);

private:
  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_EBO;
  GLuint m_IndexCount;
  GLuint m_VertexSize;
  GLuint m_AttributeOffset;
};

#endif
