# 🧭 ThunderWaze simulation with Gazebo
<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-7-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

Simulation package for Thunderatz's robot ThunderWaze v.4.0, build with [ROS](http://wiki.ros.org/ROS/Introduction) and [Gazebo](http://www.gazebosim.org/tutorials?tut=guided_b1)

## ⏯ Intro

Fork of [vss_simulation](https://github.com/ThundeRatz/vss_simulation)

To run the simulation:

```bash
roslaunch thunderwaze gazebo.launch
```

## 🕹 Control

The simulation receives a [geometry_msgs/Twist](http://docs.ros.org/noetic/api/geometry_msgs/html/msg/Twist.html) describing the robot movement

```c
# This expresses velocity in free space broken into its linear and angular parts.
Vector3  linear
Vector3  angular
```

Future versions should reveice 2 std_msgs/Float64 messages for the motor speed and the servo direction

## Dependencies

To install ROS and Gazebo, use the command:

```bash
sudo apt install ros-noetic-desktop-full
```

The project requires the libraries velocity_controllers and position_controllers inside [ros_controllers](https://github.com/ros-controls/ros_controllers) and the python library [pygame](https://github.com/pygame/pygame). It is possible to install with ```apt-get```

```bash
sudo apt install ros-noetic-position-controllers ros-noetic-velocity-controllers python-pygame
```

Or with ```rosdep```

```bash
rosdep install thunderwaze
```

## 🎨 Gazebo colors

To a complete list of default Gazebo colors, see the oficial repo [material script](https://bitbucket.org/osrf/gazebo/src/gazebo11/media/materials/scripts/gazebo.material).

To create new colors, check the OGRE script inside ./media/materials/scripts

## 📷 Screenshots

![foto1](./docs/thunderwaze-1.png)

![foto2](./docs/thunderwaze-2.png)

![foto3](./docs/thunderwaze-3.png)

## Contributors ✨

Thanks goes to these wonderful people ([emoji key](https://allcontributors.org/docs/en/emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://github.com/DamaralHenrique"><img src="https://avatars.githubusercontent.com/u/62445591?v=4?s=100" width="100px;" alt=""/><br /><sub><b>DamaralHenrique</b></sub></a><br /><a href="https://github.com/ThundeRatz/perse_simulation/commits/develop?author=DamaralHenrique" title="Code">💻</a> <a href="https://github.com/ThundeRatz/perse_simulation/commits/develop?author=DamaralHenrique" title="Documentation">📖</a> <a href="https://github.com/ThundeRatz/perse_simulation/pulls?q=is%3Apr+reviewed-by%3ADamaralHenrique" title="Reviewed Pull Requests">👀</a></td>
    <td align="center"><a href="https://github.com/mariarezende07"><img src="https://avatars.githubusercontent.com/u/54939351?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Maria Fernanda Rezende</b></sub></a><br /><a href="https://github.com/ThundeRatz/perse_simulation/commits/develop?author=mariarezende07" title="Code">💻</a> <a href="https://github.com/ThundeRatz/perse_simulation/commits/develop?author=mariarezende07" title="Documentation">📖</a> <a href="https://github.com/ThundeRatz/perse_simulation/pulls?q=is%3Apr+reviewed-by%3Amariarezende07" title="Reviewed Pull Requests">👀</a></td>
    <td align="center"><a href="https://github.com/guedeslucas1"><img src="https://avatars.githubusercontent.com/u/49926722?v=4?s=100" width="100px;" alt=""/><br /><sub><b>guedeslucas1</b></sub></a><br /><a href="https://github.com/ThundeRatz/perse_simulation/commits/develop?author=guedeslucas1" title="Code">💻</a> <a href="https://github.com/ThundeRatz/perse_simulation/commits/develop?author=guedeslucas1" title="Documentation">📖</a> <a href="https://github.com/ThundeRatz/perse_simulation/pulls?q=is%3Apr+reviewed-by%3Aguedeslucas1" title="Reviewed Pull Requests">👀</a></td>
    <td align="center"><a href="https://github.com/FelipeGdM"><img src="https://avatars.githubusercontent.com/u/1054087?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Felipe Gomes de Melo</b></sub></a><br /><a href="https://github.com/ThundeRatz/perse_simulation/pulls?q=is%3Apr+reviewed-by%3AFelipeGdM" title="Reviewed Pull Requests">👀</a></td>
    <td align="center"><a href="https://github.com/febagni"><img src="https://avatars.githubusercontent.com/u/39801091?v=4?s=100" width="100px;" alt=""/><br /><sub><b>febagni</b></sub></a><br /><a href="https://github.com/ThundeRatz/perse_simulation/pulls?q=is%3Apr+reviewed-by%3Afebagni" title="Reviewed Pull Requests">👀</a></td>
    <td align="center"><a href="https://github.com/d-nery"><img src="https://avatars.githubusercontent.com/u/12468184?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Daniel Nery</b></sub></a><br /><a href="https://github.com/ThundeRatz/perse_simulation/pulls?q=is%3Apr+reviewed-by%3Ad-nery" title="Reviewed Pull Requests">👀</a></td>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/Tocoquinho"><img src="https://avatars.githubusercontent.com/u/37677881?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Tocoquinho</b></sub></a><br /><a href="https://github.com/ThundeRatz/perse_simulation/commits/develop?author=Tocoquinho" title="Code">💻</a> <a href="https://github.com/ThundeRatz/perse_simulation/commits/develop?author=Tocoquinho" title="Documentation">📖</a> <a href="https://github.com/ThundeRatz/perse_simulation/pulls?q=is%3Apr+reviewed-by%3ATocoquinho" title="Reviewed Pull Requests">👀</a></td>
  </tr>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification. Contributions of any kind welcome!