#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 768;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform bool isAnimated = false;

void main(){
  vec4 totalPosition = vec4(aPos, 1.0f);
  vec3 totalNormal = aNormal;

  if(isAnimated){
    totalPosition = vec4(0.0f);
    totalNormal = vec3(0.0f);

    for(int i = 0; i < MAX_BONE_INFLUENCE; i++){
      int boneId = boneIds[i];
      if(boneId < 0)
        continue;

      if(boneId >= MAX_BONES){
        totalPosition += vec4(aPos, 1.0f) * weights[i];
        totalNormal += aNormal * weights[i];
        continue;
      }

      mat4 boneMatrix = finalBonesMatrices[boneId];
      totalPosition += (boneMatrix * vec4(aPos, 1.0f)) * weights[i];
      totalNormal += (mat3(boneMatrix) * aNormal) * weights[i];
    }
  }

  vec4 worldPos = model * totalPosition;
  gl_Position = projection * view * worldPos;
  FragPos = worldPos.xyz;
  Normal = totalNormal;
  TexCoords = aTexCoords;
}
