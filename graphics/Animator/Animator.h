#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Animation/Animation.h>
#include <vector>

class Animator
{
public:
    Animator(Animation* animation);
    
    void UpdateAnimation(float dt);
    void PlayAnimation(Animation* animation);
    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
    void CalculateBindTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
    void SetRotation(const glm::mat4& rotation) { m_Rotation = rotation; }
    std::vector<glm::mat4> GetFinalBoneMatrices() {
      std::vector<glm::mat4> out;
      out.reserve(m_FinalBoneMatrices.size());
      for(const auto& m : m_FinalBoneMatrices) out.push_back(m_Rotation * m);
      return out;
    }

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;
    glm::mat4 m_Rotation = glm::mat4(1.0f);
};
