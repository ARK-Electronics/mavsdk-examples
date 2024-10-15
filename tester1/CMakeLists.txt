cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(tester1)

find_package(MAVSDK REQUIRED)

add_executable(tester1
    tester1.cpp
)

set_target_properties(tester1 PROPERTIES OUTPUT_NAME tester1)

target_link_libraries(tester1
    MAVSDK::mavsdk
)

install(TARGETS tester1 RUNTIME DESTINATION bin)

if(NOT MSVC)
    add_compile_options(tester1 PRIVATE -Wall -Wextra)
else()
    add_compile_options(tester1 PRIVATE -WX -W2)
endif()
