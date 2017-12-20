#include "Bitboard.h"
#include "BitMath.h"

using namespace GambitEngine;


Bitboard::Bitboard()
{
	Clear();
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

	m_material[set][piece] |= INT64_C(1) << shift;

	m_materialCombined[set] = ~universe;
	m_combMaterialDirty[set] = true;
	m_attackedDirty[set] = true;
	return true;
}

bool 
Bitboard::CapturePiece(SET set, PIECE piece, byte tSqr)
{
	u64 mask = INT64_C(1) << tSqr;
	m_material[set][piece] ^= mask;

	MarkDirty(set);
	return true;
}

bool 
Bitboard::MakeMove(byte sSqr, SET set, PIECE piece, byte tSqr)
{
	u64 sMask = INT64_C(1) << sSqr;
	u64 tMask = INT64_C(1) << tSqr;

	m_material[set][piece] ^= sMask;
	m_material[set][piece] |= tMask;

	MarkDirty(set);
	return true;
}

bool 
Bitboard::Promote(SET set, PIECE toPiece, byte sqr)
{
	u64 mask = INT64_C(1) << sqr;
	m_material[set][PAWN] ^= mask;
	m_material[set][toPiece] |= mask;

	MarkDirty(set);
	return true;
}

u64 
Bitboard::AvailableMoves(SET set, PIECE piece, u32 square, byte enPassant, byte castling, byte& promotion)
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
		u64 sqbb = INT64_C(1) << sq8x8;

		if (!(m_matComb & sqbb || m_matCombOp & sqbb))
		{
			retVal |= sqbb;
			if (rank == startingRank)
			{
				sq0x88 += (mvMod * Pieces::Moves0x88[piece][0]);
				sq8x8 = (sq0x88 + (sq0x88 & 7)) >> 1;
				sqbb = INT64_C(1) << sq8x8;
				if (!(m_matComb & sqbb || m_matCombOp & sqbb))
					retVal |= sqbb;
			}
		}

		// add en passant to op's material.
		u64 enPass = INT64_C(1) << enPassant;
		m_matCombOp |= enPass;
	}

	u64 sqbb = INT64_C(1) << square;
	if (piece == KING && !(sqbb & Attacked((SET)seti)))
		retVal |= AvailableCastling(set, castling);
	
	for (int a = 0; a < Pieces::MoveCount[piece]; a++)
	{
		curSqr = square;

		bool sliding = Pieces::Slides[piece];
		signed short dir = (mvMod * Pieces::Attacks0x88[piece][a]);
		do
		{
			byte sq0x88 = 0x00;
			byte sq8x8 = 0x00;
			sq0x88 = curSqr + (curSqr & ~7);

			sq0x88 += dir;

			sq8x8 = (sq0x88 + (sq0x88 & 7)) >> 1;
			u64 sqbb = INT64_C(1) << sq8x8;

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

	if (piece == PAWN)
	{
		// white promotion
		u64 mask = 0xff00000000000000;
		if (retVal & mask)
			promotion = square;

		// black promotion
		mask = 0xff;
		if (retVal & mask)
			promotion = square;
	}

	return retVal;
}

u64 
Bitboard::AvailableMovesSimple(SET set, PIECE piece, byte square)
{
	u64 retVal = ~universe;
	int opSet = !(int)set;
	u64 matCombOp = MaterialCombined((SET)opSet);
	u64 matComb = MaterialCombined(set);
	
	byte curSqr = square;
	for (int pI = 2; pI < Pieces::MoveCount[piece]; pI++)
	{
		curSqr = square;
		bool sliding = Pieces::Slides[piece];
		signed short dir = Pieces::Attacks0x88[piece][pI];
		do
		{
			byte sq0x88 = 0x00;
			byte sq8x8 = 0x00;
			sq0x88 = curSqr + (curSqr & ~7);

			sq0x88 += dir;

			sq8x8 = (sq0x88 + (sq0x88 & 7)) >> 1;
			u64 sqbb = INT64_C(1) << sq8x8;

			if (sq0x88 & 0x88 || matComb & sqbb)
			{
				sliding = false;
				continue;
			}
			else if (matCombOp & sqbb)
			{
				retVal |= sqbb;
				sliding = false;
			}
			retVal |= sqbb;
		
			curSqr = sq8x8;

		} while (sliding);
	}

	return retVal;
}

u64 
Bitboard::MatCombinedNoKing(SET set)
{
	u64 combined = ~universe;
	for (int i = KING - 1; i > 0; i--)
	{
		combined |= m_material[set][i];
	}

	return combined;
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
Bitboard::Attacked(SET set, bool ignoreKing)
{
	if (m_attackedDirty[set] == true || ignoreKing)
	{
		m_attacked[set] = ~universe;
		CalculateAttacked(set, ignoreKing);

		// don't want to save state if we ignore king.
		m_attackedDirty[set] = ignoreKing;
	}
	
	return m_attacked[set];
}

bool 
Bitboard::IsSquareAttacked(SET opSet, byte sqr)
{
	u64 sqrMask = INT64_C(1) << sqr;
	u64 atked = Attacked(opSet);

	return (atked & sqrMask);
}

void
Bitboard::CalculateAttacked(SET set, bool ignoreKing)
{
	SET setOp = (SET)!(int)set;
	u64 matCombOp = ~universe;
	if (ignoreKing)
		matCombOp = MatCombinedNoKing(setOp);
	else
		matCombOp = MaterialCombined(setOp);

	for (int i = 0; i < 64; i++)
	{
		PIECE pc = GetPieceOnSquare(set, i);
		if (pc != NR_OF_PIECES)
		{
			AddAttackedFrom(set, pc, i, matCombOp);
		}
	}
}

PIECE 
Bitboard::GetPieceOnSquare(SET set, int square)
{
	u64 sqr = INT64_C(1) << square;
	for (int i = 1; i < NR_OF_PIECES; i++)
	{
		if (m_material[set][i] & sqr)
			return (PIECE)i;
	}

	return NR_OF_PIECES;
}

void 
Bitboard::AddAttackedFrom(SET set, PIECE piece, int square, u64 matCombedOp)
{
	u64 matComb = MaterialCombined(set);

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
			u64 sqbb = INT64_C(1) << sq8x8;
			
			if (sq0x88 & 0x88 || matComb & sqbb)
				sliding = false;
			else if (matCombedOp & sqbb)
			{
				sliding = false;
				m_attacked[set] |= sqbb;
			}
			else
				m_attacked[set] |= sqbb;

			curSqr = sq8x8;

		} while (sliding);			
	}
}

