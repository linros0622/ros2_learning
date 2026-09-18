#include <memory>
#include <chrono>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

using namespace std::chrono_literals;

class DynamicTFBroadcaster : public rclcpp::Node {
public:
    DynamicTFBroadcaster() : Node("dynamic_tf_broadcaster"), t_(0.0) {
        broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
        timer_ = this->create_wall_timer(
            100ms, std::bind(&DynamicTFBroadcaster::publish_tf, this));
    }

private:
    void publish_tf() {
        geometry_msgs::msg::TransformStamped tf;
        tf.header.stamp = this->get_clock()->now();
        tf.header.frame_id = "odom";
        tf.child_frame_id = "base_link";

        // 模拟机器人绕圈移动
        tf.transform.translation.x = std::cos(t_) * 1.0;
        tf.transform.translation.y = std::sin(t_) * 1.0;
        tf.transform.translation.z = 0.0;

        // 朝向跟运动方向一致
        tf.transform.rotation.x = 0.0;
        tf.transform.rotation.y = 0.0;
        tf.transform.rotation.z = std::sin(t_ / 2);
        tf.transform.rotation.w = std::cos(t_ / 2);

        broadcaster_->sendTransform(tf);
        t_ += 0.1;
    }

    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
    rclcpp::TimerBase::SharedPtr timer_;
    double t_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DynamicTFBroadcaster>());
    rclcpp::shutdown();
    return 0;
}
