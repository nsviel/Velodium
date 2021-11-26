#ifndef SMALL_VELOCITYY_H
#define SMALL_VELOCITYY_H

#include <Eigen/Dense>


//SmallVelocityFunctor (SVF)
// A Const Functor which enforces a Small Velocity constraint
struct SVF {

    static constexpr int NumResiduals() { return 3; }

    SVF(double beta) : beta_(beta) {};

    template<typename T>
    bool operator()(const T *const begin_t, const T *const end_t, T *residual) const {
        residual[0] = beta_ * (begin_t[0] - end_t[0]);
        residual[1] = beta_ * (begin_t[1] - end_t[1]);
        residual[2] = beta_ * (begin_t[2] - end_t[2]);
        return true;
    }

    double beta_;
};
/*class SmallVelocityFunctor{
public:
  template<typename T> bool operator()(T *const begin_t, const T *const end_t, T *residual) {
    residual[0] = beta_ * (begin_t[0] - end_t[0]);
    residual[1] = beta_ * (begin_t[1] - end_t[1]);
    residual[2] = beta_ * (begin_t[2] - end_t[2]);
    return true;
  }
  SmallVelocityFunctor(double beta) : beta_(beta) {};
  static constexpr int NumResiduals() { return 3; }

protected:
  double beta_;
};*/

#endif
