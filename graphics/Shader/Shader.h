#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>

class Shader{
public:
  Shader(const char* vertexPath, const char* fragmentPath);

  void bind();
  void unbind();

  void setInt(const std::string& name, int value);
  void setMat4(const std::string& name, const glm::mat4& mat);
  void setVec3(const std::string& name, const glm::vec3& value);

  unsigned int getID() const { return id; }

private:
  unsigned int id;

  std::string readFile(const char* path);
};
