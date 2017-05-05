// Model.hpp

#ifndef __MODEL_HPP__
#define __MODEL_HPP__

// GLM Includes
#include <glm/glm.hpp>
// Assimp Includes
#include <assimp\scene.h>
// STL Includes
#include <memory>
#include <vector>
// Project Includes
#include "Mesh.hpp"

class Model
{
public:
  Model();
  ~Model();

  bool Load(const std::string& path);

  void Draw(Shader& shader);

private:
  void ProcessNode(aiNode* node, const aiScene* scene);
  void ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType textureType);

  std::vector<std::unique_ptr<Mesh>> m_Meshes;
  std::string m_Directory;
  std::vector<std::shared_ptr<Texture>> m_LoadedTextures;
};

#endif
