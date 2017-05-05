// Mesh.cpp

// Project Includes
#include "Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint> indices, const std::vector<std::shared_ptr<Texture>> textures) :
  m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{
  Setup();
}

Mesh::~Mesh()
{
  glDeleteBuffers(1, &m_EBO);
  glDeleteBuffers(1, &m_VBO);
  glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::Draw(Shader& shader) const
{
  GLuint diffuseID = 1;
  GLuint specularID = 1;

  for (GLuint i = 0; i < m_Textures.size(); ++i)
  {
    std::string name;

    switch (m_Textures[i]->GetType())
    {
      case TextureType::Diffuse:
        name = "material.diffuseTexture" + std::to_string(diffuseID++);
        break;
      case TextureType::Specular:
        name = "material.specularTexture" + std::to_string(specularID++);
        break;
    }

    shader.SetUniform(name, i);

    m_Textures[i]->Bind(i);
  }

  shader.SetUniform("material.shininess", 16.0f);

  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  for (GLuint i = 0; i < m_Textures.size(); ++i)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void Mesh::Setup()
{
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices.front(), GL_STATIC_DRAW);

  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), &m_Indices.front(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}
