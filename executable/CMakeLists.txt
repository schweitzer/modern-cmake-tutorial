cmake_minimum_required(VERSION 3.5)
# Project Name Version & main Languages.
project(sampleExec VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the B Component of ModernCMake.
find_package(ModernCMake QUIET COMPONENTS ModernCMake::B)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ModernCMake::B)