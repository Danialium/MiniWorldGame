cmake_minimum_required(VERSION 3.10.0)
set(SOLUTION_NAME "MiniWorldGame")

project(${SOLUTION_NAME} VERSION 0.1.0 LANGUAGES C CXX)

###################################################################

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/external/SDL3-3.2.26/cmake")

find_package(SDL3 REQUIRED)

###################################################################

file(GLOB SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c")

###################################################################

add_executable(${SOLUTION_NAME} ${SOURCES})

###################################################################

set(PROJECT_INCLUDE_DIR 
    "${CMAKE_SOURCE_DIR}/include")

set(PROJECT_LIBRARY_DIR 
    )

set(PROJECT_LIBS 
    SDL3::SDL3)

###################################################################

target_include_directories(${SOLUTION_NAME} PUBLIC 
    ${PROJECT_INCLUDE_DIR})

target_link_directories(${SOLUTION_NAME} PUBLIC 
    ${PROJECT_LIBRARY_DIR})

target_link_libraries(${SOLUTION_NAME} PUBLIC 
    ${PROJECT_LIBS})

###################################################################
###################################################################

message(STATUS "-------------------------------------------------------------------------")
message(STATUS "PROJECT_INCLUDE_DIR : ${PROJECT_INCLUDE_DIR}")
message(STATUS "PROJECT_LIBRARY_DIR : ${PROJECT_LIBRARY_DIR}")
message(STATUS "PROJECT_LIBS : ${PROJECT_LIBS}")
message(STATUS "-------------------------------------------------------------------------")

###################################################################
###################################################################
