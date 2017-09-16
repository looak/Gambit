#pragma once

#include "typedef.h"
#include "Pieces.h"

namespace GambitEngine
{

class Board
{
public:
	Board();
	~Board();

	// attempts to place said piece of said set in the given file & rank.
	// returns false if this failed either because of being outside of board 
	// or square is occupied
	bool PlacePiece(SET set, PIECE piece, byte file, byte rank);

private:
	// resets board to be empty;
	void ResetBoard();

	byte m_board[120];
	byte m_boardLookup[64];


};

}
