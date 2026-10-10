#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader/Shader.h>
#include <Mesh/Mesh.h>
#include <Camera/Camera.h>
#include <Model/Model.h>
#include <Animation/Animation.h>
#include <Animator/Animator.h>

int main(){
  glfwInit();
  
  int width = 600, height = 600;

  GLFWwindow* window = glfwCreateWindow(width,height, "basic", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ return -1; }

  glEnable(GL_DEPTH_TEST);

  Shader shader("resources/shaders/vert.glsl", "resources/shaders/frag.glsl");
  Camera camera(window, width, height);
  Model man("resources/models/CesiumMan.glb");
  Animation animation("resources/models/CesiumMan.glb", &man, 0);
  Animator animator(&animation);
  man.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(180.0f));

  double lastFrame = 0.0f;
  double currentFrame = 0.0f;

  glClearColor(0.1f, 0.1f,0.1f,1.0f);
  while(!glfwWindowShouldClose(window)){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    animator.UpdateAnimation(deltaTime);

    camera.FreeCamera(shader);
    man.Draw(shader, animator);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }


  return 0;
}
