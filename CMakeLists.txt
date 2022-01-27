cmake_minimum_required(VERSION 2.8.3)
project(perse_simulation)

add_compile_options(-std=c++11)

# Load catkin and all dependencies required for this package
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  gazebo_ros
)

# Depend on system install of Gazebo
find_package(gazebo REQUIRED)

link_directories(${GAZEBO_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS})

catkin_package(
  INCLUDE_DIRS include
  DEPENDS
    roscpp
    gazebo_ros
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_library(encoder_plugin src/encoder_plugin.cpp)
target_link_libraries(encoder_plugin ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

add_library(bumper_plugin src/bumper_plugin.cpp)
target_link_libraries(bumper_plugin ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})
