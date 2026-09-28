#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

class ScanProcessor : public rclcpp::Node {
public:
    ScanProcessor() : Node("scan_processor") {
        subscription_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "scan", 10,
            std::bind(&ScanProcessor::scan_callback, this, std::placeholders::_1));
    }

private:
    void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
        // 找到“正前方”的索引（角度 = 0）
        int center_index = msg->ranges.size() / 2;
        float front_distance = msg->ranges[center_index];

        if (std::isinf(front_distance)) {
            RCLCPP_INFO(this->get_logger(), "正前方无障碍物");
        } else if (front_distance < 1.0) {
            RCLCPP_WARN(this->get_logger(), "⚠️ 正前方 %.2f 米有障碍物！", front_distance);
        } else {
            RCLCPP_INFO(this->get_logger(), "正前方 %.2f 米", front_distance);
        }
    }

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscription_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ScanProcessor>());
    rclcpp::shutdown();
    return 0;
}
