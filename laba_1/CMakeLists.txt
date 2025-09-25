cmake_minimum_required(VERSION 3.10)
project(MyProject C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES
    main.c
    function.c
)

add_executable(Laba ${SOURCES})

target_include_directories(Laba PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})