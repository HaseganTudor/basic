#pragma once 

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Mesh/Mesh.h>
#include <Model/Model.h>

struct KeyPosition{
  glm::vec3 position;
  float timeStamp;
};

struct KeyRotation{
  glm::quat orientation;
  float timeStamp;
};

struct KeyScale{
  glm::vec3 scale;
  float timeStamp;
};

class Bone{
private:
  std::vector<KeyPosition> m_Positions;
  std::vector<KeyRotation> m_Rotations;
  std::vector<KeyScale> m_Scales;

  int m_NumPositions;
  int m_NumRotations;
  int m_NumScales;

  glm::mat4 m_LocalTransform;
  std::string m_Name;
  int m_ID;

public:
  Bone(const std::string& name, int ID, const aiNodeAnim* channel);
  void Update(float animationTime);
  glm::mat4 GetLocalTransform() { return m_LocalTransform; }
  std::string GetBoneName() const { return m_Name; }
  int GetBoneID() { return m_ID; }
  int GetPositionIndex(float animationTime);
  int GetRotationIndex(float animationTime);
  int GetScaleIndex(float animationTime);

private:
  float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) { 
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
  }
  glm::mat4 InterpolatePosition(float animationTime);
  glm::mat4 InterpolateRotation(float animationTime);
  glm::mat4 InterpolateScaling(float animationTime);
};
