// main.cpp

// STL Includes
#include <memory>
// Project Includes
//#include "ProgramGLFW.hpp"
#include "ProgramSDL.hpp"

int main(int argc, char** argv)
{
  //std::unique_ptr<IProgram> program(new ProgramGLFW());
  std::unique_ptr<IProgram> program(new ProgramSDL());

  return program->Run() ? 0 : -1;
}
