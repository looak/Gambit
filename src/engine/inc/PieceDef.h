#pragma once

#include "typedef.h"
////////////////////////////////////////////////////////////////

namespace GambitEngine
{
////////////////////////////////////////////////////////////////

enum PIECE : byte
{
	PAWN = 1,
	KNIGHT = 2,
	BISHOP = 3,
	ROOK = 4,
	QUEEN = 5,
	KING = 6,
	NR_OF_PIECES =7,
};

enum SET : byte
{
	WHITE = 0,
	BLACK = 1,
	NR_OF_SETS = 2,
};
////////////////////////////////////////////////////////////////

class PieceDef
{
public:
	static signed short 	MoveCount(unsigned int pIndex);
	static bool 			Slides(unsigned int pIndex);
	static signed short 	Moves0x88(unsigned int pIndex, unsigned int mIndex);
	static signed short 	Attacks0x88(unsigned int pIndex, unsigned int mIndex);

	static byte				converter(PIECE toConvert);
	static PIECE			converter(byte toConvert);

private:
	static signed short m_moveCount[NR_OF_PIECES];
	static bool m_slides[NR_OF_PIECES];
	static signed short m_moves0x88[NR_OF_PIECES][8];
	static signed short m_attacks0x88[NR_OF_PIECES][8];
};

////////////////////////////////////////////////////////////////

}