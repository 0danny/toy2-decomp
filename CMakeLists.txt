cmake_minimum_required(VERSION 3.20.0)

set(PROJECT_NAME "toy2decomp")

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(${PROJECT_NAME} C CXX RC)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Tests/Patcher)
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

# Toy2 Sources
set(TOY2_SOURCES
    src/Toy2/Toy2.cpp
    src/Toy2/Toy2.h

    src/Logger.cpp
    src/Logger.h

    src/FileUtils.h
    src/FileUtils.cpp

    src/Common.h
    src/Numerics.h
    src/Numerics.cpp

    src/DrawingDevice.h
    src/DrawingDevice.cpp

    src/InputManager.h
    src/InputManager.cpp

    src/Toy2/D3DApp.cpp
    src/Toy2/D3DApp.h
    src/Toy2/D3DAppTypes.h

    # Nu3D
    src/Nu3D/Creature.h
    src/Nu3D/Link.h
    src/Nu3D/Link.cpp
    src/Nu3D/Material.h
    src/Nu3D/Nu3D.h
    src/Nu3D/Nu3D.cpp
    src/Nu3D/BmpDataNode.h
    src/Nu3D/BmpDataNode.cpp
    src/Nu3D/Patch.h
    src/Nu3D/Portal.h
    src/Nu3D/Portal.cpp
    src/Nu3D/Primitive.h
    src/Nu3D/Sprite.h
    src/Nu3D/Viewport.h
    src/Nu3D/Font.h
    src/Nu3D/Font.cpp

    # Renderer
    src/Renderer/Renderer.h
    src/Renderer/Renderer.cpp
    src/Renderer/RenderType.h
    src/Renderer/SpriteRenderer.h

    # NGNLoader
    src/NGNLoader/NGNLoader.cpp
    src/NGNLoader/NGNLoader.h
    src/NGNLoader/NGNTypes.h

    # AudioManager
    src/AudioManager/AudioManager.cpp
    src/AudioManager/AudioManager.h
)

# Build the game
add_executable(toy2decomp WIN32 
    ${TOY2_SOURCES}
)

# Build the testing dll(s)
add_library(patcher SHARED
    patcher/main.cpp

    patcher/Loader.cpp
    patcher/Loader.h

    patcher/GamePatches.cpp
    patcher/GamePatches.h

    patcher/Utilities.h

    ${TOY2_SOURCES}
)

target_link_directories(patcher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/libs)
target_link_libraries(patcher PRIVATE minhook_x86 ddraw dxguid)

target_link_directories(toy2decomp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/libs)
target_link_libraries(toy2decomp PRIVATE ddraw dxguid)