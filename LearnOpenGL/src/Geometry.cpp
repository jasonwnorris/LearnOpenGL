// Geometry.hpp

// Project Includes
#include "Geometry.hpp"

const std::vector<GLfloat> Geometry::CubeVertices = {
  // Back Face
  // Positions            // Texture Coords   // Normals             // Tangents
  -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,         0.0f,  0.0f, -1.0f,    -1.0f, 0.0f, 0.0f,
   0.5f, -0.5f, -0.5f,    1.0f, 0.0f,         0.0f,  0.0f, -1.0f,    -1.0f, 0.0f, 0.0f,
   0.5f,  0.5f, -0.5f,    1.0f, 1.0f,         0.0f,  0.0f, -1.0f,    -1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,         0.0f,  0.0f, -1.0f,    -1.0f, 0.0f, 0.0f,
  // Front Face
  // Positions            // Texture Coords   // Normals             // Tangents
  -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,         0.0f,  0.0f,  1.0f,    1.0f, 0.0f, 0.0f,
   0.5f, -0.5f,  0.5f,    1.0f, 0.0f,         0.0f,  0.0f,  1.0f,    1.0f, 0.0f, 0.0f,
   0.5f,  0.5f,  0.5f,    1.0f, 1.0f,         0.0f,  0.0f,  1.0f,    1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,         0.0f,  0.0f,  1.0f,    1.0f, 0.0f, 0.0f,
  // Left Face
  // Positions            // Texture Coords   // Normals             // Tangents
  -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,         -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,         -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,         -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,         -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,
  // Right Face
  // Positions            // Texture Coords   // Normals             // Tangents
   0.5f,  0.5f,  0.5f,    1.0f, 0.0f,         1.0f,  0.0f,  0.0f,    0.0f, -1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,    1.0f, 1.0f,         1.0f,  0.0f,  0.0f,    0.0f, -1.0f, 0.0f,
   0.5f, -0.5f, -0.5f,    0.0f, 1.0f,         1.0f,  0.0f,  0.0f,    0.0f, -1.0f, 0.0f,
   0.5f, -0.5f,  0.5f,    0.0f, 0.0f,         1.0f,  0.0f,  0.0f,    0.0f, -1.0f, 0.0f,
   // Bottom Face
  // Positions            // Texture Coords   // Normals             // Tangents
  -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,         0.0f, -1.0f,  0.0f,    -1.0f, 0.0f, 0.0f,
   0.5f, -0.5f, -0.5f,    1.0f, 1.0f,         0.0f, -1.0f,  0.0f,    -1.0f, 0.0f, 0.0f,
   0.5f, -0.5f,  0.5f,    1.0f, 0.0f,         0.0f, -1.0f,  0.0f,    -1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,         0.0f, -1.0f,  0.0f,    -1.0f, 0.0f, 0.0f,
  // Top Face
  // Positions            // Texture Coords   // Normals             // Tangents
  -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,         0.0f,  1.0f,  0.0f,    1.0f, 0.0f, 0.0f,
   0.5f,  0.5f, -0.5f,    1.0f, 1.0f,         0.0f,  1.0f,  0.0f,    1.0f, 0.0f, 0.0f,
   0.5f,  0.5f,  0.5f,    1.0f, 0.0f,         0.0f,  1.0f,  0.0f,    1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,         0.0f,  1.0f,  0.0f,    1.0f, 0.0f, 0.0f
};

const std::vector<GLuint> Geometry::CubeIndices = {
  // Back Face
  0, 3, 1,
  1, 3, 2,
  // Front Face
  4, 5, 7,
  5, 6, 7,
  // Left Face
  8, 9, 11,
  9, 10, 11,
  // Right Face
  12, 15, 13,
  13, 15, 14,
  // Bottom Face
  16, 17, 19,
  17, 18, 19,
  // Top Face
  20, 23, 21,
  21, 23, 22
};

const std::vector<GLuint> Geometry::CubeInvertedIndices = {
  // Back Face
  0, 1, 3,
  1, 2, 3,
  // Front Face
  4, 7, 5,
  5, 7, 6,
  // Left Face
  8, 11, 9,
  9, 11, 10,
  // Right Face
  12, 13, 15,
  13, 14, 15,
  // Bottom Face
  16, 19, 17,
  17, 19, 18,
  // Top Face
  20, 21, 23,
  21, 22, 23
};

const GLuint Geometry::CubeCount = 3;

const std::vector<glm::vec3> Geometry::CubeTranslations = {
  glm::vec3(0.0f, 1.5f, 0.0f),
  glm::vec3(2.0f, 0.0f, 1.0f),
  glm::vec3(-1.0f, 0.0f, 2.0f)
};

const std::vector<GLfloat> Geometry::PlaneVertices = {
  // Positions    // Texture Coords
  -1.0f, -1.0f,   0.0f, 0.0f,
   1.0f, -1.0f,   1.0f, 0.0f,
   1.0f,  1.0f,   1.0f, 1.0f,
  -1.0f,  1.0f,   0.0f, 1.0f
};

const std::vector<GLuint> Geometry::PlaneIndices = {
  0, 1, 3,
  1, 2, 3
};
