#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class CmdVelPublisher : public rclcpp::Node {
public:
    CmdVelPublisher() : Node("cmd_vel_publisher"), count_(0) {
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        timer_ = this->create_wall_timer(100ms, std::bind(&CmdVelPublisher::timer_callback, this));
    }

private:
    void timer_callback() {
        auto msg = geometry_msgs::msg::Twist();

        if (count_ < 50) {
            // 前 5 秒：直行
            msg.linear.x = 0.2;
            msg.angular.z = 0.0;
            RCLCPP_INFO(this->get_logger(), "直行");
        } else {
            // 之后：绕圈
            msg.linear.x = 0.15;
            msg.angular.z = 0.5;
            RCLCPP_INFO(this->get_logger(), "绕圈");
        }

        publisher_->publish(msg);
        count_++;
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    int count_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CmdVelPublisher>());
    rclcpp::shutdown();
    return 0;
}
