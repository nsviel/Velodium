#Project
cmake_minimum_required(VERSION 3.1)
project(VELODIUM LANGUAGES CXX)
cmake_policy(SET CMP0072 NEW)

option(WITH_PCL OFF)

#General policy
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Library dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenMP REQUIRED)
find_package(Ceres REQUIRED)
find_package(tsl-robin-map REQUIRED)
find_package(Freetype REQUIRED)

#Denpendancies
set(ALL_LIBS
  ${OPENGL_LIBRARY}
  ${GLFW_LIBRARY}
  ${GLEW_LIBRARY}
  ${GLM_LIBRARY}
  ${OpenMP_CXX_FLAGS}
  ${CERES_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  OpenMP::OpenMP_CXX
  Eigen3::Eigen
  glfw
  tsl::robin_map # Fast unordored map
  -fopenmp  # Multithreading
  -lGLEW
)

#Optional libraries
if(WITH_PCL)
MESSAGE(STATUS "WITH_PCL")
add_definitions(-DUSE_PCL)
find_package(PCL 1.2 REQUIRED)
set(OPTIONAL_LIBS
  ${PCL_LIBRARIES})
endif()

#Others libraries
set(OTHERS_LIBS
  tins #packet crafting and sniffing library
  -lstdc++fs
  -lblas
  -llapack
  -ljsoncpp
  -ldl
  -lboost_iostreams
  -llz4
  -lssh #SSH connexion libray
)

#Header libraries
include_directories(${CMAKE_SOURCE_DIR}/../extern)
include_directories (/usr/include/freetype2)

#Source files
file(GLOB_RECURSE ALL_SOURCES src/*.cpp extern/*.cpp)

#Executable
add_executable(executable ${ALL_SOURCES})

#Library linking
target_link_libraries(executable ${ALL_LIBS} ${OPTIONAL_LIBS} ${OTHERS_LIBS})



 
