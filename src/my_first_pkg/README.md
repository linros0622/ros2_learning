# 移动机器人控制系统（ROS2 + Gazebo）

基于 ROS2 Humble 和 Gazebo 仿真的移动机器人，实现自主避障功能。

## 功能

- URDF 机器人建模（两轮差速底盘 + 激光雷达）
- Gazebo 仿真环境加载
- 激光雷达数据接收（/scan 话题）
- 自动避障（订阅 /scan，发布 /cmd_vel）

## 环境

- Ubuntu 22.04
- ROS2 Humble
- Gazebo 11

## 编译

cd ~/ros2_ws
colcon build --packages-select my_first_pkg
source install/setup.bash

## 运行

终端 1：启动 Gazebo
source /usr/share/gazebo/setup.sh
gazebo -s libgazebo_ros_factory.so

终端 2：发布机器人描述
cd ~/ros2_ws
source install/setup.bash
ros2 run robot_state_publisher robot_state_publisher --ros-args -p robot_description:="$(cat ~/ros2_ws/src/my_first_pkg/urdf/my_robot.urdf)"

终端 3：生成机器人
cd ~/ros2_ws
source install/setup.bash
ros2 run gazebo_ros spawn_entity.py -topic robot_description -entity robot_final -x 0 -y 0 -z 0.1

终端 4：自动避障
cd ~/ros2_ws
source install/setup.bash
ros2 run my_first_pkg auto_avoid
