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

void send_camera_trigger(MavlinkPassthrough& mavlink_passthrough, uint32_t counter);

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
    std::cout << "tester2" << std::endl;
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    // Silence mavsdk noise
    mavsdk::log::subscribe([](...) {
     // https://mavsdk.mavlink.io/main/en/cpp/guide/logging.html
     return true;
    });

    Mavsdk mavsdk{Mavsdk::Configuration{1, MAV_COMP_ID_ONBOARD_COMPUTER4,true}};
    ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);

    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    auto system = mavsdk.first_autopilot(5.0);
    if (!system) {
        std::cerr << "Timed out waiting for system" << std::endl;
        return 1;
    }

    std::cout << "Connected" << std::endl;

    auto mavlink_passthrough = MavlinkPassthrough{system.value()};


    auto camera_trigger_callback = [](const mavlink_message_t& message) {
        std::cout << "received CAMERA_TRIGGER" << std::endl;
    };

    mavlink_passthrough.subscribe_message(
        MAVLINK_MSG_ID_CAMERA_TRIGGER,
        camera_trigger_callback
    );

    uint32_t counter = 0;
    while (true) {
        send_camera_trigger(mavlink_passthrough, counter++);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

void send_camera_trigger(MavlinkPassthrough& mavlink_passthrough, uint32_t counter)
{
    std::cout << "sending " << counter << std::endl;
    mavlink_passthrough.queue_message([&](MavlinkAddress mavlink_address, uint8_t channel) {
        mavlink_message_t message;
        mavlink_msg_camera_trigger_pack_chan(
            mavlink_address.system_id,
            mavlink_address.component_id,
            channel,
            &message,
            0, // time_usec
            counter); // seq
        return message;
    });
}
