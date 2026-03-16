Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests 
  image.cpp
)
target_link_libraries(tests PRIVATE core)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_test(NAME tests COMMAND tests)
