#include "PieceDef.h"
////////////////////////////////////////////////////////////////

using namespace GambitEngine;
////////////////////////////////////////////////////////////////

signed short PieceDef::m_moveCount[NR_OF_PIECES] = {
		0, 2, 8, 4, 4, 8, 8
};

bool PieceDef::m_slides[NR_OF_PIECES] = {
		false, false, false, true, true, true, false,
};

signed short PieceDef::m_moves0x88[NR_OF_PIECES][8] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ -16, -32, 0, 0, 0, 0, 0, 0 },
		{ -33, -31, -18, -14, 14, 18, 31, 33 },
		{ -17, -15, 15, 17, 0, 0, 0, 0 },
		{ -16, -1, 1, 16, 0, 0, 0, 0 },
		{ -17, -16, -15, -1, 1, 15, 16, 17 },
		{ -17, -16, -15, -1, 1, 15, 16, 17 }
};


signed short PieceDef::m_attacks0x88[NR_OF_PIECES][8] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ -15, -17, 0, 0, 0, 0, 0, 0 },
		{ -33, -31, -18, -14, 14, 18, 31, 33 },
		{ -17, -15, 15, 17, 0, 0, 0, 0 },
		{ -16, -1, 1, 16, 0, 0, 0, 0 },
		{ -17, -16, -15, -1, 1, 15, 16, 17 },
		{ -17, -16, -15, -1, 1, 15, 16, 17 }
};

////////////////////////////////////////////////////////////////

signed short PieceDef::MoveCount(unsigned int pIndex)
{
	return m_moveCount[pIndex];
}

bool PieceDef::Slides(unsigned int pIndex)
{
	return m_slides[pIndex];
}

signed short PieceDef::Moves0x88(unsigned int pIndex, unsigned int mIndex)
{
	return m_moves0x88[pIndex][mIndex];
}

signed short PieceDef::Attacks0x88(unsigned int pIndex, unsigned int mIndex)
{
	return m_attacks0x88[pIndex][mIndex];
}


PIECE PieceDef::converter(byte toConvert)
{
	switch (toConvert)
	{
		case 'p':
			return PAWN;
		case 'n':
			return KNIGHT;
		case 'b':
			return BISHOP;
		case 'r':
			return ROOK;
		case 'q':
			return QUEEN;
		case 'k':
			return KING;
		default:
			return PAWN;
	}
}

byte PieceDef::converter(PIECE toConvert)
{
	switch (toConvert)
	{
		case PAWN:
			return 'p';
		case KNIGHT:
			return 'n';
		case BISHOP:
			return 'b';
		case ROOK:
			return 'r';
		case QUEEN:
			return 'q';
		case KING:
			return 'k';
		default:
			return PAWN;
	}
}

////////////////////////////////////////////////////////////////
