/**
 * @file encoder_plugin.h
 * 
 * @author Lucas Guedes dos Santos (lucas.guedes@thunderatz.org)
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


#include <gazebo/common/Plugin.hh>
#include <thread>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "std_msgs/Float64MultiArray.h"
#include "sensor_msgs/JointState.h"
#include <cmath>

#define wheel_radius 0.06
#define resolution 2


namespace gazebo {

	class EncoderPlugin : public WorldPlugin {

	private: 
	 
		ros::NodeHandle* rosNode;           /*!< A node use for ROS transport */
		ros::Subscriber rosSub;             /*!< ROS subscriber */
		ros::CallbackQueue rosQueue;        /*!< ROS callbackqueue that helps process messages */
		std::thread rosQueueThread;         /*!< A thread the keeps running the rosQueue */
		ros::Publisher rosPub;              /*!< ROS publisher */
        std_msgs::Float64MultiArray pubMsg; /*!< ROS message */

	public:
	
		void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf);
		public: void callback(const sensor_msgs::JointStateConstPtr &_msg);
		private: void QueueThread();
	};
};


#endif // __SENSOR_PLUGIN_H__
