#ifndef REG_STRUCT_H
#define REG_STRUCT_H


struct Register{
  //---------------------------

  std::vector<glm::vec3> keypoints;
  std::vector<float> keypoint_weight;
  std::vector<float> keypoint_intensity;
  std::vector<glm::vec3> trgpoints;
  std::vector<glm::vec4> XYZI;
  std::vector<glm::vec3> XYZ_groundTruth;
  
  glm::mat4 Scale;
  glm::mat4 Rotation;
  glm::mat4 Translation;
  glm::mat4 TransformationMatrix;
  glm::mat4 RealTransformation;

  //---------------------------
};

#endif
