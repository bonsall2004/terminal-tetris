cmake_minimum_required(VERSION 3.10)
project(multilayer_perceptron)

if(WIN32)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries/win)
elseif(UNIX AND NOT APPLE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries/linux)
elseif(APPLE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries/macos)
else()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries/unknown)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../includes)

set(SOURCES main.cpp
    Tetris/Tetris.cpp
    )

add_executable(tetris ${SOURCES})

if(UNIX)
  find_package(Threads REQUIRED)
  target_link_libraries(tetris PRIVATE ${CMAKE_THREAD_LIBS_INIT})
endif()