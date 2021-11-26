#ifndef CT_POINT_TO_PLANE_H
#define CT_POINT_TO_PLANE_H

#include <Eigen/Dense>
#include <ceres/ceres.h>


//CTPointToPlaneFunctor (CTPTPF)
// A Const Functor for the Continuous time Point-to-Plane
struct CTPTPF {

  static constexpr int NumResiduals() { return 1; }

  CTPTPF(const Eigen::Vector3d &iNN_, const Eigen::Vector3d &keypoint_, const Eigen::Vector3d &normal_, double ts_n_, double weight_ = 1.0){
    keypoint = keypoint_;
    iNN = iNN_;
    normal = normal_;
    ts_n = ts_n_;
    weight = weight_;
  }

  template<typename T>
  bool operator()(const T *const rotat_b, const T *trans_b, const T *const rotat_e, const T *trans_e, T *residual) const {

    Eigen::Map<Eigen::Quaternion<T>> quat_b(const_cast<T *>(rotat_b));
    Eigen::Map<Eigen::Quaternion<T>> quat_e(const_cast<T *>(rotat_e));

    Eigen::Quaternion<T> quat_inter = quat_b.slerp(T(ts_n), quat_e);
    quat_inter.normalize();

    Eigen::Matrix<T, 3, 1> transformed = quat_inter * keypoint.template cast<T>();

    /*Eigen::Map<Eigen::Quaternion<T>> quat(const_cast<T *>(rotat_e));
    Eigen::Matrix<T, 3, 1> transformed = quat * keypoint.template cast<T>();*/

    T ts_m = T(1.0 - ts_n);
    transformed(0, 0) += ts_m * trans_b[0] + ts_n * trans_e[0];
    transformed(1, 0) += ts_m * trans_b[1] + ts_n * trans_e[1];
    transformed(2, 0) += ts_m * trans_b[2] + ts_n * trans_e[2];

    residual[0] = weight * (iNN.template cast<T>() - transformed).transpose() * normal.template cast<T>();

    return true;
  }

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  Eigen::Vector3d keypoint;
  Eigen::Vector3d iNN;
  Eigen::Vector3d normal;
  double ts_n;
  double weight = 1.0;
};

using CTPTPR = ceres::AutoDiffCostFunction<CTPTPF, 1, 4, 3, 4, 3>;



#endif
