#pragma once
#include "typedef.h"
#include "Board.h"

namespace GambitEngine
{

class GameState
{
public:
	GameState();
	~GameState();

	void Setup();

private: 
	Board m_currentBoard;

	// half moves
	// fifty moves counter since pawn moved
	// or capture been made
	int m_plyCounter;	
	int m_moveCounter;
	byte m_activeSet;
};

}

