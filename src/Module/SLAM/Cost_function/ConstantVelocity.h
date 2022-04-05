#ifndef CONSTANT_VELOCITY_H
#define CONSTANT_VELOCITY_H

#include <Eigen/Dense>

//ConstantVelocityFunctor (CVF)
// A Const Functor which enforces a Constant Velocity constraint on translation
struct CVF {

    static constexpr int NumResiduals() { return 3; }

    CVF(const Eigen::Vector3f &previous_velocity, double beta) : previous_velocity_(previous_velocity), beta_(beta) {}

    template<typename T>
    bool operator()(const T *const begin_t, const T *const end_t, T *residual) const {
        residual[0] = beta_ * (end_t[0] - begin_t[0] - previous_velocity_(0, 0));
        residual[1] = beta_ * (end_t[1] - begin_t[1] - previous_velocity_(1, 0));
        residual[2] = beta_ * (end_t[2] - begin_t[2] - previous_velocity_(2, 0));
        return true;
    }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
    Eigen::Vector3f previous_velocity_;
    double beta_ = 1.0;
};
/*class ConstantVelocityFunctor{
public:
  ConstantVelocityFunctor(const Eigen::Vector3f &previous_velocity, double beta) : previous_velocity_(previous_velocity), beta_(beta) {}
  template<typename T> bool operator()(const T *const begin_t, const T *const end_t, T *residual) const {
    residual[0] = beta_ * (end_t[0] - begin_t[0] - previous_velocity_(0, 0));
    residual[1] = beta_ * (end_t[1] - begin_t[1] - previous_velocity_(1, 0));
    residual[2] = beta_ * (end_t[2] - begin_t[2] - previous_velocity_(2, 0));
    return true;
  }
  static constexpr int NumResiduals() { return 3; }

protected:
  Eigen::Vector3f previous_velocity_;
  double beta_ = 1.0;
};*/

#endif
