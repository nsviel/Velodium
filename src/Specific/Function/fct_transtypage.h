#ifndef TRANSTYPAGE_FCT_H
#define TRANSTYPAGE_FCT_H

#include <bits/stdc++.h>

/**
 * \namespace Transtypage
 * \brief Function to convert data format between libraries
 */

namespace{

  //EIGEN TO EIGEN
  Eigen::Vector4d eigen_4d(Eigen::Vector3d in){
    Eigen::Vector4d out = Eigen::Vector4d::Zero();
    //---------------------------

    for(int i=0; i<3; i++){
      out(i) = in[i];
    }

    //---------------------------
    return out;
  }

  //EIGEN to GLM
  Cloud eigen_to_glm(Eigen::MatrixXf mat_in){
    Cloud cloud;
    std::vector<glm::vec3>& XYZ = cloud.xyz;
    //---------------------------

    for(int i=0; i<mat_in.rows(); i++){
      for(int j=0; j<mat_in.cols(); j++){
        XYZ[i][j] = mat_in(i,j);
      }
    }

    //---------------------------
    return cloud;
  }
  glm::mat4 eigen_to_glm_mat4(Eigen::Matrix4f mat_eig){
    glm::mat4 mat_glm;
    //---------------------------

    for(int i=0; i<mat_eig.rows(); i++){
      for(int j=0; j<mat_eig.cols(); j++){
        mat_glm[i][j] = mat_eig(i,j);
      }
    }

    //---------------------------
    return mat_glm;
  }
  glm::mat4 eigen_to_glm_mat4(Eigen::Matrix4d mat_eig){
    glm::mat4 mat_glm;
    //---------------------------

    for(int i=0; i<mat_eig.rows(); i++){
      for(int j=0; j<mat_eig.cols(); j++){
        mat_glm[i][j] = mat_eig(i,j);
      }
    }

    //---------------------------
    return mat_glm;
  }
  glm::mat4 eigen_to_glm_mat4(Eigen::Matrix3f mat_eig){
    glm::mat4 mat_glm(1.0f);
    //---------------------------

    for(int i=0; i<mat_eig.rows(); i++){
      for(int j=0; j<mat_eig.cols(); j++){
        mat_glm[i][j] = mat_eig(i,j);
      }
    }

    //---------------------------
    return mat_glm;
  }
  glm::mat4 eigen_to_glm_mat4(Eigen::Matrix3d mat_eig){
    glm::mat4 mat_glm(1.0f);
    //---------------------------

    for(int i=0; i<mat_eig.rows(); i++){
      for(int j=0; j<mat_eig.cols(); j++){
        mat_glm[i][j] = mat_eig(i,j);
      }
    }

    //---------------------------
    return mat_glm;
  }
  glm::vec3 eigen_to_glm_vec3(Eigen::Vector3f eig_vec){
    glm::vec3 glm_vec = glm::vec3(eig_vec(0), eig_vec(1), eig_vec(2));
    return glm_vec;
  }
  glm::vec3 eigen_to_glm_vec3(Eigen::Vector3d eig_vec){
    glm::vec3 glm_vec = glm::vec3(eig_vec(0), eig_vec(1), eig_vec(2));
    return glm_vec;
  }
  std::vector<glm::vec3> eigen_to_glm_vectorVec3(std::vector<Eigen::Vector3f> eig_vec, int nb_thread){
    vector<glm::vec3> glm_vec(eig_vec.size());
    //---------------------------

    #pragma omp parallel for num_threads(nb_thread)
    for(int i=0; i<eig_vec.size(); i++){
      Eigen::Vector3f& element = eig_vec[i];
      glm_vec[i] = glm::vec3(element(0), element(1), element(2));
    }

    //---------------------------
    return glm_vec;
  }
  std::vector<glm::vec3> eigen_to_glm_vectorVec3(std::vector<Eigen::Vector3d> eig_vec, int nb_thread){
    vector<glm::vec3> glm_vec(eig_vec.size());
    //---------------------------

    #pragma omp parallel for num_threads(nb_thread)
    for(int i=0; i<eig_vec.size(); i++){
      Eigen::Vector3d& element = eig_vec[i];
      glm_vec[i] = glm::vec3(element(0), element(1), element(2));
    }

    //---------------------------
    return glm_vec;
  }

