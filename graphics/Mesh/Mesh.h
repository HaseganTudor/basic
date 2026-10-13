#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

#include <Shader/Shader.h>

#define MAX_BONE_INFLUENCE 4

struct Vertex{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;

  int m_BoneIDs[MAX_BONE_INFLUENCE];
  float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture{
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh{
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  glm::mat4 transform;

  Mesh() = default;
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures,
      glm::mat4 transform = glm::mat4(1.0f), glm::vec3 baseColor = glm::vec3(1.0f))
    : vertices(vertices), indices(indices), textures(textures), transform(transform), baseColor(baseColor) 
    {
      setupMesh();
    };

  static Mesh Square();
  static Mesh Cube();

  void Draw(Shader& shader);

private:
  unsigned int VAO, VBO, EBO;
  glm::vec3 baseColor;

  void setupMesh();
};
