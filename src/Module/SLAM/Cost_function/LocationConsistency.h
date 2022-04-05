#ifndef LOCATION_CONSISTENCY_H
#define LOCATION_CONSISTENCY_H

#include <Eigen/Dense>


//LocationConsistencyFunctor (LCF)
// A Const Functor which enforces Frame consistency between two poses
struct LCF {

    static constexpr int NumResiduals() { return 3; }

    LCF(const Eigen::Vector3f &previous_location, double beta) : beta_(beta), previous_location_(previous_location) {}

    template<typename T>
    bool operator()(const T *const location_params, T *residual) const {
        residual[0] = beta_ * (location_params[0] - previous_location_(0, 0));
        residual[1] = beta_ * (location_params[1] - previous_location_(1, 0));
        residual[2] = beta_ * (location_params[2] - previous_location_(2, 0));
        return true;
    }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
private:
    Eigen::Vector3f previous_location_;
    double beta_ = 1.0;
};
/*
class LocationConsistencyFunctor{
public:
  LocationConsistencyFunctor(const Eigen::Vector3f& previous_location, double beta) : beta_(beta), previous_location_(previous_location) {}
  static constexpr int NumResiduals(){
    return 3;
  }
  template<typename T> bool operator()(const T *const location_params, T *residual){
    residual[0] = beta_ * (location_params[0] - previous_location_(0, 0));
    residual[1] = beta_ * (location_params[1] - previous_location_(1, 0));
    residual[2] = beta_ * (location_params[2] - previous_location_(2, 0));
    return true;
  }

protected:
   Eigen::Vector3f previous_location_;
   double beta_ = 1.0;
};*/

#endif
