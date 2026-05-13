cmake_minimum_required(VERSION 3.5.0)
project("toy2tools" CXX)

set(CMAKE_CXX_STANDARD 23)
set(EXTERNAL_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../external")

# Build SaveViewer
add_executable(SaveViewer 
    SaveViewer/SaveViewer.cpp
    SaveViewer/SaveViewer.hpp
)

target_compile_definitions(SaveViewer PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:NO_UNICODE>
)

# Build RawDecompressor
add_executable(RawDecompressor 
    RawDecompressor/main.cpp
)

target_compile_options(RawDecompressor PRIVATE -m32)
target_link_options(RawDecompressor PRIVATE -m32)

# Build DatDumper
add_executable(DatDumper 
    DatDumper/main.cpp
)

target_compile_options(DatDumper PRIVATE -m32)
target_link_options(DatDumper PRIVATE -m32)

# Build ToyDebugger

# ImGui Sources for ToyDebugger
set(IMGUI_DIR ${EXTERNAL_FOLDER}/submodules/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp

    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
)

add_library(ToyDebugger SHARED
    ToyDebugger/main.cpp
    
    ToyDebugger/imgui_impl_d3d3.cpp
    ToyDebugger/imgui_impl_d3d3.hpp

    ToyDebugger/ImGuiWrapper.cpp
    ToyDebugger/ImGuiWrapper.hpp

    ToyDebugger/NoClip.hpp
    ToyDebugger/GameInterface.hpp

    ToyDebugger/FreeCam.cpp
    ToyDebugger/FreeCam.hpp

    ${IMGUI_SOURCES}
)

target_compile_options(ToyDebugger PRIVATE -m32)
target_link_options(ToyDebugger PRIVATE
    -m32
    "LINKER:/SAFESEH:NO"
)

target_link_directories(ToyDebugger PRIVATE ${EXTERNAL_FOLDER}/libs)
target_link_libraries(ToyDebugger PRIVATE minhook_x86 ddraw dxguid)
target_include_directories(ToyDebugger PRIVATE
    ${EXTERNAL_FOLDER}/include
    ${EXTERNAL_FOLDER}/submodules/imgui
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
