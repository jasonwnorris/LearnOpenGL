// DeferredScene.hpp

#ifndef __DEFERREDSCENE_HPP__
#define __DEFERREDSCENE_HPP__

// Project Includes
#include "BufferObject.hpp"
#include "IScene.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class DeferredScene : public IScene
{
public:
  DeferredScene();

  bool Initialize() override;
  bool Finalize() override;

  bool Update(const Timer& timer) override;
  bool Draw(const Camera& camera) override;

  void KeyboardCallback(int key, int scancode, int action, int mode) override;

private:
  bool LoadTextures();
  bool LoadShaders();
  bool LoadBufferObjects();
  bool LoadFrameBuffers();

private:
  std::vector<glm::vec3> m_LightPositions;
  std::vector<glm::vec3> m_LightColors;
  std::vector<glm::mat4> m_BoxModelMatrices;
  Texture m_DiffuseTexture;
  Texture m_SpecularTexture;
  Shader m_GeometryShader;
  Shader m_DeferredShader;
  Shader m_BasicShader;
  BufferObject m_CubeBufferObject;
  BufferObject m_PlaneBufferObject;
  GLuint m_GeometryBuffer;
  GLuint m_PositionBuffer;
  GLuint m_NormalBuffer;
  GLuint m_AlbedoBuffer;
  GLuint m_DepthBuffer;
};

#endif
