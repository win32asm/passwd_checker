cmake_minimum_required(VERSION 3.6)
project(passwd_checker)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(passwd_checker ${SOURCE_FILES})
if (!WIN32)
    target_link_libraries(passwd_checker curses)
endif ()
