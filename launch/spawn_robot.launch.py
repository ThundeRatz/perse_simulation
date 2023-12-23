from ament_index_python import get_package_share_directory
from os import path

import launch
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction, RegisterEventHandler
from launch.event_handlers import OnProcessExit
from launch_ros.actions import Node, PushRosNamespace
from launch.substitutions import LaunchConfiguration

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

    xacro_file = model
    urdf_file = path.join(perse_simulation_share, 'models/Perse/model.urdf')


    # Convert xacro to URDF
    xacro_to_urdf = launch.actions.ExecuteProcess(
        cmd=['xacro', xacro_file, '-o', urdf_file],
        output='screen'
    )

    robot_state_publisher = Node(
        namespace=ns,
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'use_sim_time': True}],
        arguments=[urdf_file]
    )

    synchronous_robot_state_publisher = RegisterEventHandler(
        OnProcessExit(
            target_action=xacro_to_urdf,
            on_exit=[robot_state_publisher]
        )
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
            # '-file', urdf_file
            '-topic', 'robot_description'
        ],
        output='screen'
    )

    # joints_yaml = launch.actions.IncludeLaunchDescription(
    #     launch.launch_description_sources.PythonLaunchDescriptionSource([
    #         path.join(perse_simulation_share, 'config/joints.yaml')
    #     ])
    # )

    # rqt_robot_steering = Node(
    #     package='rqt_robot_steering',
    #     executable='rqt_robot_steering',
    #     parameters=[{'default_topic': '/perse/actuator/cmd_vel'}]
    # )

    # diff_drive_yaml = launch.actions.IncludeLaunchDescription(
    #     launch.launch_description_sources.PythonLaunchDescriptionSource([
    #         path.join(perse_simulation_share, 'config/diff_drive.yaml')
    #     ])
    # )

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
                xacro_to_urdf,
                synchronous_robot_state_publisher,
                urdf_spawner
                # joints_yaml,
                # rqt_robot_steering,
                # diff_drive_yaml,
                # robot_controller
            ]
        )
    ])
