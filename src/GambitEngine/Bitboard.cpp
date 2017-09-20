#include "stdafx.h"
#include "Bitboard.h"

using namespace GambitEngine;


Bitboard::Bitboard()
{
	for (int s = 0; s < NR_OF_SETS; s++)
		for (int p = 0; p < NR_OF_PIECES; p++)
			m_material[s][p] = ~universe;
}


Bitboard::~Bitboard()
{
}

bool 
Bitboard::PlacePiece(SET set, PIECE piece, byte file, byte rank)
{
	byte corrFile = file - 96;
	byte corrRank = rank - 1;
	byte shift = corrFile + (corrRank * 8);
	m_material[set][piece] = 1i64 << shift;

	return true;
}

uint64 
Bitboard::MaterialCombined(SET set)
{
	uint64 combined = ~universe; 
	for (int i = NR_OF_PIECES -1; i > 0; i --)
	{
		combined |= m_material[set][i];
	}

	return combined;
}
