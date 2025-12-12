# ========================== src/CMakeLists.txt ==========================

add_subdirectory(utils)
add_subdirectory(modules)
add_subdirectory(engine)

add_executable(MiniWorld main.cpp)

target_include_directories(MiniWorld PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MiniWorld
    PRIVATE
        mwg_utils
        mwg_modules
        mwg_engine
        SDL3::SDL3
)