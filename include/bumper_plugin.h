/**
 * @file bumper_plugin.h
 *
 * @author Lucas Guedes dos Santos <lucas.guedes@thunderatz.org>
 *
 * @brief Plugin that publishes perse's bumper state on a ros topic.
 *
 * @date 01/2022
 *
 * @copyright MIT License - Copyright (c) 2021 ThundeRatz
 *
 */

#ifndef __BUMPER_PLUGIN_H__
#define __BUMPER_PLUGIN_H__

#include "iostream"
#include <gazebo/common/Plugin.hh>
#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/Joint.hh>

#define MSG_BUFFER 1

namespace gazebo {
class BumperPlugin :
    public ModelPlugin {
    public:
        /**
         * @brief Function called when GAZEBO initializes the plugin. Initializes the ROS objects.
         */
        void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

    private:
        ros::NodeHandle node_handler;  /*!< A node use for ROS transport */
        ros::Publisher bumper_pub;
        std_msgs::Bool bool_msg;  /*!< True if bumper_joint position is bigger than trigger_position */

        event::ConnectionPtr update_connection;
        physics::JointPtr bumper_joint;  /*!< Bumper joint from Gazebo model  */

        float joint_force;      /*!< Constant force applied to the bumper Joint */
        float trigger_position; /*!< Joint Poisition where the plugin will send True as a message */

        /**
         * @brief Calculates bumper position, compares with trigger_position and publishes in topic "gazebo_client/distance "
         */
        void OnUpdate();
};

GZ_REGISTER_MODEL_PLUGIN(BumperPlugin)
};

#endif // __BUMPER_PLUGIN_H__
