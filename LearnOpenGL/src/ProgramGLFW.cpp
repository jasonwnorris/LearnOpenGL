// ProgramGLFW.hpp

// STL Includes
#include <stdexcept>
// Project Includes
#include "Globals.hpp"
#include "ProgramGLFW.hpp"

#include "DeferredScene.hpp"
//#include "BloomScene.hpp"
//#include "NormalShadowScene.hpp"
//#include "ParallaxScene.hpp"

Camera ProgramGLFW::m_Camera(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), Globals::WindowAspectRatio);
bool ProgramGLFW::m_KeyStates[1024];
bool ProgramGLFW::m_IsFirstMouseMovement = true;
float ProgramGLFW::m_LastMouseX = 400.0f;
float ProgramGLFW::m_LastMouseY = 300.0f;
std::unique_ptr<IScene> ProgramGLFW::m_Scene(new DeferredScene());
//std::unique_ptr<IScene> ProgramGLFW::m_Scene(new BloomScene());
//std::unique_ptr<IScene> ProgramGLFW::m_Scene(new NormalShadowScene());
//std::unique_ptr<IScene> ProgramGLFW::m_Scene(new ParallaxScene());

ProgramGLFW::ProgramGLFW()
{
}

ProgramGLFW::~ProgramGLFW()
{
}

bool ProgramGLFW::IsRunning()
{
  return !glfwWindowShouldClose(m_Window);
}

bool ProgramGLFW::Initialize()
{
  // Initialize GLFW.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  // Create a window.
  m_Window = glfwCreateWindow(Globals::WindowWidth, Globals::WindowHeight, "Learn OpenGL", nullptr, nullptr);
  if (m_Window == nullptr)
  {
    glfwTerminate();

    throw std::exception("Failed to create GLFW window.");
  }

  glfwMakeContextCurrent(m_Window);
  glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(m_Window, &ProgramGLFW::KeyboardCallback);
  glfwSetCursorPosCallback(m_Window, &ProgramGLFW::MouseCallback);
  glfwSetScrollCallback(m_Window, &ProgramGLFW::ScrollCallback);

  // Initialize GLEW.
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    glfwTerminate();

    throw std::exception("Failed to initialize GLEW.");
  }

  // Set OpenGL attributes.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  return m_Scene->Initialize();
}

bool ProgramGLFW::Finalize()
{
  if (!m_Scene->Finalize())
  {
    return false;
  }

  glfwTerminate();

  return true;
}

bool ProgramGLFW::Update()
{
  glfwPollEvents();

  m_Timer.Update();

  float deltaTime = m_Timer.GetDeltaTime();

  if (m_KeyStates[GLFW_KEY_W])
  {
    m_Camera.HandleKeyboard(Direction::Forward, deltaTime);
  }
  if (m_KeyStates[GLFW_KEY_S])
  {
    m_Camera.HandleKeyboard(Direction::Backward, deltaTime);
  }
  if (m_KeyStates[GLFW_KEY_A])
  {
    m_Camera.HandleKeyboard(Direction::Left, deltaTime);
  }
  if (m_KeyStates[GLFW_KEY_D])
  {
    m_Camera.HandleKeyboard(Direction::Right, deltaTime);
  }
  if (m_KeyStates[GLFW_KEY_R])
  {
    m_Camera.HandleKeyboard(Direction::Up, deltaTime);
  }
  if (m_KeyStates[GLFW_KEY_F])
  {
    m_Camera.HandleKeyboard(Direction::Down, deltaTime);
  }

  return m_Scene->Update(m_Timer);
}

bool ProgramGLFW::Draw()
{
  if (!m_Scene->Draw(m_Camera))
  {
    return false;
  }

  // Flip buffers.
  glfwSwapBuffers(m_Window);

  return true;
}

void ProgramGLFW::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
  switch (action)
  {
    case GLFW_PRESS:
      m_KeyStates[key] = true;
      switch (key)
      {
        case GLFW_KEY_ESCAPE:
          glfwSetWindowShouldClose(window, GL_TRUE);
          break;
      }
      break;
    case GLFW_RELEASE:
      m_KeyStates[key] = false;
      break;
  }

  m_Scene->KeyboardCallback(key, scancode, action, mod);
}

void ProgramGLFW::MouseCallback(GLFWwindow* window, double positionX, double positionY)
{
  float x = static_cast<float>(positionX);
  float y = static_cast<float>(positionY);

  if (m_IsFirstMouseMovement)
  {
    m_LastMouseX = x;
    m_LastMouseY = y;
    m_IsFirstMouseMovement = false;
  }

  float offsetX = x - m_LastMouseX;
  float offsetY = m_LastMouseY - y;

  m_Camera.HandleMouseMovement(offsetX, offsetY);

  m_LastMouseX = x;
  m_LastMouseY = y;

  m_Scene->MouseCallback(x, y);
}

void ProgramGLFW::ScrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
  float x = static_cast<float>(offsetX);
  float y = static_cast<float>(offsetY);

  m_Camera.HandleMouseScroll(x, y);

  m_Scene->ScrollCallback(x, y);
}
