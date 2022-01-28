find_package(Eigen3 3.3 REQUIRED NO_MODULE)

file(GLOB_RECURSE sources *.cpp)

add_library(dir_operation ${sources})
target_link_libraries(dir_operation Eigen3::Eigen dir_extern)
target_include_directories(dir_operation PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/extern"
                          )
