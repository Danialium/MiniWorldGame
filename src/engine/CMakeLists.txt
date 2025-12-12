# ========================== src/engine/CMakeLists.txt ==========================

add_subdirectory(core)

add_library(mwg_engine INTERFACE)

target_link_libraries(mwg_engine
    INTERFACE
        mwg_engine_core
)