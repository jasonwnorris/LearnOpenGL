// ProgramGLFW.hpp

#ifndef __PROGRAMGLFW_HPP__
#define __PROGRAMGLFW_HPP__

// GLEW Includes
#include <GL/glew.h>
// GLFW Includes
#include <GLFW\glfw3.h>
// STL Includes
#include <memory>
// Project Includes
#include "IProgram.hpp"
#include "IScene.hpp"

class ProgramGLFW : public IProgram
{
public:
  ProgramGLFW();
  ~ProgramGLFW();

protected:
  bool IsRunning() override;

  bool Initialize() override;
  bool Finalize() override;

  bool Update() override;
  bool Draw() override;

private:
  static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
  static void MouseCallback(GLFWwindow* window, double positionX, double positionY);
  static void ScrollCallback(GLFWwindow* window, double offsetX, double offsetY);

private:
  GLFWwindow* m_Window;
  Timer m_Timer;
  static Camera m_Camera;
  static bool m_KeyStates[1024];
  static bool m_IsFirstMouseMovement;
  static float m_LastMouseX;
  static float m_LastMouseY;
  static std::unique_ptr<IScene> m_Scene;
};

#endif
