#include "Camera.h"
#include <GLFW/glfw3.h>

Camera::Camera(GLFWwindow* window, int width, int height){
  this->window = window;
  this->width = width;
  this->height = height;
}

void Camera::FreeCamera(Shader &shader){
  shader.bind();

  int newWidth, newHeight;
  glfwGetFramebufferSize(window, &newWidth, &newHeight);

  if (newWidth != width || newHeight != height){
    width = newWidth;
    height = newHeight;
    glViewport(0, 0, width, height);
  }

  float aspect = height > 0 ? (float)width / (float)height : 1.0f;

  view = glm::lookAt(cameraPos, cameraPos + cameraFront, camaraUp);
  projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    cameraPos += cameraSpeed * cameraFront;
  }
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
    cameraPos -= cameraSpeed * cameraFront;
  }
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    cameraPos -= glm::normalize(glm::cross(cameraFront, camaraUp)) * cameraSpeed;
  }
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
    cameraPos += glm::normalize(glm::cross(cameraFront, camaraUp)) * cameraSpeed;
  }

  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if(firstMouse){
      firstMouse = false;
      lastX = mouseX;
      lastY = mouseY;
    }

    float xoffset = mouseX - lastX;
    float yoffset = lastY - mouseY;
    lastX = mouseX;
    lastY = mouseY;

    float sensitivity = 0.1f;
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
  }
  else{
    firstMouse = true;
  }


  shader.setMat4("view", view);
  shader.setMat4("projection", projection);

  shader.unbind();
}
