# ========================== src/modules/CMakeLists.txt ==========================

add_subdirectory(Animation)
add_subdirectory(Movement)
add_subdirectory(Object)

add_library(mwg_modules INTERFACE)

target_link_libraries(mwg_modules
    INTERFACE
        mwg_animation
        mwg_movement
        mwg_object
)