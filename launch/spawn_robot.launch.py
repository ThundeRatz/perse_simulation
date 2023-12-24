from ament_index_python import get_package_share_directory
from os import path

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction
from launch_ros.actions import Node, PushRosNamespace
from launch.substitutions import Command, LaunchConfiguration

def generate_launch_description():
    perse_simulation_share = get_package_share_directory('perse_simulation')

    # robot_number = LaunchConfiguration('robot_number', default='1')
    # team_number = LaunchConfiguration('team_number', default='0')
    robot_name = LaunchConfiguration('robot_name', default='perse')
    x = LaunchConfiguration('x', default='0')
    y = LaunchConfiguration('y', default='0')
    z = LaunchConfiguration('z', default='0.4')
    row = LaunchConfiguration('row', default='0')
    pitch = LaunchConfiguration('pitch', default='0')
    yaw = LaunchConfiguration('yaw', default='0')
    model = LaunchConfiguration('model', default=path.join(perse_simulation_share, 'models/Perse/urdf/perse.xacro'))

    ns = 'perse'

    xacro_command = Command(['xacro ', model])


    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'use_sim_time': False, 'robot_description': xacro_command}],
        # arguments=[urdf_file]
    )

    joint_state_publisher_gui_node = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        name='joint_state_publisher_gui',
        parameters=[{'robot_description': xacro_command}],
    )


    # joint_state_publisher = Node(
    #     package='joint_state_publisher',
    #     executable='joint_state_publisher',
    #     parameters=[{'use_sim_time': True, 'robot_description': xacro_command}],
    #     # arguments=[urdf_file]
    # )

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', path.join(perse_simulation_share, 'models/Perse/perse.rviz')],
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
        output='screen'
    )

    rqt_robot_steering = Node(
        package='rqt_robot_steering',
        executable='rqt_robot_steering',
        parameters=[{'default_topic': '/perse/actuator/cmd_vel'}]
    )

    # robot_controller = Node(
    #     package='controller_manager',
    #     executable='spawner',
    #     arguments=['actuator', 'robot_joint_state_controller'],
    #     parameters=[
    #         path.join(perse_simulation_share, 'config/joints.yaml'),
    #         path.join(perse_simulation_share, 'config/diff_drive.yaml'),
    #     ]
    # )

    return LaunchDescription([
        DeclareLaunchArgument('robot_number', default_value='1'),
        DeclareLaunchArgument('team_number', default_value='0'),
        DeclareLaunchArgument('robot_name', default_value='perse'),
        DeclareLaunchArgument('x', default_value='0'),
        DeclareLaunchArgument('y', default_value='0'),
        DeclareLaunchArgument('z', default_value='0.4'),
        DeclareLaunchArgument('row', default_value='0'),
        DeclareLaunchArgument('pitch', default_value='0'),
        DeclareLaunchArgument('yaw', default_value='0'),
        DeclareLaunchArgument('model', default_value=path.join(perse_simulation_share, 'models/Perse/urdf/perse.xacro')),

        GroupAction(
            actions=[
                PushRosNamespace(ns),
                robot_state_publisher,
                joint_state_publisher_gui_node,
                # joint_state_publisher,

                urdf_spawner,
                rviz_node,
                # robot_controller,
                # rqt_robot_steering,
            ]
        )
    ])
