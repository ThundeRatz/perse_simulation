cmake_minimum_required(VERSION 3.5)
project(perse_simulation)

add_compile_options(-std=c++11)

# Depend on system install of Gazebo
find_package(gazebo REQUIRED)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(gazebo_ros REQUIRED)

link_directories(${GAZEBO_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIR} ${GAZEBO_INCLUDE_DIRS})

ament_export_include_directories(include)

ament_export_dependencies(rclcpp gazebo_ros)

# add_library(encoder_plugin src/encoder_plugin.cpp)

# ament_target_dependencies(encoder_plugin
# rclcpp
#  sensors_msgs
#  gazebo_ros
#  gazebo
#)

add_library(bumper_plugin src/bumper_plugin.cpp)

ament_target_dependencies(bumper_plugin
  rclcpp
  sensors_msgs
  gazebo_ros
  gazebo
)

ament_package()