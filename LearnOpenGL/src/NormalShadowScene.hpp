// NormalShadowScene.hpp

#ifndef __NORMALSHADOWSCENE_HPP__
#define __NORMALSHADOWSCENE_HPP__

// Project Includes
#include "BufferObject.hpp"
#include "IScene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class NormalShadowScene : public IScene
{
public:
  NormalShadowScene();

  bool Initialize() override;
  bool Finalize() override;

  bool Update(const Timer& timer) override;
  bool Draw(const Camera& camera) override;

private:
  void DrawScenePass(Shader& shader);

private:
  Texture m_DiffuseTexture;
  Texture m_NormalTexture;
  Shader m_BasicShader;
  Shader m_LightingShader;
  Shader m_ShadowShader;
  BufferObject m_CubeBufferObject;
  BufferObject m_PlaneBufferObject;
  GLuint m_DepthFrameBuffer;
  GLuint m_DepthCubemap;
  glm::vec3 m_LightPosition;
  glm::mat4 m_ShadowTransforms[6];
};

#endif
