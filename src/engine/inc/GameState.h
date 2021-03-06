#pragma once
#include "typedef.h"
#include "Board.h"

namespace GambitEngine
{

class GameState
{
friend class FENParser;

public:
	GameState(Board&);
	~GameState() = default;

	bool Setup(const std::string& fen);

	Board& getBoard() { return m_currentBoard; }
	const Board& getBoard() const { return m_currentBoard; }
	bool MakeMove(byte sFile, byte sRank, byte tFile, byte tRank, byte promotion = 0x00);
	
	/* Set to move */
	SET getActiveSet() const { return m_activeSet; }
	int getPly() const { return m_plyCounter; }
	int getMoveCounter() const { return m_moveCounter; }

private: 
	Board& m_currentBoard;

	// half moves
	// fifty moves counter since pawn moved
	// or capture been made
	int m_plyCounter;	
	int m_moveCounter;
	SET m_activeSet;
};

}

