cmake_minimum_required(VERSION 3.30)
project(Wash)

set(CMAKE_CXX_STANDARD 20)

#add_compile_definitions(_XOPEN_SOURCE=700)
find_package(PkgConfig REQUIRED)
pkg_check_modules(NOTCURSES REQUIRED IMPORTED_TARGET notcurses++ notcurses-core)

include_directories(
        include
        src/utils
)
add_executable(Wash
        main.cpp
        src/shell.cpp
        src/status_line.cpp
)

target_link_libraries(Wash
        PkgConfig::NOTCURSES
        notcurses++
)


