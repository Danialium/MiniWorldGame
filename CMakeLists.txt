# ========================== root/CMakeLists.txt ==========================

cmake_minimum_required(VERSION 3.20)

project(MiniWorldGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL3 for whole project
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/external/SDL3-3.2.26/cmake")
find_package(SDL3 REQUIRED)

add_subdirectory(src)
