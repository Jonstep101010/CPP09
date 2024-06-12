# CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(CPP09)

set(CLANG_TIDY_COMMAND "/usr/bin/clang-tidy -config=.clang-tidy")
set(CMAKE_CXX_STANDARD 98)

add_subdirectory(ex00)