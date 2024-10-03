cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(comms_check)

find_package(MAVSDK REQUIRED)

add_executable(comms_check
    comms_check.cpp
)

set_target_properties(comms_check PROPERTIES OUTPUT_NAME comms_check)

target_link_libraries(comms_check
    MAVSDK::mavsdk
)

install(TARGETS comms_check RUNTIME DESTINATION bin)

if(NOT MSVC)
    add_compile_options(comms_check PRIVATE -Wall -Wextra)
else()
    add_compile_options(comms_check PRIVATE -WX -W2)
endif()
