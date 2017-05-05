// IProgram.cpp

// Project Includes
#include "IProgram.hpp"

bool IProgram::Run()
{
  if (!Initialize())
  {
    return false;
  }

  while (IsRunning())
  {
    if (!Update() || !Draw())
    {
      return false;
    }
  }

  if (!Finalize())
  {
    return false;
  }

  return true;
}
