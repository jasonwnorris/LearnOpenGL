// BufferObject.cpp

// Project Includes
#include "BufferObject.hpp"

BufferObject::BufferObject() :
  m_VAO(0), m_VBO(0), m_EBO(0), m_IndexCount(0), m_VertexSize(0), m_AttributeOffset(0)
{
}

BufferObject::~BufferObject()
{
  glDeleteBuffers(1, &m_EBO);
  glDeleteBuffers(1, &m_VBO);
  glDeleteVertexArrays(1, &m_VAO);
}

GLuint BufferObject::GetID() const
{
  return m_VAO;
}

void BufferObject::Construct(GLuint vertexSize, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
{
  m_IndexCount = static_cast<GLuint>(indices.size());
  m_VertexSize = vertexSize;

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices.front(), GL_STATIC_DRAW);

  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

  glBindVertexArray(0);
}

void BufferObject::Draw() const
{
  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void BufferObject::EnableAttribute(GLuint index, GLuint size)
{
  glBindVertexArray(m_VAO);
  glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, m_VertexSize * sizeof(GLfloat), (GLvoid*)(m_AttributeOffset * sizeof(GLfloat)));
  glEnableVertexAttribArray(index);
  glBindVertexArray(0);

  m_AttributeOffset += size;
}
