#pragma once
#include "typedef.h"
#include "Board.h"

namespace GambitEngine
{

class GameState
{
friend class FENParser;

public:
	GameState();
	~GameState();

	void Setup(char* fen, int length);

private: 
	Board m_currentBoard;

	// half moves
	// fifty moves counter since pawn moved
	// or capture been made
	int m_plyCounter;	
	int m_moveCounter;
	SET m_activeSet;
};

}

