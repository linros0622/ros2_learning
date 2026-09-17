#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "my_first_pkg/srv/add_two_ints.hpp"

using AddTwoInts = my_first_pkg::srv::AddTwoInts;

class AddServer : public rclcpp::Node {
public:
    AddServer() : Node("add_server") {
        service_ = this->create_service<AddTwoInts>(
            "add_two_ints",
            std::bind(&AddServer::handle_add, this,
                      std::placeholders::_1, std::placeholders::_2));
    }

private:
    void handle_add(const std::shared_ptr<AddTwoInts::Request> request,
                    std::shared_ptr<AddTwoInts::Response> response) {
        response->sum = request->a + request->b;
        RCLCPP_INFO(this->get_logger(), "收到请求: %ld + %ld = %ld",
                    request->a, request->b, response->sum);
    }

    rclcpp::Service<AddTwoInts>::SharedPtr service_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<AddServer>());
    rclcpp::shutdown();
    return 0;
}
