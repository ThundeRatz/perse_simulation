from ament_index_python import get_package_share_directory
from os import path

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction, RegisterEventHandler, IncludeLaunchDescription
from launch.conditions import IfCondition
from launch.launch_description_sources import AnyLaunchDescriptionSource
from launch_ros.actions import Node
from launch.substitutions import Command, LaunchConfiguration, TextSubstitution
from launch.event_handlers import OnProcessExit

def generate_launch_description():
    perse_simulation_share = get_package_share_directory('perse_simulation')

    x = LaunchConfiguration('x', default='0')
    y = LaunchConfiguration('y', default='0')
    z = LaunchConfiguration('z', default='0.4')
    row = LaunchConfiguration('row', default='0')
    pitch = LaunchConfiguration('pitch', default='0')
    yaw = LaunchConfiguration('yaw', default='0')
    robot_name = LaunchConfiguration('robot_name', default='perse')
    model = LaunchConfiguration('model', default=path.join(perse_simulation_share, 'models/Perse/urdf/perse.xacro'))
    should_run_robot_steering = LaunchConfiguration('robot_steering', default=TextSubstitution(text='false'))
    should_run_rviz = LaunchConfiguration('rviz', default=TextSubstitution(text='false'))
    should_run_perse = LaunchConfiguration('perse', default=TextSubstitution(text='true'))
    should_run_darknet = LaunchConfiguration('darknet', default=TextSubstitution(text='true'))

    xacro_command = Command(['xacro ', model])


    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'use_sim_time': True, 'robot_description': xacro_command}],
    )

    diff_drive_base_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["diff_drive_base_controller"],
    )

    joint_state_broadcaster = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster"],
    )

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', path.join(perse_simulation_share, 'models/Perse/perse.rviz')],
        condition=IfCondition(should_run_rviz),
    )

    urdf_spawner = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=[
            '-x', x,
            '-y', y,
            '-z', z,
            '-R', row,
            '-P', pitch,
            '-Y', yaw,
            '-entity', robot_name,
            '-topic', 'robot_description'
        ],
        output='screen',
    )

    rqt_robot_steering = Node(
        package='rqt_robot_steering',
        executable='rqt_robot_steering',
        parameters=[{'default_topic': '/perse/actuator/cmd_vel'}],
        condition=IfCondition(should_run_robot_steering),
    )

    darknet_ros_launch = IncludeLaunchDescription(
        AnyLaunchDescriptionSource(path.join(get_package_share_directory('darknet_ros'), 'launch/darknet_ros.launch.py')),
        condition=IfCondition(should_run_darknet),
    )
    gazebo_launch = IncludeLaunchDescription(
        AnyLaunchDescriptionSource(path.join(perse_simulation_share, 'launch/gazebo.launch'))
    )
    perse_launch = IncludeLaunchDescription(
        AnyLaunchDescriptionSource(path.join(get_package_share_directory('perse'), 'launch/perse.launch')),
        condition=IfCondition(should_run_perse),
    )

    return LaunchDescription([
        DeclareLaunchArgument('x', default_value='0'),
        DeclareLaunchArgument('y', default_value='0'),
        DeclareLaunchArgument('z', default_value='0.4'),
        DeclareLaunchArgument('row', default_value='0'),
        DeclareLaunchArgument('pitch', default_value='0'),
        DeclareLaunchArgument('yaw', default_value='0'),
        DeclareLaunchArgument('robot_name', default_value='perse'),
        DeclareLaunchArgument('model', default_value=path.join(perse_simulation_share, 'models/Perse/urdf/perse.xacro')),
        DeclareLaunchArgument('robot_steering', default_value=TextSubstitution(text='false')),
        DeclareLaunchArgument('rviz', default_value=TextSubstitution(text='false')),
        DeclareLaunchArgument('perse', default_value=TextSubstitution(text='true')),
        DeclareLaunchArgument('darknet', default_value=TextSubstitution(text='true')),
        DeclareLaunchArgument('odometry_variables', default_value=path.join(perse_simulation_share, 'config/odometry_variables.yaml')),

        GroupAction(
            actions=[
                gazebo_launch,
                robot_state_publisher,
                urdf_spawner,
                darknet_ros_launch,

                RegisterEventHandler(
                    event_handler=OnProcessExit(
                        target_action=urdf_spawner,
                        on_exit=[joint_state_broadcaster],
                    )
                ),
                RegisterEventHandler(
                    event_handler=OnProcessExit(
                        target_action=joint_state_broadcaster,
                        on_exit=[diff_drive_base_controller],
                    )
                ),

                RegisterEventHandler(
                    event_handler=OnProcessExit(
                        target_action=diff_drive_base_controller,
                        on_exit=[rviz_node, rqt_robot_steering, perse_launch],
                    )
                ),
            ]
        )
    ])
