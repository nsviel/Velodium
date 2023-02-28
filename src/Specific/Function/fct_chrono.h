#ifndef CHRONO_FUNCTIONS_H
#define CHRONO_FUNCTIONS_H

#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;


namespace{
  //---------------------------

  auto start_chrono(){
    return t1 = high_resolution_clock::now();
  }
  template <typename T>
  float stop_chrono(T t1){
    auto t2 = high_resolution_clock::now();
    float duration = duration_cast<milliseconds>(t2 - t1).count();
    return duration;
  }

  //---------------------------
}

#endif
