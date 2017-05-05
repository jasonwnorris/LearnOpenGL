// ProgramSDL.hpp

// STL Includes
#include <stdexcept>
// Project Includes
#include "Globals.hpp"
#include "ProgramSDL.hpp"
#include "DeferredScene.hpp"

ProgramSDL::ProgramSDL() :
  m_IsRunning(true),
  m_Camera(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), Globals::WindowAspectRatio),
  m_Scene(new DeferredScene())
{
}

ProgramSDL::~ProgramSDL()
{
}

bool ProgramSDL::IsRunning()
{
  return m_IsRunning;
}

bool ProgramSDL::Initialize()
{
  // Initialize SDL.
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    throw std::exception("Failed to initialize SDL.");
  }

  // Create window.
  m_Window = SDL_CreateWindow("Learn OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Globals::WindowWidth, Globals::WindowHeight, SDL_WINDOW_OPENGL);
  if (m_Window == nullptr)
  {
    throw std::exception("Failed to create SDL window.");
  }

  // Create OpenGL context.
  m_Context = SDL_GL_CreateContext(m_Window);
  if (m_Context == nullptr)
  {
    throw std::exception("Failed to create OpenGL context.");
  }
  SDL_GL_MakeCurrent(m_Window, m_Context);

  // Set vertical sync to off.
  if (SDL_GL_SetSwapInterval(0) < 0)
  {
    throw std::exception("Failed to disable vertical sync.");
  }

  // Lock the mouse cursor.
  SDL_SetRelativeMouseMode(SDL_TRUE);
  SDL_SetWindowGrab(m_Window, SDL_TRUE);
  SDL_WarpMouseInWindow(m_Window, Globals::WindowWidth / 2, Globals::WindowHeight / 2);
  SDL_ShowCursor(0);

  // Initialize GLEW.
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    throw std::exception("Failed to initialize GLEW.");
  }

  // Set SDL OpenGL attributes.
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  // Set OpenGL attributes.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Print OpenGL information.
  SDL_Log("OpenGL Version: %s", glGetString(GL_VERSION));
  SDL_Log("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
  SDL_Log("Vendor: %s", glGetString(GL_VENDOR));
  SDL_Log("Renderer: %s", glGetString(GL_RENDERER));

  return m_Scene->Initialize();
}

bool ProgramSDL::Finalize()
{
  if (!m_Scene->Finalize())
  {
    return false;
  }

  SDL_GL_MakeCurrent(m_Window, nullptr);
  SDL_GL_DeleteContext(m_Context);
  SDL_DestroyWindow(m_Window);

  SDL_Quit();

  return true;
}

bool ProgramSDL::Update()
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    switch (e.type)
    {
      case SDL_QUIT:
        {
          m_IsRunning = false;
        }
        break;
      case SDL_KEYDOWN:
        {
          switch (e.key.keysym.sym)
          {
            case SDLK_ESCAPE:
              {
                m_IsRunning = false;
              }
              break;
          }

          m_KeyStates[e.key.keysym.sym] = true;

          m_Scene->KeyboardCallback(e.key.keysym.sym, e.key.keysym.scancode, e.key.state, e.key.keysym.mod);
        }
        break;
      case SDL_KEYUP:
        {
          m_KeyStates[e.key.keysym.sym] = false;

          m_Scene->KeyboardCallback(e.key.keysym.sym, e.key.keysym.scancode, e.key.state, e.key.keysym.mod);
        }
        break;
      case SDL_MOUSEMOTION:
        {
          m_Camera.HandleMouseMovement(static_cast<float>(e.motion.xrel), static_cast<float>(-e.motion.yrel));

          m_Scene->MouseCallback(static_cast<float>(e.motion.xrel), static_cast<float>(-e.motion.yrel));

          SDL_WarpMouseInWindow(m_Window, Globals::WindowWidth / 2, Globals::WindowHeight / 2);
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        {
        }
        break;
      case SDL_MOUSEBUTTONUP:
        {
        }
        break;
      case SDL_MOUSEWHEEL:
        {
          m_Camera.HandleMouseScroll(static_cast<float>(e.wheel.x), static_cast<float>(e.wheel.y));

          m_Scene->ScrollCallback(static_cast<float>(e.wheel.x), static_cast<float>(e.wheel.y));
        }
        break;
    }
  }

  m_Timer.Update();

  float deltaTime = m_Timer.GetDeltaTime();

  if (m_KeyStates[SDLK_w])
  {
    m_Camera.HandleKeyboard(Direction::Forward, deltaTime);
  }
  if (m_KeyStates[SDLK_s])
  {
    m_Camera.HandleKeyboard(Direction::Backward, deltaTime);
  }
  if (m_KeyStates[SDLK_a])
  {
    m_Camera.HandleKeyboard(Direction::Left, deltaTime);
  }
  if (m_KeyStates[SDLK_d])
  {
    m_Camera.HandleKeyboard(Direction::Right, deltaTime);
  }
  if (m_KeyStates[SDLK_r])
  {
    m_Camera.HandleKeyboard(Direction::Up, deltaTime);
  }
  if (m_KeyStates[SDLK_f])
  {
    m_Camera.HandleKeyboard(Direction::Down, deltaTime);
  }

  return m_Scene->Update(m_Timer);
}

bool ProgramSDL::Draw()
{
  if (!m_Scene->Draw(m_Camera))
  {
    return false;
  }

  // Flip buffers.
  SDL_GL_SwapWindow(m_Window);

  return true;
}
