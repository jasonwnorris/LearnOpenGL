// NormalShadowScene.cpp

// GLM Includes
#include <glm/gtc/matrix_transform.hpp>
// STL Includes
#include <iostream>
// Project Includes
#include "Geometry.hpp"
#include "Globals.hpp"
#include "NormalShadowScene.hpp"

const GLfloat c_DelayFactor = 3.5f;
const GLfloat c_OrbitRadius = 3.5f;
const glm::mat4 c_ShadowProjection = glm::perspective(glm::radians(90.0f), Globals::ShadowAspectRatio, Globals::NearPlane, Globals::FarPlane);

NormalShadowScene::NormalShadowScene()
{
}

bool NormalShadowScene::Initialize()
{
  const std::vector<GLfloat> planeVertices = {
    // Positions          // Texture Coords   // Normals            // Tangents
    -0.5f, 0.0f, -0.5f,    0.0f,  0.0f,       0.0f,  1.0f,  0.0f,   1.0f, 0.0f, 0.0f,
     0.5f, 0.0f, -0.5f,   10.0f,  0.0f,       0.0f,  1.0f,  0.0f,   1.0f, 0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,   10.0f, 10.0f,       0.0f,  1.0f,  0.0f,   1.0f, 0.0f, 0.0f,
    -0.5f, 0.0f,  0.5f,    0.0f, 10.0f,       0.0f,  1.0f,  0.0f,   1.0f, 0.0f, 0.0f
  };

  const std::vector<GLuint> planeIndices = {
    0, 3, 1,
    1, 3, 2
  };

  // Load the textures.
  if (!m_DiffuseTexture.Load("res/textures/brickwall_diffuse.jpg", TextureType::Diffuse))
  {
    std::cout << "Failed to load texture brickwall_diffuse.jpg." << std::endl;

    return false;
  }

  if (!m_NormalTexture.Load("res/textures/brickwall_normal.jpg", TextureType::Normal))
  {
    std::cout << "Failed to load texture brickwall_normal.jpg." << std::endl;

    return false;
  }

  // Create the shaders.
  if (!m_BasicShader.Load("res/shaders/basic.vs", "res/shaders/basic.fs"))
  {
    std::cout << "Failed to load shader basic." << std::endl;

    return false;
  }
  m_BasicShader.Use();
  m_BasicShader.SetUniform("solidColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

  if (!m_LightingShader.Load("res/shaders/cube-point.vs", "res/shaders/cube-point.fs"))
  {
    std::cout << "Failed to load shader cube-point." << std::endl;

    return false;
  }
  m_LightingShader.Use();
  m_LightingShader.SetUniform("diffuseTexture", 0);
  m_LightingShader.SetUniform("normalTexture", 1);
  m_LightingShader.SetUniform("shadowCubemap", 2);
  m_LightingShader.SetUniform("farPlane", Globals::FarPlane);

  if (!m_ShadowShader.Load("res/shaders/shadow-point.vs", "res/shaders/shadow-point.fs", "res/shaders/shadow-point.gs"))
  {
    std::cout << "Failed to load shader shadow-point." << std::endl;

    return false;
  }
  m_ShadowShader.Use();
  m_ShadowShader.SetUniform("farPlane", Globals::FarPlane);

  // Create the buffer objects.
  m_CubeBufferObject.Construct(11, Geometry::CubeVertices, Geometry::CubeIndices);
  m_CubeBufferObject.EnableAttribute(0, 3);
  m_CubeBufferObject.EnableAttribute(1, 2);
  m_CubeBufferObject.EnableAttribute(2, 3);
  m_CubeBufferObject.EnableAttribute(3, 3);

  m_PlaneBufferObject.Construct(11, planeVertices, planeIndices);
  m_PlaneBufferObject.EnableAttribute(0, 3);
  m_PlaneBufferObject.EnableAttribute(1, 2);
  m_PlaneBufferObject.EnableAttribute(2, 3);
  m_PlaneBufferObject.EnableAttribute(3, 3);

  // Create the shadow framebuffer.
  glGenFramebuffers(1, &m_DepthFrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_DepthFrameBuffer);

  glGenTextures(1, &m_DepthCubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthCubemap);
  for (GLuint i = 0; i < 6; ++i)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, Globals::ShadowWidth, Globals::ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthCubemap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "Warning: Framebuffer is not complete." << std::endl;

    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}

bool NormalShadowScene::Finalize()
{
  glDeleteTextures(1, &m_DepthCubemap);
  glDeleteFramebuffers(1, &m_DepthFrameBuffer);

  return true;
}

bool NormalShadowScene::Update(const Timer& timer)
{
  float totalTime = timer.GetTotalTime();

  m_LightPosition.x = std::cosf(totalTime / c_DelayFactor) * c_OrbitRadius;
  m_LightPosition.y = 2.5f + std::sinf(totalTime * c_DelayFactor * 0.5f);
  m_LightPosition.z = std::sinf(totalTime / c_DelayFactor) * c_OrbitRadius;

  m_ShadowTransforms[0] = c_ShadowProjection * glm::lookAt(m_LightPosition, m_LightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
  m_ShadowTransforms[1] = c_ShadowProjection * glm::lookAt(m_LightPosition, m_LightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
  m_ShadowTransforms[2] = c_ShadowProjection * glm::lookAt(m_LightPosition, m_LightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  m_ShadowTransforms[3] = c_ShadowProjection * glm::lookAt(m_LightPosition, m_LightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
  m_ShadowTransforms[4] = c_ShadowProjection * glm::lookAt(m_LightPosition, m_LightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
  m_ShadowTransforms[5] = c_ShadowProjection * glm::lookAt(m_LightPosition, m_LightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

  return true;
}

bool NormalShadowScene::Draw(const Camera& camera)
{
  // Create view and projection matrices.
  glm::vec3 cameraPosition = camera.GetPosition();
  glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 projection = camera.GetProjectionMatrix();

  // Draw depth values.
  glBindFramebuffer(GL_FRAMEBUFFER, m_DepthFrameBuffer);
  glClear(GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, Globals::ShadowWidth, Globals::ShadowHeight);

  m_ShadowShader.Use();
  for (GLuint i = 0; i < 6; ++i)
  {
    m_ShadowShader.SetUniform("shadowTransforms[" + std::to_string(i) + "]", m_ShadowTransforms[i]);
  }
  m_ShadowShader.SetUniform("lightPosition", m_LightPosition);

  DrawScenePass(m_ShadowShader);

  // Draw scene.
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.125f, 0.125f, 0.125f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, Globals::WindowWidth, Globals::WindowHeight);

  m_LightingShader.Use();
  m_LightingShader.SetUniform("view", view);
  m_LightingShader.SetUniform("projection", projection);
  m_LightingShader.SetUniform("lightPosition", m_LightPosition);
  m_LightingShader.SetUniform("viewPosition", cameraPosition);

  m_DiffuseTexture.Bind(0);
  m_NormalTexture.Bind(1);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthCubemap);
  glActiveTexture(GL_TEXTURE0);

  DrawScenePass(m_LightingShader);

  // Draw light.
  m_BasicShader.Use();
  m_BasicShader.SetUniform("model", glm::scale(glm::translate(glm::mat4(), m_LightPosition), glm::vec3(0.25f)));
  m_BasicShader.SetUniform("view", view);
  m_BasicShader.SetUniform("projection", projection);

  m_CubeBufferObject.Draw();

  return true;
}

void NormalShadowScene::DrawScenePass(Shader& shader)
{
  shader.SetUniform("model", glm::scale(glm::translate(glm::mat4(), glm::vec3(0.0f, -1.0f, 0.0f)), glm::vec3(15.0f)));

  m_PlaneBufferObject.Draw();

  for (GLuint i = 0; i < Geometry::CubeCount; ++i)
  {
    shader.SetUniform("model", glm::translate(glm::mat4(), Geometry::CubeTranslations[i]));

    m_CubeBufferObject.Draw();
  }
}
