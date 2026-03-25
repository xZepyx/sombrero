cmake_minimum_required(VERSION 3.16)

project(sombrero LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOMBRERO_SRC CONFIGURE_DEPENDS
    src/*.cpp
)

file(GLOB_RECURSE SOMBRERO_TEST CONFIGURE_DEPENDS
    testing/*.cpp
)

add_library(sombrero STATIC ${SOMBRERO_SRC})

target_include_directories(sombrero
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PIPEWIRE REQUIRED IMPORTED_TARGET libpipewire-0.3)

target_link_libraries(sombrero PRIVATE PkgConfig::PIPEWIRE)

target_compile_options(sombrero PRIVATE -Wall -Wextra -Wpedantic)

add_executable(sombrero_test ${SOMBRERO_TEST})

target_link_libraries(sombrero_test PRIVATE sombrero)