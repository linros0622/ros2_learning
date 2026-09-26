#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class ParamPublisher : public rclcpp::Node {
public:
    ParamPublisher() : Node("param_publisher") {
        // 声明参数：线性速度、角速度
        this->declare_parameter<double>("linear_speed", 0.2);
        this->declare_parameter<double>("angular_speed", 0.5);

        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        timer_ = this->create_wall_timer(100ms, std::bind(&ParamPublisher::timer_callback, this));
    }

private:
    void timer_callback() {
        // 每次回调都读取参数（这样运行时改了参数立刻生效）
        double lin = this->get_parameter("linear_speed").as_double();
        double ang = this->get_parameter("angular_speed").as_double();

        auto msg = geometry_msgs::msg::Twist();
        msg.linear.x = lin;
        msg.angular.z = ang;

        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "速度: 线%.2f 角%.2f", lin, ang);
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ParamPublisher>());
    rclcpp::shutdown();
    return 0;
}
