add_executable(tests 
  image_tests.cpp
)

target_link_libraries(tests PRIVATE libsit)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_test(NAME tests COMMAND tests)
