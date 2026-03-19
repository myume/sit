cmake_minimum_required(VERSION 3.25)

project(sit)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

add_subdirectory(src)

if (BENCHMARK OR TEST) 
    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        v3.8.1
    )

    FetchContent_MakeAvailable(Catch2)

    enable_testing()
endif()

if (TEST)
    add_subdirectory(tests)
endif()

if (BENCHMARK)
    add_subdirectory(benchmarks)
endif()
