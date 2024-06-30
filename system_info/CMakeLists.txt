cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(system_info)

find_package(MAVSDK REQUIRED)

add_executable(system_info
    system_info.cpp
)

set_target_properties(system_info PROPERTIES OUTPUT_NAME mavlink_autopilot_data)

target_link_libraries(system_info
    MAVSDK::mavsdk
)

install(TARGETS system_info RUNTIME DESTINATION bin)

if(NOT MSVC)
    add_compile_options(system_info PRIVATE -Wall -Wextra)
else()
    add_compile_options(system_info PRIVATE -WX -W2)
endif()
