#include "stdafx.h"
#include "Bitboard.h"
#include "BitMath.h"

using namespace GambitEngine;


Bitboard::Bitboard()
{
	for (int s = 0; s < NR_OF_SETS; s++)
		for (int p = 0; p < NR_OF_PIECES; p++)
			m_material[s][p] = ~universe;


	for (int set = 0; set < NR_OF_SETS; set++)
	{
		m_materialCombined[set]  = ~universe;
		m_attacked[set]			 = ~universe;
		m_combMaterialDirty[set] = true;
		m_attackedDirty[set]	 = true;
	}

	for (int val = 0; val < 128; val++)
	{
		m_board0x88[val] = val;
	}
}

Bitboard::Bitboard(const Bitboard & _src)
{
	for (int s = 0; s < NR_OF_SETS; s++)
		for (int p = 0; p < NR_OF_PIECES; p++)
			m_material[s][p] = _src.m_material[s][p];


	for (int set = 0; set < NR_OF_SETS; set++)
	{
		m_combMaterialDirty[set] = true;
		m_attackedDirty[set] = true;
	}

	for (int val = 0; val < 128; val++)
	{
		m_board0x88[val] = _src.m_board0x88[val];
	}
}


Bitboard::~Bitboard()
{
}

bool 
Bitboard::PlacePiece(SET set, PIECE piece, byte file, byte rank)
{
	byte corrFile = file - 'a';
	byte corrRank = rank - 1;
	byte shift = corrRank * 8 + corrFile;

	m_material[set][piece] |= 1i64 << shift;

	m_materialCombined[set] = ~universe;
	m_combMaterialDirty[set] = true;
	m_attackedDirty[set] = true;
	return true;
}

bool 
Bitboard::MakeMove(byte sSqr, SET set, PIECE piece, byte tSqr)
{
	u64 sMask = 1i64 << sSqr;
	u64 tMask = 1i64 << tSqr;

	m_material[set][piece] ^= sMask;
	m_material[set][piece] |= tMask;

	m_materialCombined[set] = ~universe;
	m_combMaterialDirty[set] = true;
	m_attackedDirty[set] = true;
	return true;
}

u64 
Bitboard::AvailableMoves(SET set, PIECE piece, u32 square)
{
	u64 m_matComb = MaterialCombined(set);	
	int seti = (int)!set;
	u64 m_matCombOp = MaterialCombined((SET)seti);

	u64 retVal = ~universe;

	byte startingRank = 6;
	signed short mvMod = 1;
	if (set == WHITE)
	{
		startingRank = 1;
		mvMod = -1; // inverse move if we're White.
	}

	byte curSqr = square;

	if (piece == PAWN)
	{
		byte sq0x88 = curSqr + (curSqr & ~7);
		byte rank = sq0x88 >> 4;

		sq0x88 += (mvMod * Pieces::Moves0x88[piece][0]);
		byte sq8x8 = (sq0x88 + (sq0x88 & 7)) >> 1;
		u64 sqbb = 1i64 << sq8x8;

		if (!(m_matComb & sqbb || m_matCombOp & sqbb))
		{
			retVal |= sqbb;

			if (rank == startingRank)
			{
				sq0x88 += (mvMod * Pieces::Moves0x88[piece][0]);
				sq8x8 = (sq0x88 + (sq0x88 & 7)) >> 1;
				sqbb = 1i64 << sq8x8;
				if (!(m_matComb & sqbb || m_matCombOp & sqbb))
					retVal |= sqbb;
			}
		}
	}
	
	for (int a = 0; a < Pieces::MoveCount[piece]; a++)
	{
		bool sliding = Pieces::Slides[piece];
		signed short dir = (mvMod * Pieces::Attacks0x88[piece][a]);
		do
		{
			byte sq0x88 = 0x00;
			byte sq8x8 = 0x00;
			sq0x88 = curSqr + (curSqr & ~7);

			sq0x88 += dir;

			sq8x8 = (sq0x88 + (sq0x88 & 7)) >> 1;
			u64 sqbb = 1i64 << sq8x8;

			if (sq0x88 & 0x88 || m_matComb & sqbb)
				sliding = false;
			else if (m_matCombOp & sqbb)
			{
				retVal |= sqbb;
				sliding = false;
			}
			else if (piece > 1) // if we're nt a pawn.
			{
				retVal |= sqbb;
			}

			curSqr = sq8x8;

		} while (sliding);
	}

	return retVal;
}

u64
Bitboard::MaterialCombined(SET set)
{
	if (m_combMaterialDirty[set])
	{
		u64 combined = ~universe;
		for (int i = NR_OF_PIECES - 1; i > 0; i--)
		{
			combined |= m_material[set][i];
		}
		m_materialCombined[set] = combined;
		m_combMaterialDirty[set] = false;
	}

	return m_materialCombined[set];
}

u64 
Bitboard::Attacked(SET set)
{
	if (m_attackedDirty[set] == true)
	{
		m_attacked[set] = ~universe;
		CalculateAttacked(set);
		m_attackedDirty[set] = false;
	}
	
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
	u64 sqr = 1i64 << square;
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
	u64 m_matComb = MaterialCombined(set);
	int seti = (int)!set;
	u64 m_matCombOp = MaterialCombined((SET)seti);

	for (int a = 0; a < Pieces::MoveCount[piece]; a++)
	{
		signed short atk = 1;
		if (piece == PAWN && set == WHITE)
			atk = -1; // inverse attack if we're black.

		atk *= Pieces::Attacks0x88[piece][a];
		bool sliding = Pieces::Slides[piece];
		byte curSqr = square;		

		do 
		{
			byte sq0x88 = 0x00;
			byte sq8x8 = 0x00;
			sq0x88 = curSqr + (curSqr & ~7);
			
			sq0x88 += atk;

			sq8x8 = (sq0x88 + (sq0x88 & 7)) >> 1;
			u64 sqbb = 1i64 << sq8x8;
			
			if (sq0x88 & 0x88 || m_matComb & sqbb)
				sliding = false;
			else
				m_attacked[set] |= sqbb;

			curSqr = sq8x8;

		} while (sliding);			
	}
}
