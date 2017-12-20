cmake_minimum_required(VERSION 3.2)

set(PROJECT_NAME GambitEngineTest)
set(SOURCE_FILES main.cpp src/bitboard_test.cpp)

project(${PROJECT_NAME})

add_subdirectory(lib/googletest)

include_directories(lib/googletest/googletest/include)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} GambitEngine gtest gtest_main)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

message(STATUS "Enabling testing")
include(CTest)
enable_testing()
add_test(UnitTests ${PROJECT_BINARY_DIR}\${PROJECT_NAME})
