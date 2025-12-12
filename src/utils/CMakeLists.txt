# ========================== src/utils/CMakeLists.txt ==========================
add_library(mwg_utils STATIC
    Constant.cpp
    Constant.h
)

target_include_directories(mwg_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mwg_utils PUBLIC SDL3::SDL3)