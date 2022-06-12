#ifndef ORIENTATION_CONSISTENCY_H
#define ORIENTATION_CONSISTENCY_H

#include <Eigen/Dense>


//OrientationConsistencyFunctor (OCF)
// A Functor which enforces frame orientation consistency between two poses
struct OCF {

    static constexpr int NumResiduals() { return 1; }

    OCF(const Eigen::Quaterniond &previous_orientation, double beta) : beta_(beta), previous_orientation_(previous_orientation) {}

    template<typename T>
    bool operator()(const T *const orientation_params, T *residual) const {
        Eigen::Quaternion<T> quat(orientation_params);
        T scalar_quat = quat.dot(previous_orientation_.template cast<T>());
        residual[0] = T(beta_) * (T(1.0) - scalar_quat * scalar_quat);
        return true;
    }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
    Eigen::Quaterniond previous_orientation_;
    double beta_;

};
/*class OrientationConsistencyFunctor{
public:
  OrientationConsistencyFunctor(const Eigen::Quaterniond &previous_orientation, double beta) : beta_(beta), previous_orientation_(previous_orientation) {}
  template<typename T> bool operator()(T *const orientation_params, T *residual) {
    Eigen::Quaternion<T> quat(orientation_params);
    T scalar_quat = quat.dot(previous_orientation_.template cast<T>());
    residual[0] = T(beta_) * (T(1.0) - scalar_quat * scalar_quat);
    return true;
  }
  static constexpr int NumResiduals() {
    return 1;
  }

protected:
  Eigen::Quaterniond previous_orientation_;
  double beta_;
};*/

#endif
