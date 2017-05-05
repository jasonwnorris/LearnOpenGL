// DeferredScene.cpp

// GLM Includes
#include <glm/gtc/matrix_transform.hpp>
// STL Includes
#include <iostream>
#include <time.h>
// Project Includes
#include "Geometry.hpp"
#include "Globals.hpp"
#include "DeferredScene.hpp"
// GLFW Includes
#include <GLFW\glfw3.h>
// SDL Includes
#include <SDL.h>

const GLuint c_LightCount = 32;

DeferredScene::DeferredScene()
{
  srand(static_cast<unsigned int>(time(nullptr)));

  for (GLuint i = 0; i < c_LightCount; ++i)
  {
    GLfloat x = ((rand() % 100) / 100.0f) * 6.0f - 3.0f;
    GLfloat y = ((rand() % 100) / 100.0f) * 6.0f - 4.0f;
    GLfloat z = ((rand() % 100) / 100.0f) * 6.0f - 3.0f;

    GLfloat r = ((rand() % 100) / 200.0f) + 0.5f;
    GLfloat g = ((rand() % 100) / 200.0f) + 0.5f;
    GLfloat b = ((rand() % 100) / 200.0f) + 0.5f;

    m_LightPositions.push_back(glm::vec3(x, y, z));
    m_LightColors.push_back(glm::vec3(r, g, b));
  }

  m_BoxModelMatrices.push_back(glm::scale(glm::translate(glm::mat4(), glm::vec3(0.0f, -1.0f, 0.0f)), glm::vec3(25.0f, 1.0f, 25.0f)));
  m_BoxModelMatrices.push_back(glm::translate(glm::mat4(), glm::vec3(0.0f, 1.5f, 0.0f)));
  m_BoxModelMatrices.push_back(glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, 1.0f)));
  m_BoxModelMatrices.push_back(glm::scale(glm::rotate(glm::translate(glm::mat4(), glm::vec3(-1.0f, -1.0f, 2.0f)), 60.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))), glm::vec3(2.0f)));
  m_BoxModelMatrices.push_back(glm::scale(glm::rotate(glm::translate(glm::mat4(), glm::vec3(0.0f, 2.7f, 4.0f)), 23.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))), glm::vec3(2.5f)));
  m_BoxModelMatrices.push_back(glm::scale(glm::rotate(glm::translate(glm::mat4(), glm::vec3(-2.0f, 1.0f, -3.0f)), 124.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))), glm::vec3(2.0f)));
  m_BoxModelMatrices.push_back(glm::translate(glm::mat4(), glm::vec3(-3.0f, 0.0f, 0.0f)));
}

bool DeferredScene::Initialize()
{
  // Load the textures.
  if (!LoadTextures())
  {
    return false;
  }

  // Create the shaders.
  if (!LoadShaders())
  {
    return false;
  }

  // Create the buffer objects.
  if (!LoadBufferObjects())
  {
    return false;
  }

  // Create G-Buffer.
  if (!LoadFrameBuffers())
  {
    return false;
  }

  return true;
}

bool DeferredScene::Finalize()
{
  glDeleteTextures(1, &m_PositionBuffer);
  glDeleteTextures(1, &m_NormalBuffer);
  glDeleteTextures(1, &m_AlbedoBuffer);
  glDeleteRenderbuffers(1, &m_DepthBuffer);
  glDeleteFramebuffers(1, &m_GeometryBuffer);

  return true;
}

bool DeferredScene::Update(const Timer& timer)
{
  //float totalTime = timer.GetTotalTime();

  return true;
}

bool DeferredScene::Draw(const Camera& camera)
{
  // Collect camera information.
  glm::vec3 cameraPosition = camera.GetPosition();
  glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 projection = camera.GetProjectionMatrix();

  // Draw the geometry.
  glBindFramebuffer(GL_FRAMEBUFFER, m_GeometryBuffer);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, Globals::WindowWidth, Globals::WindowHeight);

  m_GeometryShader.Use();
  m_GeometryShader.SetUniform("view", view);
  m_GeometryShader.SetUniform("projection", projection);

  m_DiffuseTexture.Bind(0);
  m_SpecularTexture.Bind(1);

  for (GLuint i = 0; i < m_BoxModelMatrices.size(); ++i)
  {
    m_GeometryShader.SetUniform("model", m_BoxModelMatrices[i]);

    m_CubeBufferObject.Draw();
  }

  // Draw the deferred lighting.
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, Globals::WindowWidth, Globals::WindowHeight);

  m_DeferredShader.Use();
  m_DeferredShader.SetUniform("viewPosition", cameraPosition);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_PositionBuffer);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_NormalBuffer);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, m_AlbedoBuffer);
  glActiveTexture(GL_TEXTURE0);

  m_PlaneBufferObject.Draw();

  //// Draw the light boxes.
  //glBindFramebuffer(GL_FRAMEBUFFER, 0);
  ////glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  ////glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ////glViewport(0, 0, Globals::WindowWidth, Globals::WindowHeight);

  //m_BasicShader.Use();
  //m_BasicShader.SetUniform("view", view);
  //m_BasicShader.SetUniform("projection", projection);

  //for (GLuint i = 0; i < c_LightCount; ++i)
  //{
  //  m_BasicShader.SetUniform("model", glm::scale(glm::translate(glm::mat4(), m_LightPositions[i]), glm::vec3(0.25f)));
  //  m_BasicShader.SetUniform("solidColor", m_LightColors[i]);

  //  m_CubeBufferObject.Draw();
  //}

  return true;
}

