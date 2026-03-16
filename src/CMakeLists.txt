add_executable(sit main.cpp)

add_library(core STATIC
    image.cpp
    image.h
)

target_link_libraries(sit PRIVATE core)
target_link_libraries(sit PRIVATE CLI11::CLI11)

target_link_libraries(core PUBLIC stb_image)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
