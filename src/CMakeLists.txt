add_executable(sit
    main.cpp

    image.cpp
    image.h
)

target_link_libraries(sit PRIVATE stb_image)
