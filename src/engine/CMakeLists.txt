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

set(LIBRARY_NAME GambitEngine)
set(PROJECT_NAME GambitEngine)
project(${PROJECT_NAME} C CXX)

set(GambitEngine_SRC
		src/AlgebraicNotation.cpp
        src/Bitboard.cpp
        src/GambitEngine.cpp
        src/MoveGenerator.cpp
        src/GameState.cpp
        src/FENParser.cpp
        src/Material.cpp
        src/Board.cpp
		src/PieceDef.cpp)

set(GambitEngine_INC
		inc/AlgebraicNotation.h
        inc/Bitboard.h
        inc/BitMath.h
        inc/Board.h
        inc/FENParser.h
        inc/GambitEngine.h
        inc/GameState.h
        inc/MoveGenerator.h
        inc/Material.h
        inc/typedef.h 
		inc/MoveNode.h 
		inc/PieceDef.h
		inc/Log.h
		inc/version.h)

source_group("src" FILES ${GambitEngine_SRC})
source_group("inc" FILES ${GambitEngine_INC})
include_directories(inc)


add_library(${LIBRARY_NAME} SHARED STATIC ${GambitEngine_SRC} ${GambitEngine_INC})

set(LINK_LIBRARIES_TEST)
if (CODE_COVERAGE)
    set(LINK_LIBRARIES_TEST ${LINK_LIBRARIES_TEST} --coverage)
endif()
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES_TEST})

install(TARGETS ${LIBRARY_NAME} DESTINATION lib)