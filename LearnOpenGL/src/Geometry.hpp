// Geometry.hpp

#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

// GLEW Includes
#include <GL/glew.h>
// GLM Includes
#include <glm/glm.hpp>
// STL Includes
#include <vector>

class Geometry
{
public:
  static const std::vector<GLfloat> CubeVertices;
  static const std::vector<GLuint> CubeIndices;
  static const std::vector<GLuint> CubeInvertedIndices;
  static const GLuint CubeCount;
  static const std::vector<glm::vec3> CubeTranslations;
  static const std::vector<GLfloat> PlaneVertices;
  static const std::vector<GLuint> PlaneIndices;

private:
  Geometry() = default;
  ~Geometry() = default;
};

#endif
