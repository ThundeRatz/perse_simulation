/**
 * @file encoder_plugin.h
 * 
 * @author Lucas Guedes dos Santos <lucas.guedes@thunderatz.org>
 * 
 * @brief Plugin that simulates a Encoder, publishing (in a ROS topic) the travelled distance with a limited resolution   
 *
 * @date 06/2021
 *
 * @copyright MIT License - Copyright (c) 2021 ThundeRatz
 *
 */

#ifndef __SENSOR_PLUGIN_H__
#define __SENSOR_PLUGIN_H__

#include <cmath>
#include <gazebo/common/Plugin.hh>
#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/Joint.hh>

namespace gazebo {
	class EncoderPlugin : public ModelPlugin {

	private: 
		ros::NodeHandle* ros_node;           /*!< A node use for ROS transport */
		ros::Publisher ros_pub;              /*!< ROS publisher that publishes the distance measured in the encoder */
		std_msgs::Float64MultiArray pub_msg; /*!< ROS message made of an array of floats, to carry values of both wheels movements */
		event::ConnectionPtr updateConnection;
		physics::JointPtr right_wheel, left_wheel;
		physics::ModelPtr model;

		/**
		* @brief Calculates the distance on each wheel, and publish in the topic "gazebo_client/distance"
		*
		* @param msg pointer to a jointState message 
		*/
		void OnUpdate();

	public:
		/**
		* @brief Function called when GAZEBO initializes the plugin. Initializes the ROS objects.
		*/
		void Load(physics::ModelPtr _model, sdf::ElementPtr /*_sdf*/);
	};
};

#endif // __SENSOR_PLUGIN_H__
