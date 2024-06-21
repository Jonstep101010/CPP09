# CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(PmergeMe)

# Set the C++ standard to C++98
set(CMAKE_CXX_STANDARD 98)

# Set the C++ compiler
set(CMAKE_CXX_COMPILER clang++)

# Add the executable target
file(GLOB sources main.cpp *.cpp *.hpp)
add_executable(PmergeMe "")
target_sources(PmergeMe PRIVATE ${sources})
set_target_properties(PmergeMe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=c++98 -fno-limit-debug-info")
# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()