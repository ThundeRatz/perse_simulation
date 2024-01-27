/**
 * @file inductive_sensor_plugin.cpp
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

#include "inductive_sensor_plugin.hpp"

namespace gazebo {
GZ_REGISTER_MODEL_PLUGIN(InductiveSensorPlugin)

void InductiveSensorPlugin::declare_cones_parameters() {
    this->ros_node->declare_parameter("number_of_points", rclcpp::PARAMETER_INTEGER);

    for (int i = 1; i <= ros_node->get_parameter("number_of_points").as_int(); i++) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "[InductiveSensor] Declarando parâmetro: p%d", i);
        Cone cone;
        cone.x = ros_node->declare_parameter("p" + std::to_string(i) + ".x", rclcpp::PARAMETER_DOUBLE).get<double>();
        cone.y = ros_node->declare_parameter("p" + std::to_string(i) + ".y", rclcpp::PARAMETER_DOUBLE).get<double>();
        this->cones.push_back(cone);
    }
}

void InductiveSensorPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "InductiveSensor plugin: Starting!");
    this->model = _model;

    this->ros_node = gazebo_ros::Node::Get(_sdf, "inductive_sensor_plugin_node");

    try {
        this->declare_cones_parameters();
    } catch (...) {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "[InductiveSensor] Erro ao declarar os parâmetros");
    }

    this->update_connection = event::Events::ConnectWorldUpdateBegin(std::bind(&InductiveSensorPlugin::OnUpdate, this));

    // Get name of the topic to be published and create the publiser
    if (_sdf->HasElement("topicName")) {
        std::string topic_name = _sdf->Get<std::string>("topicName");
        this->inductive_sensor_pub = this->ros_node->create_publisher<std_msgs::msg::Bool>(topic_name, MSG_BUFFER);
    }

}

void InductiveSensorPlugin::OnUpdate() {
    ignition::math::Vector3 position = this->model->WorldPose().Pos();

    this->pub_msg.data = false;
    for(auto & cone : this->cones){
        if(position.X() > cone.x - PLATE_SIDE && position.X() < cone.x + PLATE_SIDE
                && position.Y() > cone.y - PLATE_SIDE && position.Y() < cone.y + PLATE_SIDE){

            this->pub_msg.data = true;
            break;
        }
    }

    this->inductive_sensor_pub->publish(pub_msg);
}
}
