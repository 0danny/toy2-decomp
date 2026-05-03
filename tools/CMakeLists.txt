set(PROJECT_NAME "toy2tools")

cmake_minimum_required(VERSION 3.5.0)
project(PROJECT_NAME CXX)

set(CMAKE_CXX_STANDARD 23)

# Build SaveViewer
add_executable(SaveViewer 
    SaveViewer/SaveViewer.cpp
    SaveViewer/SaveViewer.hpp

    Common/Utils.hpp
)

# Build RawDecompressor
add_executable(RawDecompressor 
    RawDecompressor/main.cpp
)

target_compile_options(RawDecompressor PRIVATE -m32)
target_link_options(RawDecompressor PRIVATE -m32)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(SaveViewer PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:NO_UNICODE>
)