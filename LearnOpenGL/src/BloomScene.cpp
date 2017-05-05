// BloomScene.cpp

// GLM Includes
#include <glm/gtc/matrix_transform.hpp>
// STL Includes
#include <iostream>
// Project Includes
#include "BloomScene.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"
// GLFW Includes
#include <GLFW\glfw3.h>
// SDL Includes
#include <SDL.h>

BloomScene::BloomScene() :
  m_Exposure(1.0f),
  m_IsBloomEnabled(true)
{
  // White Light
  m_LightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
  m_LightColors.push_back(glm::vec3(1.5f, 1.5f, 1.5f));

  // Red Light
  m_LightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
  m_LightColors.push_back(glm::vec3(5.0f, 0.0f, 0.0f));

  // Blue Light
  m_LightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
  m_LightColors.push_back(glm::vec3(0.0f, 0.0f, 30.0f));

  // Green Light
  m_LightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
  m_LightColors.push_back(glm::vec3(0.0f, 1.5f, 0.0f));

  m_BoxModelMatrices.push_back(glm::scale(glm::translate(glm::mat4(), glm::vec3(0.0f, -1.0f, 0.0f)), glm::vec3(25.0f, 1.0f, 25.0f)));
  m_BoxModelMatrices.push_back(glm::translate(glm::mat4(), glm::vec3(0.0f, 1.5f, 0.0f)));
  m_BoxModelMatrices.push_back(glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, 1.0f)));
  m_BoxModelMatrices.push_back(glm::scale(glm::rotate(glm::translate(glm::mat4(), glm::vec3(-1.0f, -1.0f, 2.0f)), 60.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))), glm::vec3(2.0f)));
  m_BoxModelMatrices.push_back(glm::scale(glm::rotate(glm::translate(glm::mat4(), glm::vec3(0.0f, 2.7f, 4.0f)), 23.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))), glm::vec3(2.5f)));
  m_BoxModelMatrices.push_back(glm::scale(glm::rotate(glm::translate(glm::mat4(), glm::vec3(-2.0f, 1.0f, -3.0f)), 124.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))), glm::vec3(2.0f)));
  m_BoxModelMatrices.push_back(glm::translate(glm::mat4(), glm::vec3(-3.0f, 0.0f, 0.0f)));
}

