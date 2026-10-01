#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader/Shader.h>
#include <Mesh/Mesh.h>
#include <Camera/Camera.h>
#include <Model/Model.h>

int main(){
  glfwInit();
  
  int width = 600, height = 600;

  GLFWwindow* window = glfwCreateWindow(width,height, "basic", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ return -1; }

  glEnable(GL_DEPTH_TEST);

  Shader shader("resources/shaders/vert.glsl", "resources/shaders/frag.glsl");
  Camera camera(window, width, height);
  Model girl("resources/models/CesiumMan.glb");

  glClearColor(0.1f, 0.1f,0.1f,1.0f);
  while(!glfwWindowShouldClose(window)){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.FreeCamera(shader);
    girl.Draw(shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }


  return 0;
}
