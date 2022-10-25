#ifndef TERMINAL_FUNCTIONS_H
#define TERMINAL_FUNCTIONS_H

#include <iostream>
#include <time.h>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

/**
 * Debuging and diplay functions
 */

namespace{
  //---------------------------
  std::chrono::high_resolution_clock::time_point t1;

  void sayMat4(const glm::mat4 m){
      std::cout << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << std::endl;
      std::cout << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << std::endl;
      std::cout << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << std::endl;
      std::cout << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << std::endl;
  }
  void sayVec2(glm::vec2 truc){
    std::cout<< "-> " << truc[0] <<" "<< truc[1] <<std::endl;
  }
  void sayVec3(glm::vec3 truc){
    std::cout<< "-> " << truc[0] <<" "<< truc[1] <<" "<< truc[2] <<std::endl;
  }
  void sayVec3(Eigen::Vector3d truc){
    std::cout<< "-> " << truc(0) <<" "<< truc(1) <<" "<< truc(2) <<std::endl;
  }
  void sayVecVec3(std::vector<glm::vec3> truc){
    for(int i=0; i<truc.size(); i++){
      std::cout<<"line."<<i<<" -> "<< truc[i].x <<" "<< truc[i].y <<" "<< truc[i].z <<std::endl;
    }
  }
  void sayMeanVec3(std::vector<glm::vec3> truc){
    glm::vec3 mean = glm::vec3(0, 0, 0);
    for(int i=0; i<truc.size(); i++){
      mean.x += truc[i].x;
      mean.y += truc[i].y;
      mean.z += truc[i].z;
    }
    mean.x = mean.x / truc.size();
    mean.y = mean.y / truc.size();
    mean.z = mean.z / truc.size();
    std::cout<< "mean-> " << mean[0] <<" "<< mean[1] <<" "<< mean[2] <<std::endl;
  }
  void sayVecVec4(std::vector<glm::vec4> truc){
    for(int i=0; i<truc.size(); i++){
      std::cout<<"line."<<i<<" -> "<< truc[i].x <<" "<< truc[i].y <<" "<< truc[i].z <<" "<< truc[i].w <<std::endl;
    }
  }
  void sayVec4(glm::vec4 truc){
    std::cout<< "-> " << truc[0] <<" "<< truc[1] <<" "<< truc[2] <<" "<< truc[3] << std::endl;
  }
  void sayHello(){
    std::cout<<"Hello ! "<<std::endl;
  }
  void wtf(){
    std::cout<<"wtf"<<std::endl;
  }
  template<typename Type>
  void say(Type truc){
    std::cout<< "-> " << truc <<std::endl;
  }
  template<typename Type>
  void saySize(Type truc){
    std::cout<< "-> " << truc.size() <<std::endl;
  }
  template<typename Type>
  void sayVec(std::vector<Type>& vec){
    for(int i=0; i<vec.size(); i++){
      std::cout<<std::fixed<<std::setprecision(5)<<"line."<<i<<" -> "<<vec[i]<<std::endl;
    }
  }
  auto time_start(){
    auto start = std::chrono::high_resolution_clock::now();
  }

  //Time measurement
  template <typename Type> auto time_stop(Type start){
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout<<"---> "<<duration.count()<<" ms "<<std::endl;
  }
  void tic(){
    t1 = std::chrono::high_resolution_clock::now();
  }
  void toc(std::string what){
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout<<what<<" time: "<<duration<<"ms"<<std::endl;
  }
  void toc_us(std::string what){
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout<<what<<" time: "<<duration<<"us"<<std::endl;
  }
  void toc_ns(std::string what){
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    std::cout<<what<<" time: "<<duration<<"ns"<<std::endl;
  }
  float toc(){
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    //-------------
    return duration;
  }
  float toc_us(){
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    //-------------
    return duration;
  }
  float toc_ms(){
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    //-------------
    return duration;
  }
  float toc_s(){
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

    //-------------
    return duration;
  }
  std::string epoch_to_utc(float epoch) {
    const std::time_t old = (std::time_t)epoch;
    struct tm *oldt = std::gmtime(&old);
    return std::asctime(oldt);
  }

  //---------------------------
}

#endif
