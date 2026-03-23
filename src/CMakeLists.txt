FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(stb)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

FetchContent_Declare(
    cli11_proj
    QUIET
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.6.2
)

FetchContent_MakeAvailable(cli11_proj)

add_executable(sit main.cpp)
add_library(libsit STATIC
    image.cpp
    image.h
)

target_compile_options(libsit PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(sit PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(sit PRIVATE libsit)
target_link_libraries(sit PRIVATE CLI11::CLI11)

target_link_libraries(libsit PUBLIC stb_image)

set_target_properties(libsit PROPERTIES PREFIX "")

target_include_directories(libsit SYSTEM PUBLIC ${stb_SOURCE_DIR})
target_include_directories(libsit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
