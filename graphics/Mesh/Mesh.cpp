#include "Mesh.h"
#include <vector>

void Mesh::setupMesh(){
  unsigned int VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader){
  shader.bind();

  shader.setMat4("model", transform);
  shader.setVec3("baseColor", baseColor);
  shader.setInt("useTexture", textures.empty() ? 0 : 1);

  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;

  for(unsigned int i = 0; i < textures.size(); i++){
    glActiveTexture(GL_TEXTURE0 + i);
    std::string number;
    std::string name = textures[i].type;
    if(name == "texture_diffuse") number = std::to_string(diffuseNr++);
    else if(name == "texture_specular") number = std::to_string(specularNr++);
    else if(name == "texture_normal") number = std::to_string(normalNr++);
  
 
    shader.setInt((name + number).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  shader.unbind();
}

Mesh Mesh::Square(){
  std::vector<Vertex> v = {
    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
  };
  std::vector<unsigned int> i = {
    0, 1, 2,
    2, 3, 0
  };

  std::vector<Texture> t = { };

  return Mesh(v, i, t);
}
