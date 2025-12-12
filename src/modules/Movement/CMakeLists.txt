add_library(mwg_movement STATIC
    Movement.cpp
    Movement.h
)

target_include_directories(mwg_movement PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mwg_movement PUBLIC mwg_utils)