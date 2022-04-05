#ifdef USE_CERES
#ifndef CT_POINT_TO_PLANE_H
#define CT_POINT_TO_PLANE_H

#include <Eigen/Dense>
#include <ceres/ceres.h>


//CTPointToPlaneFunctor (CTPTPF)
// A Const Functor for the Continuous time Point-to-Plane
struct CTPTPF {

  static constexpr int NumResiduals() { return 1; }

  //Initialization
  CTPTPF(const Eigen::Vector3f &iNN_, const Eigen::Vector3f &keypoint_, const Eigen::Vector3f &normal_, double ts_n_, double weight_ = 1.0){
    point_raw = keypoint_;
    iNN = iNN_;
    normal = normal_;
    ts_n = ts_n_;
    weight = weight_;
  }

  //During optimization
  template<typename T>
  bool operator()(const T *const rotat_b, const T *trans_b, const T *const rotat_e, const T *trans_e, T *residual) const {

    //Translation alpha
    T ts_m = T(1.0 - ts_n);
    T tx = ts_m * trans_b[0] + ts_n * trans_e[0];
    T ty = ts_m * trans_b[1] + ts_n * trans_e[1];
    T tz = ts_m * trans_b[2] + ts_n * trans_e[2];

    //Rotation alpha
    Eigen::Map<Eigen::Quaternion<T>> quat_b(const_cast<T *>(rotat_b));
    Eigen::Map<Eigen::Quaternion<T>> quat_e(const_cast<T *>(rotat_e));
    Eigen::Quaternion<T> quat_alpha = quat_b.slerp(T(ts_n), quat_e).normalized();

    //World point
    Eigen::Matrix<T, 3, 1> transformed = quat_alpha * point_raw.template cast<T>();
    transformed(0, 0) += tx;
    transformed(1, 0) += ty;
    transformed(2, 0) += tz;

    //Residual
    residual[0] = (transformed - iNN.template cast<T>()).transpose() * normal.template cast<T>();

    return true;
  }

  Eigen::Vector3f point_raw;
  Eigen::Vector3f iNN;
  Eigen::Vector3f normal;
  double ts_n;
  double weight;
};

using CTPTPR = ceres::AutoDiffCostFunction<CTPTPF, 1, 4, 3, 4, 3>;


#endif
#endif
