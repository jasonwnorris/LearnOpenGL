// Model.cpp

// Assimp Includes
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
// STL Includes
#include <iostream>
// Project Includes
#include "Model.hpp"

Model::Model()
{
}

Model::~Model()
{
}

bool Model::Load(const std::string& path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (scene == nullptr || scene->mRootNode == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
  {
    std::cout << "Assimp failed to import file: " << importer.GetErrorString() << std::endl;
    return false;
  }

  m_Directory = path.substr(0, path.find_last_of('/'));

  ProcessNode(scene->mRootNode, scene);

  return true;
}

void Model::Draw(Shader& shader)
{
  for (auto&& mesh : m_Meshes)
  {
    mesh->Draw(shader);
  }
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
  for (GLuint i = 0; i < node->mNumMeshes; ++i)
  {
    ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene);
  }

  for (GLuint i = 0; i < node->mNumChildren; ++i)
  {
    ProcessNode(node->mChildren[i], scene);
  }
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
  std::vector<Vertex> vertices;
  for (GLuint i = 0; i < mesh->mNumVertices; ++i)
  {
    Vertex vertex;

    vertex.Position.x = mesh->mVertices[i].x;
    vertex.Position.y = mesh->mVertices[i].y;
    vertex.Position.z = mesh->mVertices[i].z;

    vertex.Normal.x = mesh->mNormals[i].x;
    vertex.Normal.y = mesh->mNormals[i].y;
    vertex.Normal.z = mesh->mNormals[i].z;

    if (mesh->mTextureCoords[0] != nullptr)
    {
      vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
      vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
    }

    vertices.push_back(vertex);
  }

  std::vector<GLuint> indices;
  for (GLuint i = 0; i < mesh->mNumFaces; ++i)
  {
    const aiFace& face = mesh->mFaces[i];

    for (GLuint j = 0; j < face.mNumIndices; ++j)
    {
      indices.push_back(face.mIndices[j]);
    }
  }

  std::vector<std::shared_ptr<Texture>> textures;
  if (mesh->mMaterialIndex >= 0)
  {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<std::shared_ptr<Texture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
    std::vector<std::shared_ptr<Texture>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  m_Meshes.push_back(std::unique_ptr<Mesh>(new Mesh(vertices, indices, textures)));
}

std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType textureType)
{
  std::vector<std::shared_ptr<Texture>> textures;
  for (GLuint i = 0; i < material->GetTextureCount(type); ++i)
  {
    aiString name;
    material->GetTexture(type, i, &name);

    std::string path = m_Directory + "/" + std::string(name.C_Str());

    auto iter = std::find_if(m_LoadedTextures.begin(), m_LoadedTextures.end(), [&path](auto&& texture) { return texture->GetPath() == path; });
    if (iter != m_LoadedTextures.end())
    {
      textures.push_back(*iter);
    }
    else
    {
      std::shared_ptr<Texture> texture(new Texture());
      if (!texture->Load(path, textureType))
      {
        std::cout << "Failed to load texture: " << path << std::endl;
      }

      textures.push_back(texture);

      m_LoadedTextures.push_back(texture);
    }
  }

  return textures;
}
