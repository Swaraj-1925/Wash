cmake_minimum_required(VERSION 3.30)
project(Wash)

set(CMAKE_CXX_STANDARD 20)

#add_compile_definitions(_XOPEN_SOURCE=700)
find_package(PkgConfig REQUIRED)
pkg_check_modules(NOTCURSES REQUIRED IMPORTED_TARGET notcurses++ notcurses-core)

include_directories(
        include
)

file(GLOB SRC_FILES src/*.cpp)

add_executable(Wash
        main.cpp
        ${SRC_FILES}
)

target_link_libraries(Wash
        PkgConfig::NOTCURSES
        notcurses++
)


