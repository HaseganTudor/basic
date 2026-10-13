#include "Shader.h"
#include <fstream>

Shader::Shader(const char* vertPath, const char* fragPath){
  std::string vertCode = readFile(vertPath);
  std::string fragCode = readFile(fragPath); 

  const char* vShaderCode = vertCode.c_str();
  const char* fShaderCode = fragCode.c_str();

  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, nullptr);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, nullptr);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(id, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
    infoLog << std::endl;
  }
}

std::string Shader::readFile(const char* path){
  std::ifstream file; 
  file.open(path);
  if(!file.is_open()){
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
  std::string content;
  std::string line;
  while(std::getline(file, line)){
    content += line + "\n";
  }
  file.close();
  return content;
}

void Shader::bind(){
  glUseProgram(id);
}

void Shader::unbind(){
  glUseProgram(0);
}

int Shader::getUniformLocation(const std::string& name){
  auto it = uniformCache.find(name);
  if(it != uniformCache.end())
    return it->second;

  int location = glGetUniformLocation(id, name.c_str());
  uniformCache.emplace(name, location);
  return location;
}

void Shader::setInt(const std::string& name, int value){
  glUniform1i(getUniformLocation(name), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat){
  glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value){
  glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setBool(const std::string& name, bool value){
  glUniform1i(getUniformLocation(name), (int)value);
}

void Shader::setMat4Array(const std::string& name, const std::vector<glm::mat4>& mats){
  if(mats.empty())
    return;

  int location = getUniformLocation(name);
  if(location < 0)
    return;

  glUniformMatrix4fv(location, (GLsizei)mats.size(), GL_FALSE, glm::value_ptr(mats[0]));
}
