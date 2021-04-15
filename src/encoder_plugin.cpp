#include <gazebo/common/Plugin.hh>
#include <thread>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "std_msgs/Float64MultiArray.h"
#include "sensor_msgs/JointState.h"
#include <cmath>

#define radius 0.06
#define resolution 2


namespace gazebo
{
	class EncoderPlugin : public WorldPlugin
	{

	/// A node use for ROS transport
	private: ros::NodeHandle* rosNode;

	/// A ROS subscriber
	private: ros::Subscriber rosSub;

	/// A ROS callbackqueue that helps process messages
	private: ros::CallbackQueue rosQueue;

	/// A thread the keeps running the rosQueue
	private: std::thread rosQueueThread;

	/// A ROS publisher
	private: ros::Publisher rosPub;
		public:
			

			EncoderPlugin() : WorldPlugin(){}

			void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
			{
				// Make sure the ROS node for Gazebo has already been initialized
				if (!ros::isInitialized())
				{
					ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
						<< "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
					return;
				}

				ROS_INFO("Encoder plugin: Starting!");


				// Create our ROS node. This acts in a similar manner to
				// the Gazebo node
				//this->rosNode.reset(new ros::NodeHandle("gazebo_client"));
				this->rosNode = new ros::NodeHandle("gazebo_client"); //node name

				// Create a named topic, and subscribe to it.
				ros::SubscribeOptions so =
					ros::SubscribeOptions::create<sensor_msgs::JointState>(
							"/thunderwaze/joint_states",
							1,
							boost::bind(&EncoderPlugin::callback, this, _1),
							ros::VoidPtr(), &this->rosQueue);
				this->rosSub = this->rosNode->subscribe(so);
				//this->rosSub = this->rosNode->subscribe("thunderwaze/joint_states", 10 ,&EncoderPlugin::OnRosMsg, this);



				// Spin up the queue helper thread.
				this->rosQueueThread =
					std::thread(std::bind(&EncoderPlugin::QueueThread, this));

				this->rosPub = this->rosNode->advertise<std_msgs::Float64MultiArray>("distance", 1000);
			}

			public: void callback(const sensor_msgs::JointStateConstPtr &_msg)
			{

				/**
				 *  sensor_msgs::JointState.position[i] returns the rotation angle of some wheel joints in radians
				 * 
				 *  msg.position[0] -> wheel_back_left_joint
				 *  msg.position[1] -> wheel_back_right_joint
				 *  msg.position[2] -> wheel_front_left_joint
				 *  msg.position[3] -> wheel_front_right_joint
				 */
				
				const double right_angle = *(&_msg->position[3]);
				const double left_angle = *(&_msg->position[2]);
				
				float angle_division = 2*M_PI/resolution;

				float right_distance = floor(right_angle/angle_division)*angle_division*wheel_radius;
				float left_distance = floor(left_angle/angle_division)*angle_division*wheel_radius;
				

				std_msgs::Float64MultiArray pubMsg;
				pubMsg.data = {right_distance, left_distance};
				this->rosPub.publish(pubMsg);

			}

			// ROS helper function that processes messages
			private: void QueueThread()
			{
				static const double timeout = 0.01;
				while (this->rosNode->ok())
				{
					this->rosQueue.callAvailable(ros::WallDuration(timeout));
				}
			}
	};
GZ_REGISTER_WORLD_PLUGIN(EncoderPlugin)
}