u64 
Bitboard::AvailableCastling(SET set, byte castling)
{
	u64 retVal = ~universe;
	byte state = 1;
	byte rank = 0;
	if (set == BLACK)
	{
		rank = 7;
		state = 4;
	}
	
	SET opSet = (SET)!(int)set;
	u64 atked = Attacked(opSet);
	u64 combMat = MaterialCombined(set);
	combMat |= MaterialCombined(opSet);
	combMat |= atked;

	// king side
	byte file = 'g' - 'a';
	byte toCheck = file;
	bool available = true;
	if (castling & state)
	{
		available = !(atked & (INT64_C(1) << (('h' - 'a') + (rank * 8))));
		while (available)
		{
			u64 sqr = INT64_C(1) << (toCheck + (rank * 8));
			if (sqr & combMat)
				available = false;
			toCheck--;

			if (toCheck == ('e' - 'a'))
				break;
		}
	
		if(available)
			retVal |= INT64_C(1) << (file + (rank * 8));
	}

	// queen side
	state *= 2;
	file = 'c' - 'a';
	toCheck = 'b' - 'a';
	if (castling & state)
	{
		available = !(atked & (INT64_C(1) << (('a' - 'a') + (rank * 8))));
		while (available)
		{
			u64 sqr = INT64_C(1) << (toCheck + (rank * 8));
			if (sqr & combMat)
				available = false;
			toCheck++;

			if (toCheck == ('e' - 'a'))
				break;
		}

		if (available)
			retVal |= INT64_C(1) << (file + (rank * 8));
	}

	return retVal;
}

bool 
Bitboard::AvailablePromotion(SET set)
{
	return false;
}

void 
Bitboard::MarkDirty(SET set)
{
	m_materialCombined[set] = ~universe;
	m_combMaterialDirty[set] = true;

	m_attacked[set] = ~universe;
	m_attackedDirty[set] = true;
}

void Bitboard::Clear()
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