bool BloomScene::Initialize()
{
  const std::vector<GLfloat> cubeVertices = {
    // Back Face
    // Positions            // Texture Coords   // Normals
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,         0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 0.0f,         0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,         0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,         0.0f,  0.0f, -1.0f,
    // Front Face
    // Positions            // Texture Coords   // Normals
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,         0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,         0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,         0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,         0.0f,  0.0f,  1.0f,
    // Left Face
    // Positions            // Texture Coords   // Normals
    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,         -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,         -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,         -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,         -1.0f,  0.0f,  0.0f,
    // Right Face
    // Positions            // Texture Coords   // Normals
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,         1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,         1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,         1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, 0.0f,         1.0f,  0.0f,  0.0f,
     // Bottom Face
    // Positions            // Texture Coords   // Normals
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,         0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 1.0f,         0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,         0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,         0.0f, -1.0f,  0.0f,
    // Top Face
    // Positions            // Texture Coords   // Normals
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,         0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,         0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,         0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,         0.0f,  1.0f,  0.0f
  };

  const std::vector<GLuint> cubeIndices = {
    // Back Face
    0, 3, 1,
    1, 3, 2,
    // Front Face
    4, 5, 7,
    5, 6, 7,
    // Left Face
    8, 9, 11,
    9, 10, 11,
    // Right Face
    12, 15, 13,
    13, 15, 14,
    // Bottom Face
    16, 17, 19,
    17, 18, 19,
    // Top Face
    20, 23, 21,
    21, 23, 22
  };

  const std::vector<GLfloat> planeVertices = {
    // Positions    // Texture Coords
    -1.0f, -1.0f,   0.0f, 0.0f,
     1.0f, -1.0f,   1.0f, 0.0f,
     1.0f,  1.0f,   1.0f, 1.0f,
    -1.0f,  1.0f,   0.0f, 1.0f
  };

  const std::vector<GLuint> planeIndices = {
    0, 1, 3,
    1, 2, 3
  };

  // Load the textures.
  if (!m_WoodTexture.Load("res/textures/wood.png", TextureType::Diffuse))
  {
    std::cout << "Failed to load texture wood.png." << std::endl;

    return false;
  }

  if (!m_ContainerTexture.Load("res/textures/container2.png", TextureType::Diffuse))
  {
    std::cout << "Failed to load texture container2.png." << std::endl;

    return false;
  }

  // Create the shaders.
  if (!m_StandardShader.Load("res/shaders/standard.vs", "res/shaders/standard.fs"))
  {
    std::cout << "Failed to load shader standard." << std::endl;

    return false;
  }
  m_StandardShader.Use();
  m_StandardShader.SetUniform("diffuseTexture", 0);
  for (int i = 0; i < 4; ++i)
  {
    m_StandardShader.SetUniform("lightPositions[" + std::to_string(i) + "]", m_LightPositions[i]);
    m_StandardShader.SetUniform("lightColors[" + std::to_string(i) + "]", m_LightColors[i]);
  }

  if (!m_LightShader.Load("res/shaders/standard.vs", "res/shaders/light.fs"))
  {
    std::cout << "Failed to load shader light." << std::endl;

    return false;
  }

  if (!m_BlurShader.Load("res/shaders/quad.vs", "res/shaders/blur.fs"))
  {
    std::cout << "Failed to load shader blur." << std::endl;

    return false;
  }
  m_BlurShader.Use();
  m_BlurShader.SetUniform("sourceTexture", 0);

  if (!m_BloomShader.Load("res/shaders/quad.vs", "res/shaders/bloom.fs"))
  {
    std::cout << "Failed to load shader bloom." << std::endl;

    return false;
  }
  m_BloomShader.Use();
  m_BloomShader.SetUniform("sceneTexture", 0);
  m_BloomShader.SetUniform("bloomBlurTexture", 1);
  m_BloomShader.SetUniform("gamma", 2.2f);

  // Create the buffer objects.
  m_CubeBufferObject.Construct(8, cubeVertices, cubeIndices);
  m_CubeBufferObject.EnableAttribute(0, 3);
  m_CubeBufferObject.EnableAttribute(1, 2);
  m_CubeBufferObject.EnableAttribute(2, 3);

  m_PlaneBufferObject.Construct(4, planeVertices, planeIndices);
  m_PlaneBufferObject.EnableAttribute(0, 2);
  m_PlaneBufferObject.EnableAttribute(1, 2);

  // Create the final buffers.
  glGenFramebuffers(1, &m_BloomFrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFrameBuffer);

  glGenTextures(2, m_ColorBuffers);
  for (GLuint i = 0; i < 2; ++i)
  {
    glBindTexture(GL_TEXTURE_2D, m_ColorBuffers[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Globals::WindowWidth, Globals::WindowHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorBuffers[i], 0);
  }

  glGenRenderbuffers(1, &m_DepthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Globals::WindowWidth, Globals::WindowHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

  GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
  glDrawBuffers(2, attachments);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "Final Framebuffer not complete!" << std::endl;

    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Create ping pong buffers.
  glGenFramebuffers(2, m_PingPongFrameBuffers);
  glGenTextures(2, m_PingPongColorBuffers);
  for (GLuint i = 0; i < 2; ++i)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFrameBuffers[i]);

    glBindTexture(GL_TEXTURE_2D, m_PingPongColorBuffers[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Globals::WindowWidth, Globals::WindowHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PingPongColorBuffers[i], 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      std::cout << "Ping Pong Framebuffer #" << i << " not complete!" << std::endl;

      return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  return true;
}

bool BloomScene::Finalize()
{
  glDeleteTextures(2, m_PingPongColorBuffers);
  glDeleteFramebuffers(2, m_PingPongFrameBuffers);

  glDeleteTextures(2, m_ColorBuffers);
  glDeleteRenderbuffers(1, &m_DepthBuffer);
  glDeleteFramebuffers(1, &m_BloomFrameBuffer);

  return true;
}

bool BloomScene::Update(const Timer& timer)
{
  //float totalTime = timer.GetTotalTime();

  return true;
}

bool BloomScene::Draw(const Camera& camera)
{
  // Collect camera information.
  glm::vec3 cameraPosition = camera.GetPosition();
  glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 projection = camera.GetProjectionMatrix();

  // Draw the containers.
  glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFrameBuffer);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, Globals::WindowWidth, Globals::WindowHeight);

  m_StandardShader.Use();
  m_StandardShader.SetUniform("view", view);
  m_StandardShader.SetUniform("projection", projection);
  m_StandardShader.SetUniform("viewPosition", cameraPosition);

  for (GLuint i = 0; i < m_BoxModelMatrices.size(); ++i)
  {
    if (i == 0)
    {
      m_WoodTexture.Bind(0);
    }
    else
    {
      m_ContainerTexture.Bind(0);
    }

    m_StandardShader.SetUniform("model", m_BoxModelMatrices[i]);

    m_CubeBufferObject.Draw();
  }

  // Draw the light sources.
  m_LightShader.Use();
  m_LightShader.SetUniform("view", view);
  m_LightShader.SetUniform("projection", projection);

  for (GLuint i = 0; i < m_LightPositions.size(); ++i)
  {
    m_LightShader.SetUniform("model", glm::scale(glm::translate(glm::mat4(), glm::vec3(m_LightPositions[i])), glm::vec3(0.5f)));
    m_LightShader.SetUniform("lightColor", m_LightColors[i]);

    m_CubeBufferObject.Draw();
  }

  // Create blur.
  m_BlurShader.Use();

  GLboolean isHorizontalBlur = true;
  for (GLuint i = 0; i < 10; ++i)
  {
    m_BlurShader.SetUniform("isHorizontalBlur", isHorizontalBlur);

    glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFrameBuffers[isHorizontalBlur ? 1 : 0]);
    if (i == 0)
    {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, m_ColorBuffers[1]);
    }
    else
    {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, m_PingPongColorBuffers[!isHorizontalBlur ? 1 : 0]);
    }

    m_PlaneBufferObject.Draw();

    isHorizontalBlur = !isHorizontalBlur;
  }

  // Draw the adjusted output.
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, Globals::WindowWidth, Globals::WindowHeight);

  m_BloomShader.Use();
  m_BloomShader.SetUniform("exposure", m_Exposure);
  m_BloomShader.SetUniform("isBloomEnabled", m_IsBloomEnabled);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_ColorBuffers[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_PingPongColorBuffers[!isHorizontalBlur ? 1 : 0]);

  m_PlaneBufferObject.Draw();

  return true;
}

void BloomScene::KeyboardCallback(int key, int scancode, int action, int mode)
{
  if (action == GLFW_PRESS || action == SDL_PRESSED)
  {
    if (key == GLFW_KEY_SPACE || key == SDLK_SPACE)
    {
      m_IsBloomEnabled = !m_IsBloomEnabled;
    }
    else if (key == GLFW_KEY_Q || key == SDLK_q)
    {
      m_Exposure -= 0.1f;
    }
    else if (key == GLFW_KEY_E || key == SDLK_e)
    {
      m_Exposure += 0.1f;
    }
  }
}
