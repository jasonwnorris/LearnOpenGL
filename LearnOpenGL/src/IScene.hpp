// IScene.hpp

#ifndef __ISCENE_HPP__
#define __ISCENE_HPP__

// Project Includes
#include "Camera.hpp"
#include "Timer.hpp"

class IScene
{
public:
  virtual bool Initialize() { return true; }
  virtual bool Finalize() { return true; }

  virtual bool Update(const Timer& timer) = 0;
  virtual bool Draw(const Camera& camera) = 0;

  virtual void KeyboardCallback(int key, int scancode, int action, int mod) {}
  virtual void MouseCallback(float positionX, float positionY) {}
  virtual void ScrollCallback(float offsetX, float offsetY) {}
};

#endif
