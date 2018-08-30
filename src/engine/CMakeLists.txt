cmake_minimum_required(VERSION 3.2)

set(LIBRARY_NAME GambitEngine)
set(PROJECT_NAME GambitEngine)
project(${PROJECT_NAME} C CXX)

set(GambitEngine_SRC
        src/Bitboard.cpp
        src/GambitEngine.cpp
        src/MoveGenerator.cpp
        src/GameState.cpp
        src/FENParser.cpp
        src/Material.cpp
        src/Board.cpp
		src/PieceDef.cpp)

set(GambitEngine_INC
        src/Bitboard.h
        src/BitMath.h
        src/Board.h
        src/FENParser.h
        src/GambitEngine.h
        src/GameState.h
        src/MoveGenerator.h
        src/Material.h
        src/typedef.h 
		src/MoveNode.h 
		src/PieceDef.h
		src/Log.h)

#include_directories(${GambitEngine_INCLUDE})

add_library(${LIBRARY_NAME} SHARED STATIC ${GambitEngine_SRC} ${GambitEngine_INC})
install(TARGETS ${LIBRARY_NAME} DESTINATION lib)