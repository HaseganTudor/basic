#include "Animation.h"

Animation::Animation(const std::string& animationPath, Model* model, int index){
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
  assert(scene && scene->mRootNode);
  auto animation = scene->mAnimations[index];
  m_Duration = animation->mDuration;
  m_TicksPerSecond = animation->mTicksPerSecond;
  ReadHeirarchyData(m_RootNode, scene->mRootNode);
  ReadMissingBones(animation, *model);
}

Bone* Animation::FindBone(const std::string& name){
  auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
    [&](const Bone& Bone) {
      return Bone.GetBoneName() == name;
    }
  );
  if (iter == m_Bones.end()) return nullptr;
  else return &(*iter);
}

void Animation::ReadMissingBones(const aiAnimation* animation, Model& model){
  int size = animation->mNumChannels;

  auto &boneInfoMap = model.GetBoneInfoMap();
  int& boneCount = model.GetBoneCount();

  for(int i = 0; i < size; i++){
    auto channel = animation->mChannels[i];
    std::string boneName = channel->mNodeName.data;

    if(boneInfoMap.find(boneName) == boneInfoMap.end()){
      boneInfoMap[boneName].id = boneCount;
      boneCount++;
    }
    m_Bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
  }

  m_BoneInfoMap = boneInfoMap;
}

void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src){
  assert(src);

  dest.name = src->mName.data;
  dest.transformation = glm::transpose(glm::make_mat4(&src->mTransformation.a1));
  dest.childrenCount = src->mNumChildren;

  for(unsigned int i = 0; i < src->mNumChildren; i++){
    AssimpNodeData newData;
    ReadHeirarchyData(newData, src->mChildren[i]);
    dest.children.push_back(newData);
  }
}
