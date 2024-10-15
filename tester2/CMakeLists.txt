cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(tester2)

find_package(MAVSDK REQUIRED)

add_executable(tester2
    tester2.cpp
)

set_target_properties(tester2 PROPERTIES OUTPUT_NAME tester2)

target_link_libraries(tester2
    MAVSDK::mavsdk
)

install(TARGETS tester2 RUNTIME DESTINATION bin)

if(NOT MSVC)
    add_compile_options(tester2 PRIVATE -Wall -Wextra)
else()
    add_compile_options(tester2 PRIVATE -WX -W2)
endif()
