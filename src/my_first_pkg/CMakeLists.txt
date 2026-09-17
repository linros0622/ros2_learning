cmake_minimum_required(VERSION 3.8)
project(my_first_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 找到依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# 生成服务接口
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/AddTwoInts.srv"
)

# 发布者节点
add_executable(publisher_node src/publisher_node.cpp)
ament_target_dependencies(publisher_node rclcpp std_msgs)

# 订阅者节点
add_executable(subscriber_node src/subscriber_node.cpp)
ament_target_dependencies(subscriber_node rclcpp std_msgs)

# 服务端
add_executable(add_server src/add_server.cpp)
ament_target_dependencies(add_server rclcpp rosidl_typesupport_cpp)
target_link_libraries(add_server ${PROJECT_NAME}__rosidl_typesupport_cpp)

# 客户端
add_executable(add_client src/add_client.cpp)
ament_target_dependencies(add_client rclcpp rosidl_typesupport_cpp)
target_link_libraries(add_client ${PROJECT_NAME}__rosidl_typesupport_cpp)

# 安装
install(TARGETS
    publisher_node
    subscriber_node
    add_server
    add_client
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()
