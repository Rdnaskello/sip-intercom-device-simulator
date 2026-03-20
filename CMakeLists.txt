cmake_minimum_required(VERSION 3.16)

project(sip-intercom-device-simulator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(sip_intercom_device_simulator
    src/main.cpp
    src/app/Application.cpp
    src/config/ConfigLoader.cpp
    src/logging/Logger.cpp
)

target_include_directories(sip_intercom_device_simulator
    PRIVATE
        include
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sip_intercom_device_simulator
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()