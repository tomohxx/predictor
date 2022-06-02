cmake_minimum_required(VERSION 3.1)

project(predictor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -DCHECK_WALL")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

add_definitions(-DWIN_RDY_FILE_PATH="${PROJECT_BINARY_DIR}")

add_executable(mkind src/mkind.cpp src/judwin.cpp src/judrdy.cpp)
add_executable(test src/test.cpp src/predictor.cpp)
