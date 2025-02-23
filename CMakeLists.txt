cmake_minimum_required(VERSION 3.15)
project(client)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find libraries

# Find SDL2_net
find_package(SDL2_net REQUIRED)
# Find SDL3
find_package(SDL3 REQUIRED)

# Source files
set(SOURCES     src/client/Client.cpp)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
include_directories(
    )

# Ling against
target_link_libraries(${PROJECT_NAME}  SDL2_net::SDL2_net SDL3::SDL3)


