// BloomScene.hpp

#ifndef __BLOOMSCENE_HPP__
#define __BLOOMSCENE_HPP__

// Project Includes
#include "BufferObject.hpp"
#include "IScene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class BloomScene : public IScene
{
public:
  BloomScene();

  bool Initialize() override;
  bool Finalize() override;

  bool Update(const Timer& timer) override;
  bool Draw(const Camera& camera) override;

  void KeyboardCallback(int key, int scancode, int action, int mode) override;

private:
  Texture m_WoodTexture;
  Texture m_ContainerTexture;
  Shader m_StandardShader;
  Shader m_LightShader;
  Shader m_BlurShader;
  Shader m_BloomShader;
  GLuint m_BloomFrameBuffer;
  GLuint m_ColorBuffers[2];
  GLuint m_DepthBuffer;
  GLuint m_PingPongFrameBuffers[2];
  GLuint m_PingPongColorBuffers[2];
  GLfloat m_Exposure;
  GLboolean m_IsBloomEnabled;
  BufferObject m_CubeBufferObject;
  BufferObject m_PlaneBufferObject;
  std::vector<glm::vec3> m_LightPositions;
  std::vector<glm::vec3> m_LightColors;
  std::vector<glm::mat4> m_BoxModelMatrices;
};

#endif
