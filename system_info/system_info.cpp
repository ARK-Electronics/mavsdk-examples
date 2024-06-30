//
// Simple example to demonstrate how to query system info using MAVSDK.
//

#include <chrono>
#include <cstdint>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/info/info.h>
#include <mavsdk/log_callback.h>
#include <iostream>
#include <future>
#include <memory>
#include <thread>

using namespace mavsdk;
using std::chrono::seconds;
using std::this_thread::sleep_for;

void usage(const std::string& bin_name)
{
    std::cerr << "Usage : " << bin_name << " <connection_url>\n"
              << "Connection URL format should be :\n"
              << " For TCP : tcp://[server_host][:server_port]\n"
              << " For UDP : udp://[bind_host][:bind_port]\n"
              << " For Serial : serial:///path/to/serial/dev[:baudrate]\n"
              << "For example, to connect to the simulator use URL: udp://:14540\n";
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    // Silence mavsdk noise
    mavsdk::log::subscribe([](...) {
     // https://mavsdk.mavlink.io/main/en/cpp/guide/logging.html
     return true;
    });

    Mavsdk mavsdk{Mavsdk::Configuration{Mavsdk::ComponentType::GroundStation}};
    ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);

    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    auto system = mavsdk.first_autopilot(3.0);
    if (!system) {
        std::cerr << "Timed out waiting for system\n";
        return 1;
    }

    auto info = Info{system.value()};

    // Wait until version/firmware information has been populated from the vehicle
    while (info.get_identification().first == Info::Result::InformationNotReceivedYet) {
        std::cout << "Waiting for Version information to populate from system." << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    auto version = info.get_version().second;
    std::ostringstream version_string;
    version_string << version.flight_sw_major << '.' << version.flight_sw_minor << '.' << version.flight_sw_patch;

    std::string autopilot_type = "unknown";
    switch (system.value()->autopilot_type()) {
        case Autopilot::Px4:
            autopilot_type = "PX4";
            break;
        case Autopilot::ArduPilot:
            autopilot_type = "ArduPilot";
            break;
        default:
            break;
    }

    // Manually constructing the JSON output
    std::ostringstream json_output;
    json_output << "{\n"
                << "  \"version\": \"" << version_string.str() << "\",\n"
                << "  \"git_hash\": \"" << version.flight_sw_git_hash << "\",\n"
                << "  \"autopilot_type\": \"" << autopilot_type << "\"\n"
                << "}";

    std::cout << json_output.str() << std::endl;

    return 0;
}
