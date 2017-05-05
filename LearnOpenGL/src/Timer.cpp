// Timer.cpp

// Project Includes
#include "Timer.hpp"

#ifdef USING_GLFW
// GLFW Includes
#include <GLFW\glfw3.h>
#endif

#ifdef USING_SDL
// SDL Includes
#include <SDL.h>
#endif

Timer::Timer() :
  m_TotalTime(0),
  m_DeltaTime(0),
  m_LastTime(0)
{
}

float Timer::GetTotalTime() const
{
  #ifdef USING_GLFW
  return m_TotalTime;
  #endif

  #ifdef USING_SDL
  return m_TotalTime / 1000.0f;
  #endif
}

float Timer::GetDeltaTime() const
{
  #ifdef USING_GLFW
  return m_DeltaTime;
  #endif

  #ifdef USING_SDL
  return m_DeltaTime / 1000.0f;
  #endif
}

void Timer::Update()
{
  #ifdef USING_GLFW
  m_TotalTime = static_cast<float>(glfwGetTime());
  #endif

  #ifdef USING_SDL
  m_TotalTime = SDL_GetTicks();
  #endif

  m_DeltaTime = m_TotalTime - m_LastTime;
  m_LastTime = m_TotalTime;
}
