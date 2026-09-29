#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "geometry_msgs/msg/twist.hpp"

class AutoAvoid : public rclcpp::Node {
public:
    AutoAvoid() : Node("auto_avoid") {
        // 订阅雷达
        subscription_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "scan", 10,
            std::bind(&AutoAvoid::scan_callback, this, std::placeholders::_1));

        // 发布速度
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);

        // 定时器：每 100ms 发布一次速度
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&AutoAvoid::control_loop, this));

        RCLCPP_INFO(this->get_logger(), "自动避障节点启动");
    }

private:
    void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
        // 取正前方 30° 范围内的最小距离
        int center = msg->ranges.size() / 2;
        int range = 15;  // 前后各 15 个点
        float min_dist = 10.0;

        for (int i = center - range; i <= center + range; i++) {
            if (i >= 0 && i < (int)msg->ranges.size()) {
                float d = msg->ranges[i];
                if (!std::isinf(d) && d < min_dist) {
                    min_dist = d;
                }
            }
        }
        front_distance_ = min_dist;
    }

    void control_loop() {
        auto msg = geometry_msgs::msg::Twist();

        if (front_distance_ > 1.0) {
            // 前方安全，前进
            msg.linear.x = 0.15;
            msg.angular.z = 0.0;
        } else {
            // 前方有障碍物，原地左转
            msg.linear.x = 0.0;
            msg.angular.z = 0.5;
            RCLCPP_WARN(this->get_logger(), "障碍物 %.2f 米，转向！", front_distance_);
        }

        publisher_->publish(msg);
    }

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    float front_distance_ = 10.0;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<AutoAvoid>());
    rclcpp::shutdown();
    return 0;
}
