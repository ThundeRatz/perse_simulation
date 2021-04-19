/**
 * @file encoder_plugin.h
 * 
 * @author Lucas Guedes dos Santos <lucas.guedes@thunderatz.org>
 * 
 * @brief Plugin that simulates a Encoder, publishing (in a ROS topic) the travelled distance with a limited resolution   
 *
 * @date 04/2021
 *
 * @copyright MIT License - Copyright (c) 2021 ThundeRatz
 *
 */

#ifndef __SENSOR_PLUGIN_H__
#define __SENSOR_PLUGIN_H__

#include <cmath>
#include <thread>
#include <gazebo/common/Plugin.hh>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "std_msgs/Float64MultiArray.h"
#include "sensor_msgs/JointState.h"

#define wheel_radius 0.06   //robot wheel radius
#define resolution 2		//number of encoder updates at each turn of the wheel

namespace gazebo {

	class EncoderPlugin : public WorldPlugin {

	private: 
	 
		ros::NodeHandle* ros_node;           /*!< A node use for ROS transport */
		ros::Subscriber ros_sub;             /*!< ROS subscriber that gets the wheels angles from topic /perse_simulation/joint_states */
		ros::Publisher ros_pub;              /*!< ROS publisher that publishes the distance measured in the encoder */
		std_msgs::Float64MultiArray pub_msg; /*!< ROS message made of an array of floats, to carry values of both wheels movements */
		ros::CallbackQueue ros_queue;        /*!< ROS callbackqueue that helps process messages */
		std::thread ros_queue_thread;        /*!< A thread the keeps running the rosQueue */

		void callback(const sensor_msgs::JointStateConstPtr &_msg);
		void QueueThread();

	public:
	
		void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf);
	};
};

#endif // __SENSOR_PLUGIN_H__
