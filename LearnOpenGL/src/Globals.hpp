// Globals.hpp

#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__

class Globals
{
public:
  static const int WindowWidth;
  static const int WindowHeight;
  static const float WindowAspectRatio;

  static const float NearPlane;
  static const float FarPlane;

  static const int ShadowWidth;
  static const int ShadowHeight;
  static const float ShadowAspectRatio;

private:
  Globals() = default;
  ~Globals() = default;
};

#endif
