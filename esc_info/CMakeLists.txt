cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(esc_info)

find_package(MAVSDK REQUIRED)

add_executable(esc_info
    esc_info.cpp
)

set_target_properties(esc_info PROPERTIES OUTPUT_NAME esc_info)

target_link_libraries(esc_info
    MAVSDK::mavsdk
)

install(TARGETS esc_info RUNTIME DESTINATION bin)

if(NOT MSVC)
    add_compile_options(esc_info PRIVATE -Wall -Wextra)
else()
    add_compile_options(esc_info PRIVATE -WX -W2)
endif()
