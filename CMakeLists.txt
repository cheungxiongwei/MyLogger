cmake_minimum_required(VERSION 3.23)
project(MyLogger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(MyLogger include/logger/logger.h src/logger/logger.cpp include/logger/source_location.hpp)

target_include_directories(MyLogger PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(MyLoggerTest main.cpp)

target_include_directories(MyLoggerTest PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(MyLoggerTest MyLogger)