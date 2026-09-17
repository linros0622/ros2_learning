#include <memory>
#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/static_transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

using namespace std::chrono_literals;

class StaticTFBroadcaster : public rclcpp::Node {
public:
    StaticTFBroadcaster() : Node("static_tf_broadcaster") {
        broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        publish_tf();
    }

private:
    void publish_tf() {
        geometry_msgs::msg::TransformStamped tf;
        tf.header.stamp = this->get_clock()->now();
        tf.header.frame_id = "base_link";
        tf.child_frame_id = "laser";

        // 平移：激光雷达在底盘前方 0.3 米、上方 0.2 米
        tf.transform.translation.x = 0.3;
        tf.transform.translation.y = 0.0;
        tf.transform.translation.z = 0.2;

        // 旋转：不旋转（四元数表示）
        tf.transform.rotation.x = 0.0;
        tf.transform.rotation.y = 0.0;
        tf.transform.rotation.z = 0.0;
        tf.transform.rotation.w = 1.0;

        broadcaster_->sendTransform(tf);
        RCLCPP_INFO(this->get_logger(), "已发布 base_link → laser 的TF");
    }

    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcaster_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<StaticTFBroadcaster>());
    rclcpp::shutdown();
    return 0;
}
