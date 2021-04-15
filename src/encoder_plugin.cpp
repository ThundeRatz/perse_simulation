#include <gazebo/common/Plugin.hh>
#include <thread>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "std_msgs/String.h"
#include "sensor_msgs/JointState.h"

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
				this->rosNode = new ros::NodeHandle("gazebo_client");

				// Create a named topic, and subscribe to it.
				ros::SubscribeOptions so =
					ros::SubscribeOptions::create<sensor_msgs::JointState>(
							"/thunderwaze/joint_states",
							1,
							boost::bind(&EncoderPlugin::OnRosMsg, this, _1),
							ros::VoidPtr(), &this->rosQueue);
				this->rosSub = this->rosNode->subscribe(so);
				//this->rosSub = this->rosNode->subscribe("thunderwaze/joint_states", 10 ,&EncoderPlugin::OnRosMsg, this);



				// Spin up the queue helper thread.
				this->rosQueueThread =
					std::thread(std::bind(&EncoderPlugin::QueueThread, this));

				this->rosPub = this->rosNode->advertise<std_msgs::String>("teste", 1000);
			}

			public: void OnRosMsg(const sensor_msgs::JointStateConstPtr &_msg)
			{
				ROS_INFO("ois");
				std_msgs::String msg;
				
				msg.data = "oi";
				this->rosPub.publish(msg);

			}

			/// \brief ROS helper function that processes messages
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