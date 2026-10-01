#include "Model.h"

#include <stb_image.h>

#include <algorithm>
#include <cfloat>

namespace {
glm::mat4 toGLM(const aiMatrix4x4& m){
  glm::mat4 result;
  for(unsigned int c = 0; c < 4; c++)
    for(unsigned int r = 0; r < 4; r++)
      result[c][r] = m[r][c];
  return result;
}
}

void Model::Draw(Shader& shader){
  for(unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].Draw(shader);
}

void Model::loadModel(std::string path){
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));

  glm::vec3 minPos(FLT_MAX), maxPos(-FLT_MAX);
  computeBounds(scene->mRootNode, scene, glm::mat4(1.0f), minPos, maxPos);

  glm::vec3 size = maxPos - minPos;
  glm::vec3 center = (maxPos + minPos) * 0.5f;
  float maxDim = std::max({size.x, size.y, size.z});
  float scale = maxDim > 0.0f ? 2.0f / maxDim : 1.0f;

  transform = glm::scale(glm::mat4(1.0f), glm::vec3(scale)) * glm::translate(glm::mat4(1.0f), -center);

  processNode(scene->mRootNode, scene, glm::mat4(1.0f));

  std::cout << "Model loaded: " << meshes.size() << " meshes, scaled by "
            << scale << ", recentered at " << center.x << ", " << center.y << ", " << center.z << std::endl;
}

void Model::computeBounds(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform, glm::vec3& minPos, glm::vec3& maxPos){
  glm::mat4 worldTransform = parentTransform * toGLM(node->mTransformation);

  for(unsigned int i = 0; i < node->mNumMeshes; i++){
    const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    for(unsigned int v = 0; v < mesh->mNumVertices; v++){
      glm::vec3 pos = glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
      pos = glm::vec3(worldTransform * glm::vec4(pos, 1.0f));

      minPos = glm::min(minPos, pos);
      maxPos = glm::max(maxPos, pos);
    }
  }

  for(unsigned int i = 0; i < node->mNumChildren; i++){
    computeBounds(node->mChildren[i], scene, worldTransform, minPos, maxPos);
  }
}

void Model::processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform){
  glm::mat4 worldTransform = parentTransform * toGLM(node->mTransformation);

  for(unsigned int i=0;i<node->mNumMeshes;i++){
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene, transform * worldTransform));
  }

  for(unsigned int i=0;i<node->mNumChildren;i++){
    processNode(node->mChildren[i], scene, worldTransform);
  }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& meshTransform){
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(meshTransform)));

  for(unsigned int i=0;i<mesh->mNumVertices;i++){
    Vertex vertex{};
    vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
    if(mesh->HasNormals()){
      vertex.Normal = glm::normalize(normalMatrix * glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
    }
    else{
      vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    
    if(mesh->mTextureCoords[0]){
      vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    else{
      vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    }
    vertices.push_back(vertex);
  }

  for(unsigned int i=0;i<mesh->mNumFaces;i++){
    aiFace face = mesh->mFaces[i];
    for(unsigned int j=0;j<face.mNumIndices;j++){
      indices.push_back(face.mIndices[j]);
    }
  }

  glm::vec3 baseColor(0.44f, 0.44f, 0.44f);

  if(mesh->mMaterialIndex >= 0){
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    aiColor4D diffuse;
    if(aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS){
      baseColor = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
    }

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", scene);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
  }

  return Mesh(vertices, indices, textures, meshTransform, baseColor);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene){
  std::vector<Texture> textures;
  for(unsigned int i=0;i<mat->GetTextureCount(type);i++){
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;
    for(unsigned int j=0;j<textures_loaded.size();j++){
      if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0){
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    if(!skip){
      Texture texture{};
      texture.id = TextureFromFile(str.C_Str(), scene);
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      textures_loaded.push_back(texture);
    }
  }
  return textures;
}

unsigned int Model::TextureFromFile(const char* path, const aiScene* scene){
  int width, height, nrComponents;
  unsigned char* data = nullptr;

  std::string texturePath = path;

  if(!texturePath.empty() && texturePath[0] == '*'){
    const aiTexture* embedded = scene->GetEmbeddedTexture(path);
    if(!embedded){
      std::cout << "Embedded texture not found: " << texturePath << std::endl;
      return 0;
    }

    if(embedded->mHeight == 0){
      data = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(embedded->pcData), embedded->mWidth, &width, &height, &nrComponents, 0);
    }
    else if(std::strncmp(embedded->achFormatHint, "rgba8888", 8) == 0){
      width = embedded->mWidth;
      height = embedded->mHeight;
      nrComponents = 4;
      data = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(embedded->pcData), width * height * 4, &width, &height, &nrComponents, 0);
    }
    else{
      std::cout << "Unsupported embedded texture format: " << embedded->achFormatHint << std::endl;
      return 0;
    }
  }
  else{
    data = stbi_load((directory + '/' + texturePath).c_str(), &width, &height, &nrComponents, 0);
  }

  unsigned int textureID = 0;

  if(data){
    GLenum format;
    if(nrComponents == 1) format = GL_RED;
    else if(nrComponents == 2) format = GL_RG;
    else if(nrComponents == 3) format = GL_RGB;
    else format = GL_RGBA;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else{
    std::cout << "Texture failed to load at path: " << texturePath << std::endl;
  }

  return textureID;
}