void DeferredScene::KeyboardCallback(int key, int scancode, int action, int mode)
{
  if (action == GLFW_PRESS || action == SDL_PRESSED)
  {
    if (key == GLFW_KEY_SPACE || key == SDLK_SPACE)
    {
      SDL_Log("Space Hit!");
    }
    else if (key == GLFW_KEY_Q || key == SDLK_q)
    {
    }
    else if (key == GLFW_KEY_E || key == SDLK_e)
    {
    }
  }
}

bool DeferredScene::LoadTextures()
{
  if (!m_DiffuseTexture.Load("res/textures/container3.png", TextureType::Diffuse))
  {
    std::cout << "Failed to load texture container3.png." << std::endl;

    return false;
  }

  if (!m_SpecularTexture.Load("res/textures/specular.png", TextureType::Specular))
  {
    std::cout << "Failed to load texture specular.png." << std::endl;

    return false;
  }

  return true;
}

bool DeferredScene::LoadShaders()
{
  if (!m_GeometryShader.Load("res/shaders/standard.vs", "res/shaders/gbuffer.fs"))
  {
    std::cout << "Failed to load shader gbuffer." << std::endl;

    return false;
  }
  m_GeometryShader.Use();
  m_GeometryShader.SetUniform("diffuseTexture", 0);
  m_GeometryShader.SetUniform("specularTexture", 1);

  if (!m_DeferredShader.Load("res/shaders/quad.vs", "res/shaders/deferred.fs"))
    //if (!m_DeferredShader.Load("res/shaders/quad.vs", "res/shaders/quad.fs"))
  {
    std::cout << "Failed to load shader quad." << std::endl;

    return false;
  }
  m_DeferredShader.Use();
  //m_DeferredShader.SetUniform("sourceTexture", 0);
  m_DeferredShader.SetUniform("gPosition", 0);
  m_DeferredShader.SetUniform("gNormal", 1);
  m_DeferredShader.SetUniform("gAlbedo", 2);
  for (GLuint i = 0; i < c_LightCount; ++i)
  {
    m_DeferredShader.SetUniform("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
    m_DeferredShader.SetUniform("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
    m_DeferredShader.SetUniform("lights[" + std::to_string(i) + "].Constant", 1.0f);
    m_DeferredShader.SetUniform("lights[" + std::to_string(i) + "].Linear", 0.7f);
    m_DeferredShader.SetUniform("lights[" + std::to_string(i) + "].Quadratic", 1.8f);
  }

  if (!m_BasicShader.Load("res/shaders/basic.vs", "res/shaders/basic.fs"))
  {
    std::cout << "Failed to load shader basic." << std::endl;

    return false;
  }

  return true;
}

bool DeferredScene::LoadBufferObjects()
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

  m_CubeBufferObject.Construct(8, cubeVertices, cubeIndices);
  m_CubeBufferObject.EnableAttribute(0, 3);
  m_CubeBufferObject.EnableAttribute(1, 2);
  m_CubeBufferObject.EnableAttribute(2, 3);

  m_PlaneBufferObject.Construct(4, planeVertices, planeIndices);
  m_PlaneBufferObject.EnableAttribute(0, 2);
  m_PlaneBufferObject.EnableAttribute(1, 2);

  return true;
}

bool DeferredScene::LoadFrameBuffers()
{
  glGenFramebuffers(1, &m_GeometryBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_GeometryBuffer);

  glGenTextures(1, &m_PositionBuffer);
  glBindTexture(GL_TEXTURE_2D, m_PositionBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Globals::WindowWidth, Globals::WindowHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PositionBuffer, 0);

  glGenTextures(1, &m_NormalBuffer);
  glBindTexture(GL_TEXTURE_2D, m_NormalBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Globals::WindowWidth, Globals::WindowHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalBuffer, 0);

  glGenTextures(1, &m_AlbedoBuffer);
  glBindTexture(GL_TEXTURE_2D, m_AlbedoBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Globals::WindowWidth, Globals::WindowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_AlbedoBuffer, 0);

  glGenRenderbuffers(1, &m_DepthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Globals::WindowWidth, Globals::WindowHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

  GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
  glDrawBuffers(3, attachments);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "Final Framebuffer not complete!" << std::endl;

    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}
