cmake_minimum_required(VERSION 3.5)
project(server)

set(SOURCES     src/Server.cpp)

# Create an option to switch between a system sdl library and a vendored SDL library
option(SDL_VENDORED "Use vendored libraries" ON)

# Create an option to switch between a system sdl_net library and a vendored SDL library
option(SDL_NET_VENDORED "Use vendored libraries" ON)

if(SDL_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
	add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

if(SDL_NET_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
	add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL2_net EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL2_net REQUIRED CONFIG REQUIRED COMPONENTS SDL2_net-shared)
endif()

# Create your game executable target as usual
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

# Link to the actual SDL3 library.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL2_net::SDL2_net)
