#include "PieceDef.h"
#include <sstream>
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

byte PieceDef::printable(byte toConvert)
{
	byte piece = toConvert & 7;	
	switch (piece)
	{
	case PAWN:
		piece = 'p';
		break;
	case KNIGHT:
		piece = 'n';
		break;
	case BISHOP:
		piece = 'b';
		break;
	case ROOK:
		piece = 'r';
		break;
	case QUEEN:
		piece = 'q';
		break;
	case KING:
		piece = 'k';
		break;
	}

	if ((toConvert & 128) == 0)
	{
		piece = (byte)toupper(piece);
	}

	return piece;
}


byte PieceDef::printable(SET toConvert)
{
	byte result = 0x0;
	switch (toConvert)
	{
	case WHITE:
		result = 'w';
		break;
	case BLACK:
		result = 'b';
		break;
	}
	return result;
}

////////////////////////////////////////////////////////////////