  //GLM to Eigen
  Eigen::MatrixXf glm_to_eigen(const std::vector<glm::vec3>& XYZ){
    //---------------------------
    int size = XYZ.size();
    Eigen::MatrixXf cloud = Eigen::MatrixXf::Zero(size,3);
    //---------------------------

    for(int i=0; i<size; i++){
      for(int j=0; j<3; j++){
        cloud(i,j) = XYZ[i][j];
      }
    }

    //---------------------------
    return cloud;
  }
  Eigen::Vector3f glm_to_eigen_vec3(const glm::vec3& vec_glm){
    Eigen::Vector3f vec_eig = Eigen::Vector3f::Zero();
    //---------------------------

    for(int i=0; i<3; i++){
      vec_eig(i) = vec_glm[i];
    }

    //---------------------------
    return vec_eig;
  }
  Eigen::Vector4d glm_to_eigen_vec3_4d(const glm::vec3& vec, float scalar){
    Eigen::Vector4d vec_eig = Eigen::Vector4d::Zero();
    //---------------------------

    for(int i=0; i<3; i++){
      vec_eig(i) = vec[i];
    }
    vec_eig(3) = scalar;

    //---------------------------
    return vec_eig;
  }
  Eigen::Vector3f glm_to_eigen_mat4_vec3(const glm::mat4& mat_glm){
    Eigen::Vector3f vec_eig = Eigen::Vector3f::Zero();
    //---------------------------

    for(int i=0; i<3; i++){
      vec_eig(i) = mat_glm[i][3];
    }

    //---------------------------
    return vec_eig;
  }
  Eigen::Vector3f glm_to_eigen_mat4_vec3d(const glm::mat4& mat_glm){
    Eigen::Vector3f vec_eig = Eigen::Vector3f::Zero();
    //---------------------------

    for(int i=0; i<3; i++){
      vec_eig(i) = mat_glm[i][3];
    }

    //---------------------------
    return vec_eig;
  }
  Eigen::Matrix4f glm_to_eigen_mat4_mat4f(const glm::mat4& mat_glm){
    Eigen::Matrix4f mat_eig = Eigen::Matrix4f::Zero();
    //---------------------------

    for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
        mat_eig(i,j) = mat_glm[i][j];
      }
    }

    //---------------------------
    return mat_eig;
  }
  Eigen::Matrix4d glm_to_eigen_mat4_mat4d(const glm::mat4& mat_glm){
    Eigen::Matrix4d mat_eig = Eigen::Matrix4d::Zero();
    //---------------------------

    for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
        mat_eig(i,j) = mat_glm[i][j];
      }
    }

    //---------------------------
    return mat_eig;
  }
  Eigen::Matrix3f glm_to_eigen_mat4_mat3d(const glm::mat4& mat_glm){
    Eigen::Matrix3f mat_eig = Eigen::Matrix3f::Zero();
    //---------------------------

    for(int i=0; i<3; i++){
      for(int j=0; j<3; j++){
        mat_eig(i,j) = mat_glm[i][j];
      }
    }

    //---------------------------
    return mat_eig;
  }
  Eigen::MatrixXf glm_to_eigen_mat4_matXf(const glm::mat4& mat_glm){
    Eigen::MatrixXf mat_eig(4,4);
    //---------------------------

    for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
        mat_eig(i,j) = mat_glm[i][j];
      }
    }

    //---------------------------
    return mat_eig;
  }

  //STD to GLM
  glm::mat4 char_to_glm_mat4(char* mat_char){
    //---------------------------

    //Convert char* to string
    std::string str;
    str = mat_char;

    //Convert string to mat4
    std::istringstream iss(str);
    float m0, m1, m2, m3;
    float m4, m5, m6, m7;
    float m8, m9, m10, m11;
    float m12, m13, m14, m15;
    iss >> m0 >> m1 >> m2 >> m3;
    iss >> m4 >> m5 >> m6 >> m7;
    iss >> m8 >> m9 >> m10 >> m11;
    iss >> m12 >> m13 >> m14 >> m15;

    glm::mat4 mat;
    mat[0][0] = m0; mat[0][1] = m1; mat[0][2] = m2; mat[0][3] = m3;
    mat[1][0] = m4; mat[1][1] = m5; mat[1][2] = m6; mat[1][3] = m7;
    mat[2][0] = m8; mat[2][1] = m9; mat[2][2] = m10; mat[2][3] = m11;
    mat[3][0] = m12; mat[3][1] = m13; mat[3][2] = m14; mat[3][3] = m15;

    //---------------------------
    return mat;
  }

  //Specific
  vector<float> vec_double_to_float(vector<double> vec){
    vector<float> vec_out;
    //---------------------------

    for(int i=0; i<vec.size(); i++){
      vec_out.push_back(vec[i]);
    }

    //---------------------------
    return vec_out;
  }
  bool string_to_bool(std::string const& s){
    bool sucess = false;
    if(s == "true"){
      sucess = true;
    }
    return sucess;
  }
}

#endif
