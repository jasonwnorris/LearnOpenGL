// Vertex.hpp

#ifndef __VERTEX_HPP__
#define __VERTEX_HPP__

// GLM Includes
#include <glm/glm.hpp>

struct Vertex
{
  glm::vec3 Position;
  glm::vec2 TexCoords;
  glm::vec3 Normal;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;

  Vertex(const glm::vec3& position, const glm::vec2& texCoords, const glm::vec3& normal)
  {
    Position = position;
    TexCoords = texCoords;
    Normal = normal;
  }
};

#endif
