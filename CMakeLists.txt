cmake_minimum_required(VERSION 3.16)
project(GaitForge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 REQUIRED)
include_directories(${Eigen3_INCLUDE_DIRS})

# Add SDL submodule
add_subdirectory(vendored/SDL)

include_directories(include)

# Your main executable
add_executable(GaitForge 
                main.cpp 
                src/render_utils.cpp
)

# Link SDL3 to your project
target_link_libraries(GaitForge PRIVATE SDL3::SDL3)