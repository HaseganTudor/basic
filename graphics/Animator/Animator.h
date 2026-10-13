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
    const std::vector<glm::mat4>& GetFinalBoneMatrices();

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    std::vector<glm::mat4> m_RotatedBoneMatrices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;
    glm::mat4 m_Rotation = glm::mat4(1.0f);
};
