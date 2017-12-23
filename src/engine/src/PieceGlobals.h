#pragma once

#include "Pieces.h"

class PieceDefs
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