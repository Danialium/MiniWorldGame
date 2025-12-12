add_library(mwg_animation STATIC
    Animation.cpp
    Animation.h
)

target_include_directories(mwg_animation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/tomlplusplus/include
)

target_link_libraries(mwg_animation PUBLIC mwg_utils)