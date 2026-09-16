# ros2_learning

我的 ROS2 学习代码仓库。

## 内容

- `src/my_first_pkg/` — 第一个 ROS2 C++ 功能包
  - `publisher_node.cpp` — 发布者节点，每秒发布一条字符串消息

## 环境

- Ubuntu 22.04 (WSL2)
- ROS2 Humble

## 编译运行

```bash
cd ~/ros2_ws
colcon build --packages-select my_first_pkg
source install/setup.bash
ros2 run my_first_pkg publisher_node
