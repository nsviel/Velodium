file(GLOB_RECURSE sources *.cpp *.hpp)
add_library(dir_extern ${sources})
target_link_libraries(dir_extern ${CMAKE_DL_LIBS})

