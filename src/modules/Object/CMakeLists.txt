add_library(mwg_object STATIC
    Object.cpp
    Object.h
)

target_include_directories(mwg_object PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Animation

)

target_link_libraries(mwg_object PUBLIC mwg_utils mwg_animation)