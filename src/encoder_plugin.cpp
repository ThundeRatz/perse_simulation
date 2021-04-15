#include <gazebo/common/Plugin.hh>
#include <ros/ros.h>

namespace gazebo
{
class EncoderPlugin : public WorldPlugin
{
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
  }



};
GZ_REGISTER_WORLD_PLUGIN(EncoderPlugin)
}