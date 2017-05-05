// Timer.hpp

#ifndef __TIMER_HPP__
#define __TIMER_HPP__

//#define USING_GLFW
#define USING_SDL

class Timer
{
public:
  Timer();

  float GetTotalTime() const;
  float GetDeltaTime() const;

  void Update();

private:
  #ifdef USING_GLFW
  float m_TotalTime;
  float m_DeltaTime;
  float m_LastTime;
  #endif

  #ifdef USING_SDL
  unsigned int m_TotalTime;
  unsigned int m_DeltaTime;
  unsigned int m_LastTime;
  #endif
};

#endif
