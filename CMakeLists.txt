cmake_minimum_required(VERSION 3.30)
project(Wash)

set(CMAKE_CXX_STANDARD 20)

#add_compile_definitions(_XOPEN_SOURCE=700)
find_package(PkgConfig REQUIRED)
pkg_check_modules(NOTCURSES REQUIRED IMPORTED_TARGET notcurses++ notcurses-core)


add_executable(Wash
        main.cpp
        src/utils/constants.h
        src/utils/options.h
)

target_link_libraries(Wash
        PkgConfig::NOTCURSES
        notcurses++
)


