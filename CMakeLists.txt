cmake_minimum_required(VERSION 3.10.2)

project(examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ftp_client)
add_subdirectory(system_info)
add_subdirectory(tester1)
add_subdirectory(tester2)
