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
  CTPTPF(const Eigen::Vector3f &trg, const Eigen::Vector3f &src, const Eigen::Vector3f &norm, double ts){
    target = trg; source = src; normal = norm; ts_n = ts;
  }

  //During optimization
  template<typename T>
  bool operator()(const T *trans_b, const T *trans_e, T *residual) const {

    //Translation alpha
    /*T ts_m = T(1.0 - ts_n);
    T tx = ts_m * trans_b[0] + ts_n * trans_e[0];
    T ty = ts_m * trans_b[1] + ts_n * trans_e[1];
    T tz = ts_m * trans_b[2] + ts_n * trans_e[2];*/

    T tx = trans_e[0];
    T ty = trans_e[1];
    T tz = trans_e[2];

    //World point
    Eigen::Matrix<T, 3, 1> source_w = source.template cast<T>();
    source_w(0, 0) += tx;
    source_w(1, 0) += ty;
    source_w(2, 0) += tz;

    //Residual
    residual[0] = (source_w - target.template cast<T>()).transpose() * normal.template cast<T>();

    return true;
  }

  Eigen::Vector3f source;
  Eigen::Vector3f target;
  Eigen::Vector3f normal;
  double ts_n;
};

using CTPTPR = ceres::AutoDiffCostFunction<CTPTPF, 1, 3, 3>;


#endif
#endif
