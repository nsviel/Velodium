# Setup EXTERN library
file(GLOB_RECURSE SRC_EXTERN *.cpp *.hpp)
add_library(LIBRARY_EXTERN ${SRC_EXTERN})
target_link_libraries(LIBRARY_EXTERN ${CMAKE_DL_LIBS})

