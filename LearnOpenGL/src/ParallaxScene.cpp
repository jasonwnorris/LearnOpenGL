// ParallaxScene.cpp

// GLM Includes
#include <glm/gtc/matrix_transform.hpp>
// STL Includes
#include <iostream>
// Project Includes
#include "Geometry.hpp"
#include "Globals.hpp"
#include "ParallaxScene.hpp"

ParallaxScene::ParallaxScene() :
  m_LightPosition(2.0f, 1.0f, -2.0f)
{
}

bool ParallaxScene::Initialize()
{
  // Create plane data.
  glm::vec3 position1(-1.0f, 1.0f, 0.0f);
  glm::vec3 position2(-1.0f, -1.0f, 0.0f);
  glm::vec3 position3(1.0f, -1.0f, 0.0f);
  glm::vec3 position4(1.0f, 1.0f, 0.0f);
  glm::vec2 uv1(0.0f, 1.0f);
  glm::vec2 uv2(0.0f, 0.0f);
  glm::vec2 uv3(1.0f, 0.0f);
  glm::vec2 uv4(1.0f, 1.0f);
  glm::vec3 normal(0.0f, 0.0f, 1.0f);

  glm::vec3 edge1 = position2 - position1;
  glm::vec3 edge2 = position3 - position1;
  glm::vec2 deltaUV1 = uv2 - uv1;
  glm::vec2 deltaUV2 = uv3 - uv1;

  GLfloat f1 = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

  glm::vec3 tangent1;
  tangent1.x = f1 * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
  tangent1.y = f1 * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
  tangent1.z = f1 * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
  tangent1 = glm::normalize(tangent1);

  glm::vec3 edge3 = position3 - position1;
  glm::vec3 edge4 = position4 - position1;
  glm::vec2 deltaUV3 = uv3 - uv1;
  glm::vec2 deltaUV4 = uv4 - uv1;

  GLfloat f2 = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

  glm::vec3 tangent2;
  tangent2.x = f2 * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
  tangent2.y = f2 * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
  tangent2.z = f2 * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
  tangent2 = glm::normalize(tangent2);

  std::vector<GLfloat> vertices = {
    // Positions                           // TexCoords  // Normals                    // Tangents
    position1.x, position1.y, position1.z, uv1.x, uv1.y, normal.x, normal.y, normal.z, tangent1.x, tangent1.y, tangent1.z,
    position2.x, position2.y, position2.z, uv2.x, uv2.y, normal.x, normal.y, normal.z, tangent1.x, tangent1.y, tangent1.z,
    position3.x, position3.y, position3.z, uv3.x, uv3.y, normal.x, normal.y, normal.z, tangent1.x, tangent1.y, tangent1.z,

    position1.x, position1.y, position1.z, uv1.x, uv1.y, normal.x, normal.y, normal.z, tangent2.x, tangent2.y, tangent2.z,
    position3.x, position3.y, position3.z, uv3.x, uv3.y, normal.x, normal.y, normal.z, tangent2.x, tangent2.y, tangent2.z,
    position4.x, position4.y, position4.z, uv4.x, uv4.y, normal.x, normal.y, normal.z, tangent2.x, tangent2.y, tangent2.z
  };

  std::vector<GLuint> indices = {
    0, 1, 2,
    3, 4, 5
  };

  // Load the textures.
  if (!m_DiffuseTexture.Load("res/textures/wood.png", TextureType::Diffuse))
  {
    std::cout << "Failed to load texture wood.png." << std::endl;

    return false;
  }

  if (!m_NormalTexture.Load("res/textures/toy_box_normal.png", TextureType::Normal))
  {
    std::cout << "Failed to load texture toy_box_normal.png." << std::endl;

    return false;
  }

  if (!m_DepthTexture.Load("res/textures/toy_box_depth.png", TextureType::Diffuse))
  {
    std::cout << "Failed to load texture toy_box_depth.png." << std::endl;

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

  if (!m_ParallaxShader.Load("res/shaders/parallax.vs", "res/shaders/parallax.fs"))
  {
    std::cout << "Failed to load shader parallax." << std::endl;

    return false;
  }
  m_ParallaxShader.Use();
  m_ParallaxShader.SetUniform("diffuseTexture", 0);
  m_ParallaxShader.SetUniform("normalTexture", 1);
  m_ParallaxShader.SetUniform("depthTexture", 2);

  // Create the buffer objects.
  m_CubeBufferObject.Construct(11, Geometry::CubeVertices, Geometry::CubeIndices);
  m_CubeBufferObject.EnableAttribute(0, 3);
  m_CubeBufferObject.EnableAttribute(1, 2);
  m_CubeBufferObject.EnableAttribute(2, 3);
  m_CubeBufferObject.EnableAttribute(3, 3);

  m_PlaneBufferObject.Construct(11, vertices, indices);
  m_PlaneBufferObject.EnableAttribute(0, 3);
  m_PlaneBufferObject.EnableAttribute(1, 2);
  m_PlaneBufferObject.EnableAttribute(2, 3);
  m_PlaneBufferObject.EnableAttribute(3, 3);

  return true;
}

bool ParallaxScene::Finalize()
{
  return true;
}

bool ParallaxScene::Update(const Timer& timer)
{
  float totalTime = timer.GetTotalTime();

  m_LightPosition.x = std::cosf(totalTime) * 1.5f;
  m_LightPosition.y = std::sinf(totalTime) * 1.5f;
  m_LightPosition.z = 1.0f;

  m_PlaneRotation = totalTime * -0.5f;

  return true;
}

bool ParallaxScene::Draw(const Camera& camera)
{
  glClearColor(0.125f, 0.125f, 0.125f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, Globals::WindowWidth, Globals::WindowHeight);

  // Create view and projection matrices.
  glm::vec3 cameraPosition = camera.GetPosition();
  glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 projection = camera.GetProjectionMatrix();

  // Draw cube.
  m_BasicShader.Use();
  m_BasicShader.SetUniform("model", glm::scale(glm::translate(glm::mat4(), m_LightPosition), glm::vec3(0.25f)));
  m_BasicShader.SetUniform("view", view);
  m_BasicShader.SetUniform("projection", projection);

  m_CubeBufferObject.Draw();

  // Draw plane.
  m_ParallaxShader.Use();
  m_ParallaxShader.SetUniform("model", glm::scale(glm::rotate(glm::mat4(), m_PlaneRotation, glm::vec3(0.0f, 0.0, 1.0f)), glm::vec3(2.5f)));
  m_ParallaxShader.SetUniform("view", view);
  m_ParallaxShader.SetUniform("projection", projection);
  m_ParallaxShader.SetUniform("lightPosition", m_LightPosition);
  m_ParallaxShader.SetUniform("viewPosition", cameraPosition);
  m_ParallaxShader.SetUniform("heightScale", 0.1f);

  m_DiffuseTexture.Bind(0);
  m_NormalTexture.Bind(1);
  m_DepthTexture.Bind(2);

  m_PlaneBufferObject.Draw();
  //m_CubeBufferObject.Draw();

  return true;
}
