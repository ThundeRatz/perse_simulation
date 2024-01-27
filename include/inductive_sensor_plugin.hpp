/**
 * @file inductive_sensor_plugin.hpp
 *
 * @author Alan José dos Santos <alan.jose@thunderatz.org>
 *
 * @brief Plugin that simulates a Inductive Sensor, simulating an inductive sensor reading of an 1mx1m square metalic plate around each cone by publishing true when close to a set of positions defined by ros parameters. 
 *
 * @date 01/2024
 *
 * @copyright MIT License - Copyright (c) 2024 ThundeRatz
 *
 */

#ifndef __INDUCTIVE_SENSOR_PLUGIN_H__
#define __INDUCTIVE_SENSOR_PLUGIN_H__

#include <iostream>
#include <gazebo/common/Plugin.hh>
#include <gazebo_ros/node.hpp>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include <gazebo/physics/Model.hh>


#define MSG_BUFFER 1000
#define PLATE_SIDE 1.0

namespace gazebo {
class InductiveSensorPlugin :
    public ModelPlugin {
    public:
        /**
         * @brief Function called when GAZEBO initializes the plugin. Initializes the ROS objects.
         */
        void Load(physics::ModelPtr _model, sdf::ElementPtr  /*_sdf*/);

        struct Cone {
            double x;
            double y;
        };
        
    private:
        rclcpp::Node::SharedPtr ros_node; /*!< A node use for ROS transport */
        std_msgs::msg::Bool pub_msg;      /*!< ROS message made of an array of booleans, to carry values of both sensor readings */
        event::ConnectionPtr update_connection;

        std::vector<Cone> cones; 
        void declare_cones_parameters();
        physics::ModelPtr model;
        rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr inductive_sensor_pub; /*!< ROS publisher that publishes when the metalic plate is under the robot */

        void OnUpdate();
};
};

#endif // __INDUCTIVE_SENSOR_PLUGIN_H__
