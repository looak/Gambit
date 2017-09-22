#include "stdafx.h"
#include "Bitboard.h"

using namespace GambitEngine;


Bitboard::Bitboard()
{
	for (int s = 0; s < NR_OF_SETS; s++)
		for (int p = 0; p < NR_OF_PIECES; p++)
			m_material[s][p] = ~universe;


	for (int set = 0; set < NR_OF_SETS; set++)
	{
		m_materialCombined[set] = ~universe;
		m_attacked[set]			= ~universe;
		m_dirty[set]			= true;
	}

	for (int val = 0; val < 128; val++)
	{
		m_board0x88[val] = val;
	}
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

	m_dirty[set] = true;

	return true;
}

uint64 
Bitboard::MaterialCombined(SET set)
{
	if (m_dirty[set])
	{
		uint64 combined = ~universe;
		for (int i = NR_OF_PIECES - 1; i > 0; i--)
		{
			combined |= m_material[set][i];
		}
		m_materialCombined[set] = combined;
		m_dirty[set] = false;
	}

	return m_materialCombined[set];
}

uint64 
Bitboard::Attacked(SET set)
{
	CalculateAttacked(set);
	return m_attacked[set];
}

void 
Bitboard::CalculateAttacked(SET set)
{
	for (int i = 0; i < 64; i++)
	{
		PIECE pc = getPieceOnSquare(set, i);
		if (pc != NR_OF_PIECES)
		{
			AddAttackedFrom(set, pc, i);
		}
	}
}

PIECE 
Bitboard::getPieceOnSquare(SET set, int square)
{
	uint64 sqr = 1i64 << square;
	for (int i = 1; i < NR_OF_PIECES; i++)
	{
		if (m_material[set][i] & sqr)
			return (PIECE)i;
	}

	return NR_OF_PIECES;
}

void 
Bitboard::AddAttackedFrom(SET set, PIECE piece, int square)
{
	uint64 m_matComb = MaterialCombined(set);

	//if (piece == KING)
	{
		for (int a = 0; a < Pieces::MoveCount[piece]; a++)
		{
			int toCheck = square + Pieces::Attacks[piece][a];
			byte sq0x88 = toCheck + (toCheck & ~7);
			uint64 sq8x8 = 1i64 << toCheck;

			if(sq0x88 & 0x88 || m_matComb & sq8x8)
				continue;

			m_attacked[set] |= sq8x8;			
		}
	}

}
