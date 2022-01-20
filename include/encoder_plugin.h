/**
 * @file encoder_plugin.cpp
 *
 * @author Lucas Guedes dos Santos <lucas.guedes@thunderatz.org>
 * @author Giovanni "Toco" Cabral <giovanni.morales@thunderatz.org>
 * @author Sergio Magalhães Contente <lucas.guedes@thunderatz.org>
 *
 * @brief Plugin that simulates a Encoder, publishing (in a ROS topic) the travelled distance with a limited resolution
 *
 * @date 01/2022
 *
 * @copyright MIT License - Copyright (c) 2021 ThundeRatz
 *
 */

#ifndef __ENCODER_PLUGIN_H__
#define __ENCODER_PLUGIN_H__

#include <gazebo/common/Plugin.hh>
#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/Joint.hh>

namespace gazebo {
class EncoderPlugin :
    public ModelPlugin {
    private:
        ros::NodeHandle* node_handler;       /*!< A node use for ROS transport */
        ros::Publisher encoder_pub;          /*!< ROS publisher that publishes the distance measured in the encoder */
        std_msgs::Float64 pub_msg; /*!< ROS message made of an array of floats, to carry values of both wheels movements */
        event::ConnectionPtr updateConnection;
        physics::JointPtr wheel;
        float wheel_radius;
        float angle;
        float angle_division;  /*!< Value of the increasing noticed by the encoder */

        /**
         * @brief Calculates the distance on each wheel, and publish in the topic "gazebo_client/distance"
         */
        void OnUpdate();

    public:
        /**
         * @brief Function called when GAZEBO initializes the plugin. Initializes the ROS objects.
         */
        void Load(physics::ModelPtr _model, sdf::ElementPtr  /*_sdf*/);
};
};

#endif // __ENCODER_PLUGIN_H__
