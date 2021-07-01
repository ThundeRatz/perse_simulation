/**
 * @file encoder_plugin.cpp
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

#include <cmath>

#include "encoder_plugin.h"

#define wheel_radius 0.06   //robot wheel radius
#define resolution 2		//number of encoder updates at each turn of the wheel

namespace gazebo {
	GZ_REGISTER_MODEL_PLUGIN(EncoderPlugin)
	void EncoderPlugin::Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/) {
		ROS_INFO("Encoder plugin: Starting!");

		// Create the publishser
		this->node_handler = new ros::NodeHandle();
		this->encoder_pub = this->node_handler->advertise<std_msgs::Float64MultiArray>("perse/encoder", 1000);
		
		this->model = _parent;

		this->right_wheel = this->model->GetJoint("wheel_front_right_joint");
		this->left_wheel = this->model->GetJoint("wheel_front_left_joint");

		this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&EncoderPlugin::OnUpdate, this));

    	this->angle_division = 2*M_PI/resolution;
	}

	void EncoderPlugin::OnUpdate() {
		this->right_angle = this->right_wheel->Position();
    	this->left_angle = this->left_wheel->Position();

		// The integer number of "angle_divisions" traveled is  floor(right_angle/angle_division)
		// Multiplying by "angle_distance" we can obtain the angle variation noticed by the encoder
		// Multiplying now by "wheel_radius" the result is the distance traveled 
		this->right_distance = floor(right_angle/angle_division)*angle_division*wheel_radius;
		this->left_distance = floor(left_angle/angle_division)*angle_division*wheel_radius;

    	this->pub_msg.data = {left_distance, right_distance};
	  	this->encoder_pub.publish(pub_msg);
  	}  
}
