#Library dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenMP REQUIRED)
find_package(Ceres REQUIRED)
find_package(tsl-robin-map REQUIRED)
find_package(Freetype REQUIRED)
find_package(libssh REQUIRED)

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
    -lglog
    -lstdc++fs
    -lblas
    -llapack
    -ljsoncpp
    -ldl
    -lboost_iostreams
    -llz4
    -lssh #SSH connexion libray
    paho-mqttpp3 # MQTT library
    paho-mqtt3as # async MQTT library
)

add_subdirectory(Engine)
add_subdirectory(GUI)
add_subdirectory(Load)
add_subdirectory(Operation)
add_subdirectory(Specific)
add_subdirectory(Module)

#/!\ lorsque deux source directory sont commentés, on casse les liens implicites
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
add_executable(executable ${ALL_SOURCES})

#Library linking
target_link_libraries(executable 
    ${ALL_LIBS} 
    ${OPTIONAL_LIBS} 
    ${OTHERS_LIBS} 
    dir_extern 
    dir_engine 
    dir_gui 
    dir_load 
    dir_operation
    dir_specific
    dir_module
    ${CMAKE_DL_LIBS}
)

# add the binary tree to the search path for include files
target_include_directories(executable PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/extern"
                          )


 
