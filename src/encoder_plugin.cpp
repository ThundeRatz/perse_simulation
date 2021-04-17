/**
 * @file encoder_plugin.cpp
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

#include <cmath>

#include "encoder_plugin.h"

namespace gazebo {

	//register the plugin as a world plugin
	GZ_REGISTER_WORLD_PLUGIN(EncoderPlugin);

	/**
	* @brief Function called when GAZEBO initializes the plugin. Initializes the ROS objects.
	*/
	void EncoderPlugin::Load(physics::WorldPtr world, sdf::ElementPtr sdf) {

		// Make sure the ROS node for Gazebo has already been initialized
		if (!ros::isInitialized()) {
			ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
				<< "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
			return;
		}

		ROS_INFO("Encoder plugin: Starting!");

		// Create a ROS node.
		this->ros_node = new ros::NodeHandle("gazebo_client");

		// Create a named topic, and subscribe to it.
		ros::SubscribeOptions so =
			ros::SubscribeOptions::create<sensor_msgs::JointState>(
					"/thunderwaze/joint_states",
						1,
					boost::bind(&EncoderPlugin::callback, this, _1),
					ros::VoidPtr(), &this->ros_queue);

		this->ros_sub = this->ros_node->subscribe(so);

		// Spin up the queue helper thread.
		this->ros_queue_thread = std::thread(std::bind(&EncoderPlugin::QueueThread, this));

		// Create the publishser
		this->ros_pub = this->ros_node->advertise<std_msgs::Float64MultiArray>("encoder", 1000);
	}
		
	/**
	* @brief Calculates the distance on each wheel, and publish in the topic "gazebo_client/distance"
	*
	* @param msg pointer to a jointState message 
	*/
	void EncoderPlugin::callback(const sensor_msgs::JointStateConstPtr& msg) {		

		const double right_angle = msg->position[3]; //front_right wheel 
		const double left_angle = msg->position[2];  //front_left wheel
		
		// Value of the increasing noticed by the encoder
		float angle_division = 2*M_PI/resolution;

		// The integer number of "angle_divisions" traveled is  floor(right_angle/angle_division)
		// Multiplying by "angle_distance" we can obtain the angle variation noticed by the encoder
		// Multiplying now by "wheel_radius" the result is the distance traveled 
		float right_distance = floor(right_angle/angle_division)*angle_division*wheel_radius;
		float left_distance = floor(left_angle/angle_division)*angle_division*wheel_radius;
		
		pub_msg.data = {left_distance, right_distance};
		this->ros_pub.publish(pub_msg);
	}

	/**
	* @brief ROS helper function that processes messages 
	*/
	void EncoderPlugin::QueueThread() {
		static const double timeout = 0.01;
		while (this->ros_node->ok()) {
			this->ros_queue.callAvailable(ros::WallDuration(timeout));
		}
	}
};


