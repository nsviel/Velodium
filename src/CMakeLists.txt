#Library dependencies
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Freetype REQUIRED)

#Denpendancies
set(ALL_LIBS
  ${OPENGL_LIBRARY}
  ${GLFW_LIBRARY}
  ${GLEW_LIBRARY}
  ${GLM_LIBRARY}
  ${OpenMP_CXX_FLAGS}
  ${FREETYPE_LIBRARIES}

  -lssl -lcrypto # SSL stuff
  OpenMP::OpenMP_CXX
  Eigen3::Eigen #Eigen library
  glfw # Opengl glfw
  tins #packet crafting and sniffing library
  GLEW  #OpenGL glew
  stdc++fs #STD filesystem
  blas #Mathematic library
  lapack #Mathematic library
  jsoncpp #JSON parser
  microhttpd #HTTP network protocol
  boost_iostreams #necessary for gnuplot
  lz4 #Necessary for FLANN
  freeimage #Screenchot lib
  -fopenmp  #Multithreading
)

#Module subdirectory
add_subdirectory(Module)

#Source files
file(GLOB_RECURSE ALL_SOURCES
  Engine/*.cpp
  GUI/*.cpp
  Load/*.cpp
  Operation/*.cpp
  Specific/*.cpp
  Module/*.cpp
  main.cpp
)

#Executable
add_executable(executable
  ${ALL_SOURCES}
)

#Library linking
target_link_libraries(executable
  ${ALL_LIBS}
  dir_extern
  dir_module
  ${CMAKE_DL_LIBS}
)

# add the binary tree to the search path for include files
target_include_directories(executable PUBLIC
  "${PROJECT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}/extern"
)
