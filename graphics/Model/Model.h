#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>

#include <Shader/Shader.h>
#include <Mesh/Mesh.h>

class Model{
public:
  Model(std::string path) { loadModel(path); };

  void Draw(Shader& shader);
private:
  std::vector<Mesh> meshes;
  std::vector<Texture> textures_loaded;
  std::string directory;
  glm::mat4 transform = glm::mat4(1.0f);
  void loadModel(std::string path);

  void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& meshTransform);
  void computeBounds(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform, glm::vec3& minPos, glm::vec3& maxPos);

  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
  unsigned int TextureFromFile(const char* path, const aiScene* scene);
};
