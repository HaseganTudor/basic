#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

class Shader{
public:
  Shader(const char* vertexPath, const char* fragmentPath);

  void bind();
  void unbind();

  void setInt(const std::string& name, int value);
  void setMat4(const std::string& name, const glm::mat4& mat);
  void setVec3(const std::string& name, const glm::vec3& value);
  void setBool(const std::string& name, bool value);

  // Uploads all matrices of an array uniform in a single driver call.
  void setMat4Array(const std::string& name, const std::vector<glm::mat4>& mats);

  unsigned int getID() const { return id; }

private:
  unsigned int id;

  // Uniform locations are looked up once and cached; glGetUniformLocation is a
  // relatively slow string lookup and these setters are called every frame.
  std::unordered_map<std::string, int> uniformCache;
  int getUniformLocation(const std::string& name);

  std::string readFile(const char* path);
};
