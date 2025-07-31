set(PROJECT_NAME "toy2tools")

cmake_minimum_required(VERSION 3.5.0)
project(PROJECT_NAME CXX)

set(CMAKE_CXX_STANDARD 20)

# Build SaveViewer
add_executable(SaveViewer 
    SaveViewer/SaveViewer.cpp
    SaveViewer/SaveViewer.hpp

    Common/Utils.hpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(SaveViewer PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:NO_UNICODE>
)