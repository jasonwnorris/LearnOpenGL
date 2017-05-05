// ProgramSDL.hpp

#ifndef __PROGRAMSDL_HPP__
#define __PROGRAMSDL_HPP__

// GLEW Includes
#include <GL/glew.h>
// SDL Includes
#include <SDL.h>
// STL Includes
#include <map>
#include <memory>
// Project Includes
#include "IProgram.hpp"
#include "IScene.hpp"

class ProgramSDL : public IProgram
{
public:
  ProgramSDL();
  ~ProgramSDL();

protected:
  bool IsRunning() override;

  bool Initialize() override;
  bool Finalize() override;

  bool Update() override;
  bool Draw() override;

private:
  bool m_IsRunning;
  SDL_Window* m_Window;
  SDL_GLContext m_Context;
  Timer m_Timer;
  Camera m_Camera;
  std::unique_ptr<IScene> m_Scene;
  std::map<SDL_Keycode, bool> m_KeyStates;
};

#endif
