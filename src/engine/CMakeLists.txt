cmake_minimum_required(VERSION 3.2)

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
		inc/Log.h)

source_group("src" FILES ${GambitEngine_SRC})
source_group("inc" FILES ${GambitEngine_INC})
include_directories(inc)

add_library(${LIBRARY_NAME} SHARED STATIC ${GambitEngine_SRC} ${GambitEngine_INC})
install(TARGETS ${LIBRARY_NAME} DESTINATION lib)