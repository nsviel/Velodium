#=========================
# Module RADIOMETRY
#=========================
if(WITH_MODULE_RADIOMETRY)
  MESSAGE(STATUS "With RADIOMETRY module")
  file(GLOB_RECURSE SRC_RADIOMETRY Radiometry/*.cpp)
  add_definitions(-DWITH_RADIOMETRY=1)

  set(LIB_RADIOMETRY
      Eigen3::Eigen #Eigen library
  )

else()
  add_definitions(-DWITH_RADIOMETRY=0)
endif()


#=========================
# Module REGISTRATION
#=========================
if(WITH_MODULE_REGISTRATION)
  MESSAGE(STATUS "With REGISTRATION module")
  file(GLOB_RECURSE SRC_REGISTRATION Registration/*.cpp)
  add_definitions(-DWITH_REGISTRATION=1)

  set(LIB_REGISTRATION
    Eigen3::Eigen #Eigen library
  )

else()
  add_definitions(-DWITH_REGISTRATION=0)
endif()


#=========================
# Module SLAM
#=========================
if(WITH_MODULE_SLAM)
  MESSAGE(STATUS "With SLAM module")
  file(GLOB_RECURSE SRC_SLAM SLAM/*.cpp)
  add_definitions(-DWITH_SLAM=1)
  find_package(tsl-robin-map REQUIRED)

  set(LIB_SLAM
    Eigen3::Eigen #Eigen library
    tsl::robin_map # Fast unordored map
  )

  #=========================
  # Library CERES
  #=========================
  if(WITH_LIBRARY_CERES)
    MESSAGE(STATUS "With CERES library")
    add_definitions(-DWITH_CERES=1)
    find_package(Ceres REQUIRED)

    set(LIB_SLAM
      ${CERES_LIBRARIES}
      tsl::robin_map # Fast unordored map
    )

  else()
    add_definitions(-DWITH_CERES=0)
  endif()
else()
  add_definitions(-DWITH_SLAM=0)
endif()


#=========================
# Module OBSTACLE
#=========================
if(WITH_MODULE_OBSTACLE)
  MESSAGE(STATUS "With OBSTACLE module")
  file(GLOB_RECURSE SRC_OBSTACLE Obstacle/*.cpp)
  add_definitions(-DWITH_OBSTACLE=1)

  set(LIB_OBSTACLE

  )

else()
  add_definitions(-DWITH_OBSTACLE=0)
endif()


#=========================
# Library PCL
#=========================
if(WITH_LIBRAY_PCL)
  MESSAGE(STATUS "With PCL library")
  add_definitions(-DWITH_PCL=1)
  find_package(PCL 1.2 REQUIRED)

  set(LIB_PCL
    ${PCL_LIBRARIES})

else()
  add_definitions(-DWITH_PCL=0)
endif()


#=========================
# Setup MODULE library
add_library(LIBRARY_MODULE
    Node_module.cpp
    ${SRC_RADIOMETRY}
    ${SRC_REGISTRATION}
    ${SRC_SLAM}
    ${SRC_OBSTACLE}
)
target_link_libraries(LIBRARY_MODULE
    LIBRARY_EXTERN
    Eigen3::Eigen #Eigen library
    ${LIB_RADIOMETRY}
    ${LIB_REGISTRATION}
    ${LIB_SLAM}
    ${LIB_OBSTACLE}
    ${LIB_PCL}
)
target_include_directories(LIBRARY_MODULE PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/extern"
)
