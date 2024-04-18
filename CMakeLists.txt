cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Statuses VERSION 1.0.0)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)
add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
)
target_compile_definitions(${PROJECT_NAME} PUBLIC 
    -DWIN32_LEAN_AND_MEAN=1 -DNOMINMAX=1 -D_WINSOCKAPI_=1 -DPSAPI_VERSION=1 
)
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME} EXTERNALS geode.node-ids:1.8.1)
