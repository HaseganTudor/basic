#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Shader/Shader.h>

class Camera{
public:
  Camera(GLFWwindow* window, int width, int height);
  
  void FreeCamera(Shader &shader);
private:
  GLFWwindow* window;
  glm::mat4 view, projection;
  
  int width, height;
  float yaw = -90.0f;
  float pitch = 0.0f;
  float lastX, lastY;
  float cameraSpeed = 0.05f;
  bool firstMouse = true;
 
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 camaraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, camaraUp));
};
