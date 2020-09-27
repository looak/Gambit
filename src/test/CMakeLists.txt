# Gambit Chess Engine - a Chess AI
# Copyright (C) 2019  Alexander Loodin Ek

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

message(STATUS "Enabling testing")
set(PROJECT_NAME GambitEngineTest)

enable_testing()

set(GTEST_LIBRARY ${PROJECT_SOURCE_DIR}/src/test/lib/release/gtest.lib)
set(GTEST_MAIN_LIBRARY ${PROJECT_SOURCE_DIR}/src/test/lib/release/gtest_main.lib)

set(GTEST_LIBRARY_DEBUG ${PROJECT_SOURCE_DIR}/src/test/lib/debug/gtestd.lib)
set(GTEST_MAIN_LIBRARY_DEBUG ${PROJECT_SOURCE_DIR}/src/test/lib/debug/gtest_maind.lib)
set(GTEST_BOTH_LIBRARIES_DEBUG ${GTEST_LIBRARY_DEBUG} ${GTEST_MAIN_LIBRARY_DEBUG})

set(GTEST_INCLUDE_DIR lib/include)
find_package(Threads)
find_package(GTest)

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

include_directories(../engine/inc)
include_directories(lib/include)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(LINK_LIBRARIES_TEST GambitEngine ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
set(LINK_LIBRARIES_TEST_DEBUG GambitEngine ${GTEST_BOTH_LIBRARIES_DEBUG} ${CMAKE_THREAD_LIBS_INIT})

# add code  coverage argument to link arguments
if (CODE_COVERAGE)
    set(LINK_LIBRARIES_TEST ${LINK_LIBRARIES_TEST} --coverage)
    set(LINK_LIBRARIES_TEST_DEBUG ${LINK_LIBRARIES_TEST_DEBUG} --coverage)
endif()

target_link_libraries(${PROJECT_NAME} optimized ${LINK_LIBRARIES_TEST})
target_link_libraries(${PROJECT_NAME} debug ${LINK_LIBRARIES_TEST_DEBUG})

gtest_discover_tests(${PROJECT_NAME})

if(INSTALL_APP)
    install(TARGETS ${PROJECT_NAME} DESTINATION CMAKE_INSTALL_BINDIR)
endif()