find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(tsl-robin-map REQUIRED)
find_package(libssh REQUIRED)
#find_package(Ceres REQUIRED)

set(ALL_LIBS
    Eigen3::Eigen
    tsl::robin_map # Fast unordored map
    paho-mqttpp3 # MQTT library
    paho-mqtt3as # async MQTT library
    -lssh #SSH connexion libray
)

#Optional libraries
if(WITH_PCL)
MESSAGE(STATUS "WITH_PCL")
add_definitions(-DUSE_PCL)
find_package(PCL 1.2 REQUIRED)
set(OPTIONAL_LIBS
  ${PCL_LIBRARIES})
endif()

file(GLOB_RECURSE sources *.cpp)

add_library(dir_module ${sources})
target_link_libraries(dir_module ${ALL_LIBS} dir_extern  ${CERES_LIBRARIES} ${OPTIONAL_LIBS} )
target_include_directories(dir_module PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/extern"
                          )
