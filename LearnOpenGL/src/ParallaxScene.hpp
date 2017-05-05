// ParallaxScene.hpp

#ifndef __PARALLAXSCENE_HPP__
#define __PARALLAXSCENE_HPP__

// Project Includes
#include "BufferObject.hpp"
#include "IScene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class ParallaxScene : public IScene
{
public:
  ParallaxScene();

  bool Initialize() override;
  bool Finalize() override;

  bool Update(const Timer& timer) override;
  bool Draw(const Camera& camera) override;

private:
  Texture m_DiffuseTexture;
  Texture m_NormalTexture;
  Texture m_DepthTexture;
  Shader m_BasicShader;
  Shader m_ParallaxShader;
  BufferObject m_CubeBufferObject;
  BufferObject m_PlaneBufferObject;
  glm::vec3 m_LightPosition;
  float m_PlaneRotation;
};

#endif
