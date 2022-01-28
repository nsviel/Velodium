find_package(Eigen3 3.3 REQUIRED NO_MODULE)

file(GLOB_RECURSE sources *.cpp)

add_library(dir_gui ${sources})
target_link_libraries(dir_gui Eigen3::Eigen dir_extern)
target_include_directories(dir_gui PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/extern"
                          )
