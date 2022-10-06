#ifdef USE_CERES
#ifndef POINT_TO_PLANE_H
#define POINT_TO_PLANE_H

#include <Eigen/Dense>
#include <ceres/ceres.h>


//PointToPlaneFunctor (PTPF)
// A Cost Functor a standard Point-to-Plane
class PTPF{
public:
  PTPF(Eigen::Vector3f& point, Eigen::Vector3f& target, Eigen::Vector3f& normal, double w = 1.0) :
  reference_point(point), target_point(target), reference_normal(normal), weight(w) {}

  template<typename T> bool operator()(const T *const rot_params, const T *const trans_params, T *residual) const {
      Eigen::Map<Eigen::Quaternion<T>> quat(const_cast<T *>(rot_params));
      Eigen::Matrix<T, 3, 1> transformed = quat * target_point.template cast<T>();
      transformed(0, 0) += trans_params[0];
      transformed(1, 0) += trans_params[1];
      transformed(2, 0) += trans_params[2];

      residual[0] = weight * (reference_point.template cast<T>() - transformed).transpose() * reference_normal.template cast<T>();
      return true;
  }
  static constexpr int NumResiduals() { return 1; }

protected:
  Eigen::Vector3f reference_point;
  Eigen::Vector3f target_point;
  Eigen::Vector3f reference_normal;
  double weight = 1.0;
};

using PTPR = ceres::AutoDiffCostFunction<PTPF, 1, 4, 3>;


#endif
#endif
