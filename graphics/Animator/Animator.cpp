#include "Animator.h"

Animator::Animator(Animation* animation)
{
    m_CurrentTime = 0.0;
    m_CurrentAnimation = animation;

    int boneCount = m_CurrentAnimation ? m_CurrentAnimation->GetBoneIDMap().size() : 0;
    m_FinalBoneMatrices.assign(boneCount, glm::mat4(1.0f));

    if (m_CurrentAnimation)
        CalculateBindTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
}

void Animator::UpdateAnimation(float dt)
{
    m_DeltaTime = dt;
    if (m_CurrentAnimation)
    {
        m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
        m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
        CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
    }
}

const std::vector<glm::mat4>& Animator::GetFinalBoneMatrices()
{
    if (m_Rotation == glm::mat4(1.0f))
        return m_FinalBoneMatrices;

    m_RotatedBoneMatrices.resize(m_FinalBoneMatrices.size());
    for (size_t i = 0; i < m_FinalBoneMatrices.size(); i++)
        m_RotatedBoneMatrices[i] = m_Rotation * m_FinalBoneMatrices[i];
    return m_RotatedBoneMatrices;
}

void Animator::PlayAnimation(Animation* animation)
{
    m_CurrentAnimation = animation;
    m_CurrentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    const std::string& nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* bone = m_CurrentAnimation->FindBone(nodeName);

    if (bone)
    {
        bone->Update(m_CurrentTime);
        nodeTransform = bone->GetLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    // Reference, not a copy: this used to duplicate the whole map per node.
    const auto& boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    auto boneIt = boneInfoMap.find(nodeName);
    if (boneIt != boneInfoMap.end())
    {
        int index = boneIt->second.id;
        glm::mat4 offset = boneIt->second.offset;
        m_FinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneTransform(&node->children[i], globalTransformation);
}

void Animator::CalculateBindTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    glm::mat4 globalTransformation = parentTransform * node->transformation;

    const auto& boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    auto boneIt = boneInfoMap.find(node->name);
    if (boneIt != boneInfoMap.end())
    {
        int index = boneIt->second.id;
        glm::mat4 offset = boneIt->second.offset;
        m_FinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBindTransform(&node->children[i], globalTransformation);
}
