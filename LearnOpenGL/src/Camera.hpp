// Camera.hpp

#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

// GLEW Includes
#include <GL/glew.h>
// GLM Includes
#include <glm/glm.hpp>

enum class Direction
{
  Forward,
  Backward,
  Left,
  Right,
  Up,
  Down
};

class Camera
{
public:
  Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, GLfloat aspectRatio = 1.33333f);

  const glm::vec3& GetPosition() const;
  const glm::vec3& GetFront() const;
  const glm::vec3& GetUp() const;
  const glm::mat4& GetViewMatrix() const;
  const glm::mat4& GetProjectionMatrix() const;

  void HandleKeyboard(Direction direction, float deltaTime);
  void HandleMouseMovement(float offsetX, float offsetY, bool doesConstrainPitch = true);
  void HandleMouseScroll(float offsetX, float offsetY);

private:
  void UpdateViewMatrix();
  void UpdateProjectionMatrix();
  void UpdateVectors();

  glm::mat4 m_ViewMatrix;
  glm::mat4 m_ProjectionMatrix;
  glm::vec3 m_Position;
  glm::vec3 m_Front;
  glm::vec3 m_Up;
  glm::vec3 m_Right;
  glm::vec3 m_WorldUp;
  GLfloat m_Yaw;
  GLfloat m_Pitch;
  GLfloat m_Speed;
  GLfloat m_Sensitivity;
  GLfloat m_FieldOfView;
  GLfloat m_AspectRatio;
};

#endif
