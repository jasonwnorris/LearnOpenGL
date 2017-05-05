// Mesh.hpp

#ifndef __MESH_HPP__
#define __MESH_HPP__

// GLM Includes
#include <glm/glm.hpp>
// STL Includes
#include <memory>
#include <vector>
// Project Includes
#include "Shader.hpp"
#include "Texture.hpp"

struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

class Mesh
{
public:
  Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint> indices, const std::vector<std::shared_ptr<Texture>> textures);
  ~Mesh();

  void Draw(Shader& shader) const;

private:
  void Setup();

  std::vector<Vertex> m_Vertices;
  std::vector<GLuint> m_Indices;
  std::vector<std::shared_ptr<Texture>> m_Textures;
  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_EBO;
};

#endif
