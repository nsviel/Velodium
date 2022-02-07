#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <Eigen/Dense>
#include <string>
#include <vector>
#include <list>


struct struct_location{
  //---------------------------

  GLuint VBO;

  std::vector<glm::vec3> Initial;
  std::vector<glm::vec3> OBJ;
  std::vector<glm::vec3> Buffer;

  glm::vec3 Min;
  glm::vec3 Max;
  glm::vec3 COM;
  glm::vec3 COM_initial;
  glm::vec3 root;

  //---------------------------
};

struct struct_intensity{
  //---------------------------

  bool hasData;
  bool heatmap;

  std::vector<float> Initial;
  std::vector<float> OBJ;
  std::vector<float> Buffer;

  //---------------------------
};

struct struct_normal{
  //---------------------------

  bool hasData;

  std::vector<glm::vec3> Initial;
  std::vector<glm::vec3> OBJ;
  std::vector<glm::vec3> Buffer;

  //---------------------------
};

struct struct_color{
  //---------------------------

  GLuint VBO;
  bool hasData;
  glm::vec3 uniColor;

  std::vector<glm::vec4> Initial;
  std::vector<glm::vec4> OBJ;
  std::vector<glm::vec4> Buffer;

  //---------------------------
};

struct struct_transformation{
  //---------------------------

  glm::mat4 Scale;
  glm::mat4 Rotation;
  glm::mat4 Translation;
  glm::mat4 TransformationMatrix;
  glm::mat4 RealTransformation;

  //---------------------------
};

struct struct_additionnal{
  //---------------------------

  std::vector<float> dist;
  std::vector<float> cosIt;
  std::vector<float> It;
  std::vector<int> list_selectPoints;
  std::list<int> list_idxPoints;

  //---------------------------
};

struct struct_keypoints{
  //---------------------------

  std::vector<glm::vec3> keypoints;
  std::vector<float> keypoint_weight;
  std::vector<float> keypoint_intensity;
  std::vector<glm::vec3> trgpoints;
  std::vector<glm::vec4> XYZI;
  std::vector<glm::vec3> XYZ_groundTruth;

  //---------------------------
};

struct struct_timestamp{
  //---------------------------

  bool hasData;

  std::vector<float> Initial;
  std::vector<float> OBJ;
  std::vector<float> Buffer;

  //---------------------------
};



#endif
