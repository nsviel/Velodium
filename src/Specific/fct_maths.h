#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include <numeric>

/**
 * \brief Basic math functions
 */

namespace{
  //---------------------------

  //Basic functions
  float fct_distance(glm::vec3 pt1, glm::vec3 pt2){
    //Euclidean distance
    float dist;
    //---------------------------

    dist = sqrt(pow(pt1.x - pt2.x,2) + pow(pt1.y - pt2.y,2) + pow(pt1.z - pt2.z,2));

    //---------------------------
    return dist;
  }
  float fct_distance_origin(Eigen::Vector3f pt1){
    //Euclidean distance
    float dist;
    //---------------------------

    dist = sqrt(pow(pt1(0), 2) + pow(pt1(1), 2) + pow(pt1(2), 2));

    //---------------------------
    return dist;
  }
  float fct_distance_origin(glm::vec3 pt1){
    //Euclidean distance
    float dist;
    //---------------------------

    dist = sqrt(pow(pt1.x, 2) + pow(pt1.y, 2) + pow(pt1.z, 2));

    //---------------------------
    return dist;
  }
  float fct_distance(Eigen::Vector3f pt1, Eigen::Vector3f pt2){
    //Euclidean distance
    //---------------------------

    float X = pt1(0) - pt2(0);
    float Y = pt1(1) - pt2(1);
    float Z = pt1(2) - pt2(2);

    float dist = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));

    //---------------------------
    return dist;
  }
  double fct_distance(Eigen::Vector3d pt1, Eigen::Vector3d pt2){
    //Euclidean distance
    //---------------------------

    double X = pt1(0) - pt2(0);
    double Y = pt1(1) - pt2(1);
    double Z = pt1(2) - pt2(2);

    double dist = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));

    //---------------------------
    return dist;
  }
  glm::vec3 fct_centroid(std::vector<glm::vec3>& vec){
    glm::vec3 centroid = glm::vec3(0, 0, 0);
    //---------------------------

    for(int i=0; i<vec.size(); i++){
      for(int j=0; j<3; j++){
        centroid[j] += vec[i][j];
      }
    }

    for(int j=0;j<3;j++){
      centroid[j] /= vec.size();
    }

    //---------------------------
    return centroid;
  }
  Eigen::Vector3f fct_centroid(std::vector<Eigen::Vector3f>& XYZ){
    Eigen::Vector3f centroid = Eigen::Vector3f::Zero();
    int size = XYZ.size();
    //---------------------------

    for(int i=0; i<size; i++){
      for(int j=0; j<3; j++){
        centroid(j) += XYZ[i](j);
      }
    }

    for(int i=0; i<3; i++){
      centroid(i) /= size;
    }

    //---------------------------
    return centroid;
  }
  Eigen::Vector3d fct_centroid(std::vector<Eigen::Vector3d>& XYZ){
    Eigen::Vector3d centroid = Eigen::Vector3d::Zero();
    int size = XYZ.size();
    //---------------------------

    for(int i=0; i<size; i++){
      for(int j=0; j<3; j++){
        centroid(j) += XYZ[i](j);
      }
    }

    for(int i=0; i<3; i++){
      centroid(i) /= (double) size;
    }

    //---------------------------
    return centroid;
  }
  template<typename Type> const Type fct_mean(std::vector<Type>& vec){
    int size = vec.size();
    float sum = 0;
    //---------------------------

    for(int i=0; i<size; i++){
      sum += vec[i];
    }
    float mean = sum / size;

    //---------------------------
    return mean;
  }
  template<typename Type> std::vector<Type> fct_inv(std::vector<Type>& vec){
    //Vector inversion
    std::vector<Type> vec_out;
    int size = vec.size();
    //---------------------------

    for(int i=0; i<size; i++){
      vec_out.push_back(vec[size-1-i]);
    }

    //---------------------------
    return vec_out;
  }
  template<typename Type> float fct_sum(std::vector<Type>& vec){
    //Sum of vector elements
    float out = 0;
    int size = vec.size();
    //---------------------------

    for(int i=0; i<size; i++){
      out = out + vec[i];
    }

    //---------------------------
    return out;
  }
  Eigen::Matrix3f fct_covarianceMat(std::vector<Eigen::Vector3f>& vec){
    //---------------------------

    // Centroide
    Eigen::Vector3f centroid = fct_centroid(vec);

    //Covariance matrix
    Eigen::Matrix3f covMat = Eigen::Matrix3f::Zero();
    for(int i=0; i<vec.size(); i++){
      for (int j=0; j<3; j++){
        for (int k=j; k<3; k++){
          Eigen::Vector3f point = vec[i];
          covMat(j, k) += (point(j) - centroid(j)) * (point(k) - centroid(k));
        }
      }
    }
    covMat(1, 0) = covMat(0, 1);
    covMat(2, 0) = covMat(0, 2);
    covMat(2, 1) = covMat(1, 2);

    //---------------------------
    return covMat;
  }
  Eigen::Matrix3d fct_covarianceMat(std::vector<Eigen::Vector3d>& vec){
    //---------------------------

    // Centroide
    Eigen::Vector3d centroid = fct_centroid(vec);

    //Covariance matrix
    Eigen::Matrix3d covMat = Eigen::Matrix3d::Zero();
    for(int i=0; i<vec.size(); i++){
      Eigen::Vector3d point = vec[i];

      for (int j=0; j<3; ++j){
        for (int k=j; k<3; ++k){
          covMat(j, k) += (point(j) - centroid(j)) * (point(k) - centroid(k));
        }
      }
    }
    covMat(1, 0) = covMat(0, 1);
    covMat(2, 0) = covMat(0, 2);
    covMat(2, 1) = covMat(1, 2);

    //---------------------------
    return covMat;
  }
  std::vector<float> fct_ones(int size){
    std::vector<float> vec;
    //---------------------------

    for(int i=0; i<size; i++){
      double value = 1;
      vec.push_back(value);
    }

    //---------------------------
    return vec;
  }
  bool fct_is_nan(glm::vec3 vec){
    //---------------------------

    if(isnan(vec[0]) || isnan(vec[1]) || isnan(vec[2])){
      return true;
    }

    //---------------------------
    return false;
  }
  bool fct_is_nan(Eigen::Vector3d vec){
    //---------------------------

    if(isnan(vec(0)) || isnan(vec(1)) || isnan(vec(2))){
      return true;
    }

    //---------------------------
    return false;
  }

  //Minimum / Maximum
  template<typename Type> const Type fct_max(std::vector<Type>& vec){
    Type max = vec[0];
    int size = vec.size();
    //---------------------------

    for(int i=0; i<size; i++)
      if(max < vec[i]) max = vec[i];

    //---------------------------
    return max;
  }
  template<typename Type> const Type fct_max_vec(std::vector<std::vector<Type>>& vec){
    Type max = vec[0].size();
    int size = vec.size();
    //---------------------------

    for(int i=0; i<size; i++){
      if(max < vec[i].size()) max = vec[i].size();
    }

    //---------------------------
    return max;
  }
  glm::vec2 fct_max_vec2(std::vector<glm::vec2> XY){
  glm::vec2 max = XY[0];
  int size = XY.size();
  //---------------------------

  for(int i=0; i<size; i++){
    for(int j=0; j<2; j++){
      if(XY[i][j] >= max[j]) max[j] = XY[i][j];
    }
  }

  //---------------------------
  return max;
}
  template<typename Type> const Type fct_min(std::vector<Type>& vec){
    Type min = vec[0];
    int size = vec.size();
    //---------------------------

    for(int i=0; i<size; i++){
      if(min > vec[i]) min = vec[i];
    }

    //---------------------------
    return min;
  }
  template<typename Type> const Type fct_min(Type in1, Type in2){
    //---------------------------

    if(in1 > in2){
      return in2;
    }else{
      return in1;
    }

    //---------------------------
  }
  glm::vec2 fct_min_vec2(std::vector<glm::vec2> XY){
  glm::vec2 min = XY[0];
  int size = XY.size();
  //---------------------------

  for(int i=0; i<size; i++){
    for(int j=0; j<2; j++){
      if(XY[i][j] <= min[j]) min[j] = XY[i][j];
    }
  }

  //---------------------------
  return min;
}
  glm::vec3 fct_min_vec3(std::vector<glm::vec3> XYZ){
    glm::vec3 min = XYZ[0];
    int size = XYZ.size();
    //---------------------------

    for(int i=0; i<size; i++){
      for(int j=0; j<3; j++){
        if ( XYZ[i][j] <= min[j] ) min[j] = XYZ[i][j];
      }
    }

    //---------------------------
    return min;
  }

  //Normalization
  template<typename Type> std::vector<Type> fct_normalize(std::vector<Type>& vec){
    std::vector<Type> vec_out(vec);
    int size = vec.size();
    //-----------------------------

    //Retrieve min & max
    Type min = vec[0];
    Type max = vec[0];
    for(int i=0; i<size; i++){
      if(vec[i] > max) max = vec[i];
      if(vec[i] < min) min = vec[i];
    }

    //Normalization
    for(int i=0; i<size; i++){
      vec_out[i] = (vec[i] - min) / (max - min);
    }

    //-----------------------------
    return vec_out;
  }
  template<typename Type> std::vector<Type> fct_normalize(std::vector<Type>& vec, glm::vec2 range){
    std::vector<Type> vec_out(vec);
    int size = vec.size();
    //-----------------------------

    //Retrieve min & max
    Type min = (Type)range.x;
    Type max = (Type)range.y;

    //Normalization
    for(int i=0; i<size; i++){
      vec_out[i] = (vec[i] - min) / (max - min);
    }

    //-----------------------------
    return vec_out;
  }
  template<typename Type> std::vector<Type> fct_normalize(std::vector<Type>& vec, float value_to_avoid){
    std::vector<Type> vec_out(vec);
    int size = vec.size();
    //-----------------------------

    //Retrieve min & max
    Type min = vec[0];
    Type max = vec[0];
    for(int i=0; i<size; i++){
      if(vec[i] != value_to_avoid){
        if(vec[i] > max) max = vec[i];
        if(vec[i] < min) min = vec[i];
      }
    }

    //Normalization
    for(int i=0; i<size; i++){
      if(vec[i] != value_to_avoid){
        vec_out[i] = (vec[i] - min) / (max - min);
      }else{
        vec_out[i] = vec[i];
      }
    }

    //-----------------------------
    return vec_out;
  }
  template<typename Type> std::vector<Type> fct_normalize_01(std::vector<Type>& vec){
    std::vector<Type> vec_out(vec);
    int size = vec.size();
    //-----------------------------

    //Retrieve min & max
    Type min = 1;
    Type max = 0;
    for(int i=0; i<size; i++){
      if(vec[i] > max) max = vec[i];
      if(vec[i] < min && vec[i] >= 0) min = vec[i];
    }

    //Normalization
    for(int i=0; i<size; i++){
      vec_out[i] = (vec[i] - min) / (max - min);
    }

    //-----------------------------
    return vec_out;
  }

  //3D functions
  float fct_dotProduct(glm::vec3 vec_A, glm::vec3 vec_B){
    float product = 0;
    //---------------------------

    // Loop for calculate cot product
    for(int i=0; i<3; i++){
      product = product + vec_A[i] * vec_B[i];
    }

    //---------------------------
    return product;
  }
  void fct_crossProduct(std::vector<float>& vec_A, std::vector<float>& vec_B, std::vector<float>& vec_cross){
    //---------------------------

    vec_cross[0] = vec_A[1] * vec_B[2] - vec_A[2] * vec_B[1];
    vec_cross[1] = vec_A[2] * vec_B[0] - vec_A[0] * vec_B[2];
    vec_cross[2] = vec_A[0] * vec_B[1] - vec_A[1] * vec_B[0];

    //---------------------------
  }

  //Statistical functions
  template<typename Type> float fct_std(std::vector<Type>& vec){
    //---> Standard deviation
    float sum = 0.0, mean, Std = 0.0;
    int size = vec.size();
    //-------------------

    for(int i=0; i<size; i++){
      sum += vec[i];
    }

    mean = sum/size;
    for(int i=0; i<size; i++){
      Std += pow(vec[i] - mean, 2);
    }

    //-------------------
    return sqrt(Std / vec.size());
  }
  template<typename Type> float fct_var(std::vector<Type>& vec){
    //---> Variance
    int size = vec.size();
    //-------------------

    //Mean
    float sum = 0.0, mean;
    for(int i=0; i<size; i++){
      sum += vec[i];
    }
    mean = sum/size;

    //Variance
    float var = 0.0;
    for(int i=0; i<size; i++){
      var += pow(vec[i] - mean, 2);
    }
    var = var / size;

    //-------------------
    return var;
  }
  template<typename Type> float fct_cv(std::vector<Type>& vec){
    //---> Coefficient of variation
    float std = fct_std(vec);
    float CV = (std / fct_mean(vec)) * 100;

    return CV;
  }
  float fct_R2(std::vector<float>& data_X, std::vector<float>& data_Y, std::vector<float>& coeffs){
    int size = data_Y.size();
    float up = 0.0f, bot = 0.0f, R_2;
    for(int i=0; i<size; i++)
    {
      if(coeffs.size() == 2) up += pow((data_Y[i] - (coeffs[1]*data_X[i] + coeffs[0])), 2);
      if(coeffs.size() == 3) up += pow((data_Y[i] - (coeffs[2]*pow(data_X[i],2) + coeffs[1]*data_X[i] + coeffs[0])), 2);
      bot += pow((data_Y[i] - fct_mean(data_Y)), 2);
    }
    R_2 = 1 - (up / bot);
    std::cout<<"---> R² = "<<R_2<<std::endl;

    return R_2;
  }

  /*Sorting functions
    --->Sort by order, keeping trace of indices
    Use with for (auto i: fct_sortByIndexes(v)) {
                cout << v[i] << endl;
              }
  */
  template <typename T> std::vector<size_t> fct_sortByIndexes(const std::vector<T> &v){
    // initialize original index locations
    std::vector<size_t> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    std::sort(idx.begin(), idx.end(),
         [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

    return idx;
  }
  template <typename T> std::vector<size_t> fct_sortByIndexes_greater(const std::vector<T> &v){
    //--->Sort by greater order, keeping trace of indices

    // initialize original index locations
    std::vector<size_t> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    std::sort(idx.begin(), idx.end(),
         [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});

    return idx;
  }
  void fct_sort_alpha_num(std::vector<std::string>& vec){
    //---------------------------

    std::sort(vec.begin(), vec.end(), [](const std::string& a, const std::string& b) {
      if (a[0] < b[0]) {
          return true;
      } else if (a[0] > b[0]) {
          return false;
      }
      string a_num = a.substr(0, a.find_last_of("."));
      a_num = a_num.substr(a.find_last_of("_") + 1);
      string b_num = b.substr(0, b.find_last_of("."));
      b_num = b_num.substr(b.find_last_of("_") + 1);
      return std::stoi(a_num) < std::stoi(b_num);
    });

    //---------------------------
  }

  //Geometric functions
  double fct_angularDistance(const Eigen::Matrix3f &rota, const Eigen::Matrix3f &rotb) {
    double norm = ((rota * rotb.transpose()).trace() - 1) / 2;
    norm = std::acos(norm) * 180 / M_PI;
    return norm;
  }

  //---------------------------
}

#endif
