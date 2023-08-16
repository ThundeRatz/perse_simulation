/**
 * @file bumper_plugin.cpp
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

#include <cmath>
#include "bumper_plugin.hpp"

namespace gazebo {
void BumperPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {
    RCLCPP_INFO(rclcpp::get_logger(), "Bumper plugin: Starting!");
    
    // arguments from .gazebo file
    if (_sdf->HasElement("topicName")) {
        std::string topic_name = _sdf->Get<std::string>("topicName");
        this->ros_node = rclcpp::Node::make_shared("bumper_plugin_node");
        this->bumper_pub = this->ros_node->create_publisher<std_msgs::msg::Bool>(topic_name, MSG_BUFFER);
    }

    if (_sdf->HasElement("triggerPosition")) {
        this->trigger_position = _sdf->Get<float>("triggerPosition");
    }

    if (_sdf->HasElement("jointForce")) {
        this->joint_force = _sdf->Get<float>("jointForce");
    }

    if (_sdf->HasElement("jointName")) {
        this->bumper_joint = _model->GetJoint(_sdf->Get<std::string>("jointName"));
    }

    this->update_connection = event::Events::ConnectWorldUpdateBegin(std::bind(&BumperPlugin::OnUpdate, this));
}

void BumperPlugin::OnUpdate() {
    this->bumper_joint->SetForce(1, -1*this->joint_force);

    this->bool_msg.data = (this->bumper_joint->Position() >= this->trigger_position);
    this->bumper_pub.publish(bool_msg);
}
}
