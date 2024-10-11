cmake_minimum_required(VERSION 3.12)
project(RayTracer VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure we're using GCC 11 or later
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11)
    message(FATAL_ERROR "GCC version must be at least 11!")
  endif()
endif()

# Find OpenMP package for parallel execution
find_package(OpenMP REQUIRED)

# Find TBB package
find_package(TBB REQUIRED)

# Add the executable
add_executable(ray_tracer 
    main.cpp
    image.hpp
    ray.hpp
    scene.hpp
    shape.hpp
    sphere.hpp
    vec3.hpp
)

# Include the current directory
target_include_directories(ray_tracer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link OpenMP and TBB
target_link_libraries(ray_tracer PRIVATE OpenMP::OpenMP_CXX TBB::tbb)

# Enable compiler warnings and C++20 features
target_compile_options(ray_tracer PRIVATE -Wall -Wextra -Wpedantic -std=c++20)

# Find and link the rang library
include(FetchContent)
FetchContent_Declare(
    rang
    GIT_REPOSITORY https://github.com/agauniyal/rang.git
    GIT_TAG master
)
FetchContent_MakeAvailable(rang)
target_link_libraries(ray_tracer PRIVATE rang)

# Print the binary directory location
add_custom_command(TARGET ray_tracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Executable location: $<TARGET_FILE:ray_tracer>"
)
