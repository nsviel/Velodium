#include "fct_math.h"


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
float fct_mean(std::vector<float>& vec){
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
float fct_sum(std::vector<float>& vec){
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
double fct_distance_origin(Eigen::Vector3d pt1){
  //Euclidean distance
  double dist;
  //---------------------------

  dist = sqrt(pow(pt1(0), 2) + pow(pt1(1), 2) + pow(pt1(2), 2));

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
double fct_distance(Eigen::Vector4d pt1, Eigen::Vector4d pt2){
  //Euclidean distance
  //---------------------------

  double X = pt1(0) - pt2(0);
  double Y = pt1(1) - pt2(1);
  double Z = pt1(2) - pt2(2);

  double dist = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));

  //---------------------------
  return dist;
}
double fct_distance(Eigen::Vector4d pt1, Eigen::Vector3d pt2){
  //Euclidean distance
  //---------------------------

  double X = pt1(0) - pt2(0);
  double Y = pt1(1) - pt2(1);
  double Z = pt1(2) - pt2(2);

  double dist = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));

  //---------------------------
  return dist;
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
std::string thousandSeparator(int n){
    std::string ans = "";

    // Convert the given integer
    // to equivalent string
    std::string num = std::to_string(n);

    // Initialise count
    int count = 0;

    // Traverse the string in reverse
    for (int i = num.size() - 1;
         i >= 0; i--) {
        count++;
        ans.push_back(num[i]);

        // If three characters
        // are traversed
        if (count == 3) {
            ans.push_back(' ');
            count = 0;
        }
    }

    // Reverse the string to get
    // the desired output
    std::reverse(ans.begin(), ans.end());

    // If the given string is
    // less than 1000
    if (ans.size() % 4 == 0) {

        // Remove ','
        ans.erase(ans.begin());
    }

    return ans;
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
glm::vec3 fct_centroid(glm::vec3& vec1, glm::vec3& vec2){
  glm::vec3 centroid = glm::vec3(0, 0, 0);
  //---------------------------

  centroid.x = (vec1.x + vec2.x) / 2;
  centroid.y = (vec1.y + vec2.y) / 2;
  centroid.z = (vec1.z + vec2.z) / 2;

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
std::vector<float> fct_inv(std::vector<float>& vec){
  //Vector inversion
  std::vector<float> vec_out;
  int size = vec.size();
  //---------------------------

  for(int i=0; i<size; i++){
    vec_out.push_back(vec[size-1-i]);
  }

  //---------------------------
  return vec_out;
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
std::vector<float> fct_crossProduct(std::vector<float>& vec_A, std::vector<float>& vec_B){
  std::vector<float> vec_cross;
  //---------------------------

  vec_cross[0] = vec_A[1] * vec_B[2] - vec_A[2] * vec_B[1];
  vec_cross[1] = vec_A[2] * vec_B[0] - vec_A[0] * vec_B[2];
  vec_cross[2] = vec_A[0] * vec_B[1] - vec_A[1] * vec_B[0];

  //---------------------------
  return vec_cross;
}

//Minimum / Maximum
int fct_min_z_id(std::vector<glm::vec3> vec){
  glm::vec3 min = vec[0];
  int id = 0;
  int size = vec.size();
  //---------------------------

  for(int i=0; i<size; i++){
    if(min.z > vec[i].z){
      min = vec[i];
      id = i;
    }
  }

  //---------------------------
  return id;
}
float fct_min(std::vector<float>& vec){
  float min = vec[0];
  int size = vec.size();
  //---------------------------

  for(int i=0; i<size; i++){
    if(min > vec[i]) min = vec[i];
  }

  //---------------------------
  return min;
}
float fct_min(float in1, float in2){
  //---------------------------

  if(in1 > in2){
    return in2;
  }else{
    return in1;
  }

  //---------------------------
}
float fct_max(std::vector<float>& vec){
  int size = vec.size();
  if(size == 0) return 0;
  //---------------------------

  float max = vec[0];
  for(int i=0; i<size; i++){
    if(max < vec[i]) max = vec[i];
  }

  //---------------------------
  return max;
}
float fct_max_vec(std::vector<std::vector<float>>& vec){
  float max = vec[0].size();
  int size = vec.size();
  //---------------------------

  for(int i=0; i<size; i++){
    if(max < vec[i].size()) max = vec[i].size();
  }

  //---------------------------
  return max;
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
glm::vec3 fct_min_z(std::vector<glm::vec3> vec){
  glm::vec3 min = vec[0];
  int size = vec.size();
  //---------------------------

  for(int i=0; i<size; i++){
    if(min.z > vec[i].z) min = vec[i];
  }

  //---------------------------
  return min;
}
glm::vec3 fct_min_vec3(std::vector<glm::vec3> XYZ){
  glm::vec3 min = glm::vec3(100000000.0f);
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
glm::vec3 fct_max_vec3(std::vector<glm::vec3> XYZ){
  glm::vec3 max = XYZ[0];
  int size = XYZ.size();
  //---------------------------

  for(int i=0; i<size; i++){
    for(int j=0; j<3; j++){
      if(XYZ[i][j] >= max[j]) max[j] = XYZ[i][j];
    }
  }

  //---------------------------
  return max;
}

//Normalization
std::vector<float> fct_normalize(std::vector<float>& vec){
  std::vector<float> vec_out(vec);
  int size = vec.size();
  //-----------------------------

  //Retrieve min & max
  float min = vec[0];
  float max = vec[0];
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
std::vector<double> fct_normalize(std::vector<double>& vec){
  std::vector<double> vec_out(vec);
  int size = vec.size();
  //-----------------------------

  //Retrieve min & max
  double min = vec[0];
  double max = vec[0];
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
std::vector<float> fct_normalize(std::vector<float>& vec, glm::vec2 range){
  std::vector<float> vec_out(vec);
  int size = vec.size();
  //-----------------------------

  //Retrieve min & max
  float min = (float)range.x;
  float max = (float)range.y;

  //Normalization
  for(int i=0; i<size; i++){
    vec_out[i] = (vec[i] - min) / (max - min);

    if(vec_out[i] > 1 || vec_out[i] < 0){
      vec_out[i] = -1;
    }
  }

  //-----------------------------
  return vec_out;
}
std::vector<float> fct_normalize(std::vector<float>& vec, float value_to_avoid){
  std::vector<float> vec_out(vec);
  int size = vec.size();
  //-----------------------------

  //Retrieve min & max
  float min = vec[0];
  float max = vec[0];
  for(int i=0; i<size; i++){
    if(vec[i] != value_to_avoid){
      if(vec[i] > max && vec[i] != value_to_avoid) max = vec[i];
      else if(vec[i] < min && vec[i] != value_to_avoid) min = vec[i];
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
std::vector<float> fct_standardize(std::vector<float>& vec, float value_to_avoid){
  std::vector<float> vec_out(vec);
  int size = vec.size();
  //-----------------------------

  //Compute std
  float sum = 0.0, mean, std = 0.0;
  for(int i=0; i<vec.size(); i++){
    sum += vec[i];
  }
  mean = sum/vec.size();
  for(int i=0; i<vec.size(); i++){
    std += pow(vec[i] - mean, 2);
  }
  std = sqrt(std / vec.size());

  //Retrieve min & max
  float min = vec[0];
  float max = vec[0];
  for(int i=0; i<size; i++){
    if(vec[i] != value_to_avoid){
      if(vec[i] > mean + std * 3 || vec[i] < mean - std * 3) vec[i] = value_to_avoid;
      else if(vec[i] > max && vec[i] != value_to_avoid) max = vec[i];
      else if(vec[i] < min && vec[i] != value_to_avoid) min = vec[i];
    }
  }

  //Normalization
  for(int i=0; i<size; i++){
    if(vec[i] != value_to_avoid && vec[i] < mean + std * 3){
      vec_out[i] = (vec[i] - min) / (max - min);
    }else{
      vec_out[i] = vec[i];
    }
  }

  //-----------------------------
  return vec_out;
}
std::vector<float> fct_normalize_01(std::vector<float>& vec){
  std::vector<float> vec_out(vec);
  int size = vec.size();
  //-----------------------------

  //Retrieve min & max
  float min = 1;
  float max = 0;
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


//Statistical functions
float fct_std(std::vector<float>& vec){
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
float fct_var(std::vector<float>& vec){
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
float fct_cv(std::vector<float>& vec){
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
std::vector<size_t> fct_sortByIndexes(const std::vector<float> &v){
  // initialize original index locations
  std::vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  std::sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

  return idx;
}
std::vector<size_t> fct_sortByIndexes_greater(const std::vector<float> &v){
  //--->Sort by greater order, keeping trace of indices

  // initialize original index locations
  std::vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  std::sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});

  return idx;
}
bool is_number(const std::string& s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
void fct_sort_alpha_num_(std::vector<std::string>& vec){
  //---------------------------

  std::sort(vec.begin(), vec.end(), [](const std::string& a, const std::string& b) {
    if (a[0] < b[0]) {
        return true;
    } else if (a[0] > b[0]) {
        return false;
    }

    if(is_number(a) && is_number(b)){
      std::string a_path = a.substr(a.find_last_of("/")+1);
      std::string b_path = b.substr(b.find_last_of("/")+1);

      std::string a_num = a_path.substr(0, a_path.find_last_of("."));
      std::string b_num = b_path.substr(0, b_path.find_last_of("."));

      if(a_num.find("_") != std::string::npos){
        a_num = a_num.substr(a_num.find_last_of("_") + 1);
      }
      if(b_num.find("_") != std::string::npos){
        b_num = b_num.substr(b_num.find_last_of("_") + 1);
      }

      return std::stoi(a_num) < std::stoi(b_num);
    }else{
      if(is_number(a) && is_number(b)){
        std::string a_path = a.substr(a.find_last_of("/")+1);
        std::string b_path = b.substr(b.find_last_of("/")+1);

        std::string a_name = a_path.substr(0, a_path.find_last_of("."));
        std::string b_name = b_path.substr(0, b_path.find_last_of("."));

        return a_name < b_name;
      }
    }
  });

  //---------------------------
  return;
}

//Geometric functions
double fct_angularDistance(const Eigen::Matrix3f &rota, const Eigen::Matrix3f &rotb) {
  double norm = ((rota * rotb.transpose()).trace() - 1) / 2;
  norm = std::acos(norm) * 180 / M_PI;
  return norm;
}
float fct_oriented_angle(glm::vec2 A, glm::vec2 B){
  float det = A.x * B.y - A.y * B.x;
  float dot = A.x * B.x + A.y * B.y;
  float angle = atan2(det, dot);
  return angle;
}
float fct_degreeToRadian(float degree){
  float radian;
  //---------------------------

  radian = degree * 3.14159265358979323846f / 180;

  //---------------------------
  return radian;
}
float fct_radianToDegree(float radian){
  float degree;
  //---------------------------

  degree = (radian * 180) / 3.14159265358979323846f;

  //---------------------------
  return degree;
}
glm::vec3 fct_degreeToRadian_vec3(glm::vec3 degree){
  glm::vec3 radian;
  //---------------------------

  for(int i=0; i<3; i++){
    radian[i] = degree[i] * 3.14159265358979323846f / 180;
  }

  //---------------------------
  return radian;
}
glm::vec3 fct_radianToDegree_vec3(glm::vec3 radian){
    glm::vec3 degree;
    //---------------------------

    for(int i=0; i<3; i++){
      degree[i] = (radian[i] * 180) / 3.14159265358979323846f;
    }

    //---------------------------
    return degree;
  }
