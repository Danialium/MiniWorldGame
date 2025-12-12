add_library(mwg_window_manager STATIC
    WindowManager.cpp
    WindowManager.h
)

target_include_directories(mwg_window_manager PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mwg_window_manager
    PUBLIC
        mwg_utils
        mwg_modules
        SDL3::SDL3
)