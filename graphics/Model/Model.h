#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>

#include <string>
#include <map>

#include <Shader/Shader.h>
#include <Mesh/Mesh.h>

struct BoneInfo{
  int id;
  glm::mat4 offset;
};

class Model{
public:
  Model(std::string path) { loadModel(path); };

  void Draw(Shader& shader, class Animator& animator);
  void Draw(Shader& shader);

  void rotate(const glm::vec3& axis, float angle) {
    m_rotation = glm::rotate(glm::mat4(1.0f), angle, axis) * m_rotation;
    for(auto& mesh : meshes){
      mesh.transform = m_rotation * mesh.transform;
    }
  }
private:
  std::vector<Mesh> meshes;
  std::vector<Texture> textures_loaded;
  std::string directory;
  glm::mat4 transform = glm::mat4(1.0f);
  glm::mat4 m_rotation = glm::mat4(1.0f);
  void loadModel(std::string path);

  void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& meshTransform);
  void computeBounds(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform, glm::vec3& minPos, glm::vec3& maxPos);

  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
  unsigned int TextureFromFile(const char* path, const aiScene* scene);

  // Bone data
  std::map<std::string, BoneInfo> m_BoneInfoMap;
  int m_BoneCounter = 0;
public:
  auto& GetBoneInfoMap() { return m_BoneInfoMap; }
  int& GetBoneCount() { return m_BoneCounter; }

private:
  void SetVertexBoneDataToDefault(Vertex& vertex);
  void ExtractBoneWeightData(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
  void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

};
