#include <memory>
#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "my_first_pkg/srv/add_two_ints.hpp"

using AddTwoInts = my_first_pkg::srv::AddTwoInts;
using namespace std::chrono_literals;

class AddClient : public rclcpp::Node {
public:
    AddClient() : Node("add_client") {
        client_ = this->create_client<AddTwoInts>("add_two_ints");
    }

    void send_request(int64_t a, int64_t b) {
        while (!client_->wait_for_service(1s)) {
            RCLCPP_INFO(this->get_logger(), "等待服务端...");
        }
        auto request = std::make_shared<AddTwoInts::Request>();
        request->a = a;
        request->b = b;
        auto future = client_->async_send_request(request);
        if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future)
            == rclcpp::FutureReturnCode::SUCCESS) {
            RCLCPP_INFO(this->get_logger(), "结果: %ld", future.get()->sum);
        } else {
            RCLCPP_ERROR(this->get_logger(), "服务调用失败");
        }
    }

private:
    rclcpp::Client<AddTwoInts>::SharedPtr client_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<AddClient>();
    node->send_request(3, 4);
    rclcpp::shutdown();
    return 0;
}
