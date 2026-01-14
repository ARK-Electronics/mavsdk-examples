#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h>
#include <mavsdk/log_callback.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <future>
#include <memory>
#include <thread>

using namespace mavsdk;
using std::chrono::seconds;

void usage(const std::string& bin_name)
{
    std::cerr << "Usage : " << bin_name << " <connection_url>\n"
              << "Connection URL format should be :\n"
              << " For TCP : tcp://[server_host][:server_port]\n"
              << " For UDP : udp://[bind_host][:bind_port]\n"
              << " For Serial : serial:///path/to/serial/dev[:baudrate]\n"
              << "For example, to connect to the simulator use URL: udpin://0.0.0.0:14540\n";
}

int main(int argc, char** argv)
{
    std::cout << "esc_info subscriber" << std::endl;
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    // Silence mavsdk noise
    mavsdk::log::subscribe([](...) {
        // https://mavsdk.mavlink.io/main/en/cpp/guide/logging.html
        return true;
    });

    Mavsdk mavsdk{Mavsdk::Configuration{1, MAV_COMP_ID_ONBOARD_COMPUTER3, true}};
    ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);

    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    auto system = mavsdk.first_autopilot(10);
    if (!system) {
        std::cerr << "Timed out waiting for system" << std::endl;
        return 1;
    }

    std::cout << "Connected" << std::endl;

    auto mavlink_passthrough = MavlinkPassthrough{system.value()};

    // Subscribe to ESC_INFO (message ID 290)
    mavlink_passthrough.subscribe_message(
        MAVLINK_MSG_ID_ESC_INFO,
        [](const mavlink_message_t& message) {
            mavlink_esc_info_t esc_info;
            mavlink_msg_esc_info_decode(&message, &esc_info);

            std::cout << "ESC_INFO: index=" << static_cast<int>(esc_info.index)
                      << " info=" << static_cast<int>(esc_info.info)
                      << std::endl;
        }
    );

    std::cout << "Waiting for ESC_INFO messages..." << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
