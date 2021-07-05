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

namespace gazebo {
GZ_REGISTER_MODEL_PLUGIN(EncoderPlugin)

void EncoderPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {
    ROS_INFO("Encoder plugin: Starting!");

    this->node_handler = new ros::NodeHandle();

    this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&EncoderPlugin::OnUpdate, this));

    // Get encoder resolution and calculate the angle divisions
    if (_sdf->HasElement("resolution")) {
        int resolution = _sdf->Get<int>("resolution");
        this->angle_division = 2*M_PI/resolution;
    }

    // Get wheel radius
    if (_sdf->HasElement("wheelRadius")) {
        this->wheel_radius = _sdf->Get<float>("wheelRadius");
    }

    // Get name of the topic to be published and create the publiser
    if (_sdf->HasElement("encoderTopicName")) {
        std::string topic_name = _sdf->Get<std::string>("encoderTopicName");
        this->encoder_pub = this->node_handler->advertise<std_msgs::Float64MultiArray>(topic_name, 1000);
    }

    // Get front left wheel joint name
    if (_sdf->HasElement("leftWheelJointName")) {
        std::string left_wheel_joint_name = _sdf->Get<std::string>("leftWheelJointName");
        this->left_wheel = _model->GetJoint(left_wheel_joint_name);
    }

    // Get front right wheel joint name
    if (_sdf->HasElement("rightWheelJointName")) {
        std::string right_wheel_joint_name = _sdf->Get<std::string>("rightWheelJointName");
        this->right_wheel = _model->GetJoint(right_wheel_joint_name);
    }
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
