set(PROJECT_NAME "toy2decomp")

cmake_minimum_required(VERSION 3.5.0)
project(PROJECT_NAME C CXX RC)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src_main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src_testing)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/include)

set(CMAKE_CXX_FLAGS "/W3 /GX /D \"WIN32\" /D \"_WINDOWS\"")
set(CMAKE_CXX_FLAGS_DEBUG "/Gm /Zi /Od /D \"_DEBUG\"")
set(CMAKE_CXX_FLAGS_RELEASE "/O2 /D \"NDEBUG\"")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Zi /O2 /D \"NDEBUG\"")
set(CMAKE_CXX_FLAGS_MINSIZEREL "/Os /D \"NDEBUG\"")

set(CMAKE_EXE_LINKER_FLAGS "/machine:I386")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/incremental:yes /debug")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/incremental:no")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/incremental:no /debug")
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "/incremental:no")

set(CMAKE_SHARED_LINKER_FLAGS "/machine:I386")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/incremental:yes /debug")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/incremental:no")
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "/incremental:no /debug")
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "/incremental:no")

# Nu3D Sources
set(NU3D_SOURCES
    
)

# Toy2 Sources
set(TOY2_SOURCES
    src_main/Common.h
    
    src_main/Toy2/Toy2.cpp
    src_main/Toy2/Toy2.h

    src_main/Toy2/D3DApp.cpp
    src_main/Toy2/D3DApp.h

    src_main/Toy2/FileUtils.cpp
    src_main/Toy2/FileUtils.h
)

# Build the game
add_executable(toy2decomp WIN32 
    ${NU3D_SOURCES}
    ${TOY2_SOURCES}

    src_main/Logger.cpp
    src_main/Logger.h
)

# Build the testing dll(s)
add_library(test_dll SHARED
    src_testing/dllmain.cpp

    src_testing/loader.cpp
    src_testing/loader.h

    src_testing/game_patches.cpp
    src_testing/game_patches.h

    src_testing/utilities.h
)

target_link_directories(test_dll PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/libs)
target_link_libraries(test_dll PRIVATE minhook_x86)
