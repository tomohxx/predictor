cmake_minimum_required(VERSION 3.1)

project(predictor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -DCHECK_WALL")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
set(CMAKE_INSTALL_DATADIR share/data)

add_library(predictor STATIC src/predictor.cpp)
set_target_properties(predictor PROPERTIES PUBLIC_HEADER src/predictor.hpp)
install(TARGETS predictor EXPORT Predictor)
install(FILES
  ${PROJECT_BINARY_DIR}/rdy_h_2_1.txt
  ${PROJECT_BINARY_DIR}/rdy_h_3_1.txt
  ${PROJECT_BINARY_DIR}/rdy_h_6_1.txt
  ${PROJECT_BINARY_DIR}/rdy_s_2_1.txt
  ${PROJECT_BINARY_DIR}/rdy_s_3_1.txt
  ${PROJECT_BINARY_DIR}/rdy_s_3_2.txt
  ${PROJECT_BINARY_DIR}/rdy_s_6_1.txt
  ${PROJECT_BINARY_DIR}/rdy_s_6_5.txt
  ${PROJECT_BINARY_DIR}/win_h_2_0.txt
  ${PROJECT_BINARY_DIR}/win_h_3_0.txt
  ${PROJECT_BINARY_DIR}/win_h_6_0.txt
  ${PROJECT_BINARY_DIR}/win_h_6_2.txt
  ${PROJECT_BINARY_DIR}/win_s_2_0.txt
  ${PROJECT_BINARY_DIR}/win_s_3_0.txt
  ${PROJECT_BINARY_DIR}/win_s_3_2.txt
  ${PROJECT_BINARY_DIR}/win_s_6_0.txt
  ${PROJECT_BINARY_DIR}/win_s_6_2.txt
TYPE DATA)
install(EXPORT Predictor FILE PredictorConfig.cmake DESTINATION share/cmake)

add_executable(mkind src/mkind.cpp src/judwin.cpp src/judrdy.cpp)
add_executable(test src/test.cpp)
target_compile_definitions(test PUBLIC WIN_RDY_FILE_PATH="${PROJECT_SOURCE_DIR}/data")
target_link_libraries(test predictor)
add_custom_target(extract ALL ${CMAKE_COMMAND} -E
  tar zxvf ${PROJECT_SOURCE_DIR}/data.tar.gz
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
