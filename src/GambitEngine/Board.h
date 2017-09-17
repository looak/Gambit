#pragma once

#ifdef GAMBIT_ENGINE_EXPORTS  
#define GAMBIT_API __declspec(dllexport)   
#else  
#define GAMBIT_API __declspec(dllimport)   
#endif  

#include "typedef.h"
#include "Pieces.h"

//	FF FF FF FF FF FF FF FF FF FF
//	FF FF FF FF FF FF FF FF FF FF
//	FF 04 02 03 05 06 03 02 04 FF
//	FF 01 01 01 01 01 01 01 01 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 81 81 81 81 81 81 81 81 FF
//	FF 84 82 83 85 86 83 82 84 FF
//	FF FF FF FF FF FF FF FF FF FF
//	FF FF FF FF FF FF FF FF FF FF

//	FF FF FF FF FF FF FF FF FF FF
//	FF FF FF FF FF FF FF FF FF FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF 00 00 00 00 00 00 00 00 FF
//	FF FF FF FF FF FF FF FF FF FF
//	FF FF FF FF FF FF FF FF FF FF

namespace GambitEngine
{

class GAMBIT_API Board
{
public:
	Board();
	Board(const Board& _src);

	~Board();

	// attempts to place said piece of said set in the given file & rank.
	// returns false if this failed either because of being outside of board 
	// or square is occupied
	bool PlacePiece(SET set, PIECE piece, byte file, byte rank);
	bool MovePiece(byte sFile, byte sRank, byte tFile, byte tRank);

	byte GetValue(byte file, byte rank) const;

private:
	// resets board to be empty;
	void ResetBoard();

	byte GetBoardIndex(byte file, byte rank) const;

	byte m_board[120];
	byte m_boardLookup[64];
};

}
