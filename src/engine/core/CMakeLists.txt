add_subdirectory(WindowManager)

add_library(mwg_engine_core INTERFACE)

target_link_libraries(mwg_engine_core
    INTERFACE mwg_window_manager
)