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
enable_testing()

include(GoogleTest)
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
gtest_discover_tests(${PROJECT_NAME})