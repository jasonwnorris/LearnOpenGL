// Camera.cpp

// GLM Includes
#include <glm/gtc/matrix_transform.hpp>
// Project Includes
#include "Camera.hpp"

Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, GLfloat aspectRatio) :
  m_Position(position), m_Front(front), m_Up(up), m_WorldUp(up), m_Yaw(-90.0f), m_Pitch(0.0f), m_Speed(3.0f), m_Sensitivity(0.075f), m_FieldOfView(45.0f), m_AspectRatio(aspectRatio)
{
  UpdateVectors();
  UpdateViewMatrix();
  UpdateProjectionMatrix();
}

const glm::vec3& Camera::GetPosition() const
{
  return m_Position;
}

const glm::vec3& Camera::GetFront() const
{
  return m_Front;
}

const glm::vec3& Camera::GetUp() const
{
  return m_Up;
}

const glm::mat4& Camera::GetViewMatrix() const
{
  return m_ViewMatrix;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
  return m_ProjectionMatrix;
}

void Camera::HandleKeyboard(Direction direction, float deltaTime)
{
  GLfloat velocity = m_Speed * deltaTime;

  switch (direction)
  {
    case Direction::Forward:
      m_Position += m_Front * velocity;
      break;
    case Direction::Backward:
      m_Position -= m_Front * velocity;
      break;
    case Direction::Left:
      m_Position -= m_Right * velocity;
      break;
    case Direction::Right:
      m_Position += m_Right * velocity;
      break;
    case Direction::Up:
      m_Position += m_WorldUp * velocity;
      break;
    case Direction::Down:
      m_Position -= m_WorldUp * velocity;
      break;
  }

  UpdateViewMatrix();
}

void Camera::HandleMouseMovement(float offsetX, float offsetY, bool doesConstrainPitch)
{
  m_Yaw += offsetX * m_Sensitivity;
  m_Pitch += offsetY * m_Sensitivity;

  if (doesConstrainPitch)
  {
    m_Pitch = std::fmaxf(std::fminf(m_Pitch, 89.0f), -89.0f);
  }

  UpdateVectors();
  UpdateViewMatrix();
}

void Camera::HandleMouseScroll(float offsetX, float offsetY)
{
  m_FieldOfView = std::fmaxf(std::fminf(m_FieldOfView - offsetY, 45), 1.0f);

  UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
  m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::UpdateProjectionMatrix()
{
  m_ProjectionMatrix = glm::perspective(m_FieldOfView, m_AspectRatio, 0.1f, 100.0f);
}

void Camera::UpdateVectors()
{
  glm::vec3 front;
  front.x = std::cosf(glm::radians(m_Pitch)) * std::cosf(glm::radians(m_Yaw));
  front.y = std::sinf(glm::radians(m_Pitch));
  front.z = std::cosf(glm::radians(m_Pitch)) * std::sinf(glm::radians(m_Yaw));

  m_Front = glm::normalize(front);
  m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
  m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
