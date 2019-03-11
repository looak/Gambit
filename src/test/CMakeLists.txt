cmake_minimum_required(VERSION 3.2)
message(STATUS "Enabling testing")
enable_testing()

set(PROJECT_NAME GambitEngineTest)
set(SOURCE_FILES
        main.cpp
        src/bitboard_test.cpp
        src/board_test.cpp
        src/fen_test.cpp
        src/perft_test.cpp
        src/movegenerator_test.cpp 
		src/unmake_test.cpp
		src/materialgrid_test.cpp
		src/notation_test.cpp)

project(${PROJECT_NAME})

add_subdirectory(lib/googletest)

include_directories(lib/googletest/googletest/include)
include_directories(../engine/inc)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} GambitEngine gtest gtest_main)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

add_test(GambitEngineTest bin/${PROJECT_NAME})