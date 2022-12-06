# Project
cmake_minimum_required(VERSION 3.1)
project(VELODIUM LANGUAGES CXX)
cmake_policy(SET CMP0072 NEW)

# Options
option(WITH_MODULE_RADIOMETRY "Use RADIOMETRY module" OFF)
option(WITH_MODULE_REGISTRATION "Use REGISTRATION module" OFF)
option(WITH_MODULE_SLAM "Use SLAM module" ON)
option(WITH_MODULE_OBSTACLE "Use OBSTACLE module" ON)
option(WITH_LIBRAY_PCL "Use PCL library" OFF)
option(WITH_LIBRARY_CERES "Use CERES library" OFF)

# General policy
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Child CMakeLists
add_subdirectory(extern)
add_subdirectory(src)
