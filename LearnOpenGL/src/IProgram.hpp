// IProgram.hpp

#ifndef __IPROGRAM_HPP__
#define __IPROGRAM_HPP__

class IProgram
{
public:
  IProgram() = default;
  virtual ~IProgram() = default;

  bool Run();

protected:
  virtual bool IsRunning() = 0;

  virtual bool Initialize() = 0;
  virtual bool Finalize() = 0;

  virtual bool Update() = 0;
  virtual bool Draw() = 0;
};

#endif
