//
// Simple example to demonstrate how to query system info using MAVSDK.
//

#include <chrono>
#include <cstdint>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/info/info.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/log_callback.h>
#include <atomic>
#include <iostream>
#include <future>
#include <memory>
#include <thread>

using namespace mavsdk;
using std::chrono::seconds;
using std::this_thread::sleep_for;

float _voltage = {};
float _current = {};
float _remaining = {};

std::atomic<bool> _received_battery_status = false;

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

    Mavsdk mavsdk{Mavsdk::Configuration{1, MAV_COMP_ID_ONBOARD_COMPUTER3,true}};
    ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);

    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    auto system = mavsdk.first_autopilot(3.0);
    if (!system) {
        std::cerr << "Timed out waiting for system" << std::endl;
        return 1;
    }

    auto telemetry = Telemetry{system.value()};
    auto info = Info{system.value()};

    telemetry.subscribe_battery([](Telemetry::Battery battery) {
        _voltage = battery.voltage_v;
        _current = battery.current_battery_a;
        _remaining = battery.remaining_percent;
        _received_battery_status.store(true);
    });


    // Wait until version/firmware information has been populated from the vehicle
    int attempt = 0;
    int max_attempts = 3;
    while ((info.get_identification().first == Info::Result::InformationNotReceivedYet) ||
            !_received_battery_status.load()) {

        std::this_thread::sleep_for(std::chrono::seconds(1));
        attempt++;

        if (attempt >= max_attempts) break;
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
    json_output << "{\"version\": \"" << version_string.str() << "\", "
                << "\"git_hash\": \"" << version.flight_sw_git_hash.c_str() << "\", "
                << "\"autopilot_type\": \"" << autopilot_type.c_str() << "\", "
                << "\"voltage\": \"" << std::to_string(_voltage).c_str() << "\", "
                << "\"remaining\": \"" << std::to_string(_remaining).c_str() << "\", "
                << "\"current\": \"" << std::to_string(_current).c_str() << "\""
                << "}";

    std::cout << json_output.str() << std::endl;

    return 0;
}
