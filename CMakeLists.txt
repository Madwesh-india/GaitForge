cmake_minimum_required(VERSION 3.16)
project(GaitForge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Eigen
find_package(Eigen3 REQUIRED)

# SDL
add_subdirectory(vendored/SDL)

# Main executable
add_executable(GaitForge 
    main.cpp 
    src/render_utils.cpp
    src/line.cpp
)

# Include directories
target_include_directories(GaitForge PRIVATE include)

# Link libraries
target_link_libraries(GaitForge PRIVATE SDL3::SDL3 Eigen3::Eigen)
