#include "Bitboard.h"
#include "PieceDef.h"
#include "Log.h"
#include <iostream>
#include "MoveNode.h"

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
		m_pinnedDirty[set] = true;
		
		m_materialCombined[set] = 0x0;
		m_attacked[set] = 0x0;
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
	byte corrRank = (byte)(rank - 1);
	byte shift = corrRank * (byte)8 + corrFile;

	return PlacePiece(set, piece, shift);
}


bool Bitboard::PlacePiece(SET set, PIECE piece, byte tSqr)
{
	u64 mask = UINT64_C(1) << tSqr;
	m_material[set][piece] |= mask;
	
	MarkDirty(set);
	return true;
}

bool 
Bitboard::CapturePiece(SET set, PIECE piece, byte tSqr)
{
	u64 mask = UINT64_C(1) << tSqr;

	if((m_material[set][piece] & mask) == 0)
		std::cout << "[    OUTPUT] Bitboard::CapturePiece failed to capture at: " << tSqr << std::endl;
	m_material[set][piece] ^= mask;

	MarkDirty(set);
	return true;
}

bool 
Bitboard::MakeMove(Move move, SET set, PIECE piece)
{
	u64 sMask = UINT64_C(1) << move.fromSqr;
	u64 tMask = UINT64_C(1) << move.toSqr;

	if ((m_material[set][piece] & sMask) == 0)
		std::cout << "[    OUTPUT] Bitboard::MakeMove failed to move " << PieceDef::converter(piece) << ": " << move.toString().c_str() << std::endl;

	m_material[set][piece] ^= sMask;
	m_material[set][piece] |= tMask;

	MarkDirty(set);
	return true;
}

bool 
Bitboard::MakeMove(byte sSqr, SET set, PIECE piece, byte tSqr)
{
	Move move;
	move.fromSqr = sSqr;
	move.toSqr = tSqr;
	return MakeMove(move, set, piece);
}

bool 
Bitboard::Promote(SET set, PIECE toPiece, byte sqr)
{
	u64 mask = UINT64_C(1) << sqr;
	if ((m_material[set][PAWN] & mask) == 0)
		std::cout << "[    OUTPUT] Bitboard::Promote failed to promote pawn at: " << sqr << std::endl;

	m_material[set][PAWN] ^= mask;
	m_material[set][toPiece] |= mask;

	MarkDirty(set);
	return true;
}

bool
Bitboard::Demote(SET set, PIECE fromPiece, byte sqr)
{
	u64 mask = UINT64_C(1) << sqr;
	if ((m_material[set][fromPiece] & mask) == 0)
		std::cout << "[    OUTPUT] Bitboard::Demote failed to undo promotion at: " << sqr << std::endl;

	m_material[set][fromPiece] ^= mask;
	m_material[set][PAWN] |= mask;

	MarkDirty(set);
	return true;
}

u64 
Bitboard::AvailableMoves(SET set, PIECE piece, u32 square, byte enPassant, byte castling, byte& promotion)
{
	u64 matComb = MaterialCombined(set);

	int seti = (int)!set;
	u64 matCombOp = MaterialCombined((SET)seti);

	u64 retVal = ~universe;

	byte startingRank = 6;
	signed short mvMod = 1;
	if (set == WHITE)
	{
		startingRank = 1;
		mvMod = -1; // inverse move if we're White.
	}

	byte curSqr = (byte)square;

	if (piece == PAWN)
	{
		byte sq0x88 = curSqr + (curSqr & (byte)~7);
		byte rank = sq0x88 >> 4;

		short dir = mvMod * PieceDef::Moves0x88((PIECE)piece, 0);
		FATAL_ASSERT(dir < INT8_MAX || dir > INT8_MIN, "dir is out of bounds");
		sq0x88 += (byte)dir;
		byte sq8x8 = (sq0x88 + (sq0x88 & (byte)7)) >> 1;
		u64 sqbb = UINT64_C(1) << sq8x8;

		if (!(matComb & sqbb || matCombOp & sqbb))
		{
			retVal |= sqbb;
			if (rank == startingRank)
			{
				dir = mvMod * PieceDef::Moves0x88((PIECE)piece, 0);
				FATAL_ASSERT(dir < INT8_MAX || dir > INT8_MIN, "dir is out of bounds");
				sq0x88 += (byte)dir;
				sq8x8 = (sq0x88 + (sq0x88 & (byte)7)) >> 1;
				sqbb = UINT64_C(1) << sq8x8;
				if (!(matComb & sqbb || matCombOp & sqbb))
					retVal |= sqbb;
			}
		}
	}

	u64 sqbb = UINT64_C(1) << square;
	if (piece == KING && !(sqbb & Attacked((SET)seti)))
		retVal |= AvailableCastling(set, castling);

	CalculatePotentialPinns(set);

	u64 potentialMoves = AvailableMovesSimple(set, piece, static_cast<byte>(square), static_cast<byte>(mvMod), enPassant);

	if (sqbb & m_pinned[set] && piece != KING)
	{
		retVal |= (potentialMoves & m_pinned[set]);
	}
	else
	{ 
		retVal |= potentialMoves;
	}

	if (piece == PAWN)
	{
		// white promotion
		u64 mask = 0xff00000000000000;
		if (retVal & mask)
			promotion = static_cast<byte>(square);

		// black promotion
		mask = 0xff;
		if (retVal & mask)
			promotion = static_cast<byte>(square);
	}

	return retVal;
}

u64 
Bitboard::AvailableMovesSimple(SET set, PIECE piece, byte square, byte mvMod, byte enPassant)
{
	u64 retVal = ~universe;
	int opSet = !(int)set;
	u64 matCombOp = MaterialCombined((SET)opSet);
	u64 matComb = MaterialCombined(set);

	if(piece == PAWN && enPassant > 0)
	{
		// add en passant sqr to op's material.
		u64 enPass = UINT64_C(1) << enPassant;
		matCombOp |= enPass;
	}
	
	byte curSqr = square;
	for (int pI = 0; pI < PieceDef::MoveCount(piece); pI++)
	{
		curSqr = square;
		bool sliding = PieceDef::Slides(piece);
		signed short dir = mvMod * PieceDef::Attacks0x88(piece, pI);		
		FATAL_ASSERT(dir < INT8_MAX || dir > INT8_MIN, "dir is out of bounds");
		do
		{
			byte sq0x88 = 0x00;
			byte sq8x8 = 0x00;
			sq0x88 = curSqr + (curSqr & (byte)~7);

			sq0x88 += (byte)dir;

			sq8x8 = (sq0x88 + (sq0x88 & (byte)7)) >> (byte)1;
			u64 sqbb = UINT64_C(1) << sq8x8;

			if (sq0x88 & 0x88 || matComb & sqbb)
				sliding = false;
			else if (matCombOp & sqbb)
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

void
Bitboard::CalculatePotentialPinns(SET set)
{
    if(m_pinnedDirty[set] == false)
        return;

    SET opSet = (SET)!(int)set;
	u64 matComb = MaterialCombined(set);
	u64 matCombOp = MaterialCombined(opSet);
	for (int i = 0; i < 64; i++)
	{
		PIECE pc = GetPieceOnSquare(opSet, i);
		if (pc != NR_OF_PIECES)
		{
			
			u32 pieceIndx = pc;
			// are we a sliding piece?
			if (!PieceDef::Slides(pieceIndx))
				continue;

			signed short direction = 1;
			// get attacking direction
			for (int a = 0; a < PieceDef::MoveCount(pieceIndx); a++)
			{
				byte curSqr = (byte)i;
				u64 cachedAttack = 0;
				byte pinnedCounter = 0;
				direction = PieceDef::Attacks0x88(pieceIndx, a);
				bool sliding = true;

				FATAL_ASSERT(direction < INT8_MAX || direction > INT8_MIN, "direction is out of bounds");
				do
				{
					byte sq0x88 = 0x00;
					byte sq8x8 = 0x00;
					// convert current square to 0x88 format
					sq0x88 = curSqr + (curSqr & (byte)~7);

					sq0x88 += (byte)direction;

					// convert back to sq8x8
					sq8x8 = (sq0x88 + (sq0x88 & (byte)7)) >> (byte)1;
					u64 sqbb = UINT64_C(1) << sq8x8;

					bool validSqr = !(sq0x88 & 0x88);
					if (validSqr)
					{
						if (matCombOp & sqbb) // this is a bit confusing, but this is actually checking whether or not the pin is being blocked by same set pieces.
						{
							sliding = false;
						}
                        else if (matComb & sqbb) 
						/* if we find a piece that is attacked we cache the square for later pin toggle 
						though if we find two pieces in the same sliding direction we're not pinned */
                        {
							pinnedCounter += 1; 
							cachedAttack |= sqbb;

							if (pinnedCounter > 1) // if we run into more then one piece before the king, we're by definition not pinned.
								sliding = false;

                            if(GetPieceOnSquare(set, sq8x8) == KING)
                            {
								// minor hack to add origin square to attack
								// this is so that when we & pinned with possible moves capture is still possible
								cachedAttack |= UINT64_C(1) << i;
                                m_pinned[set] |= cachedAttack;
                                sliding = false;
                            }
                        }
						else {
							cachedAttack |= sqbb;
						}
					}
					else
						sliding = false;

					curSqr = sq8x8;

				} while (sliding);
			}
		}
	}

	m_pinnedDirty[set] = false;
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

	for (int a = 0; a < PieceDef::MoveCount(piece); a++)
	{
		signed short direction = 1;
		if (piece == PAWN && set == WHITE)
			direction = -1; // inverse attack if we're black.
		
		// get attacking direction
		direction *= PieceDef::Attacks0x88(piece, a);
		bool sliding = PieceDef::Slides(piece);
		auto curSqr = (byte)square;

		FATAL_ASSERT(direction < INT8_MAX || direction > INT8_MIN, "direction is out of bounds");
		do 
		{
			byte sq0x88 = 0x00;
			byte sq8x8 = 0x00;
			// convert current square to 0x88 format
			sq0x88 = curSqr + (curSqr & (byte)~7);
			
			sq0x88 += (byte)direction;

			// convert back to sq8x8
			sq8x8 = (sq0x88 + (sq0x88 & (byte)7)) >> (byte)1;
			u64 sqbb = UINT64_C(1) << sq8x8;
			
			bool validSqr = !(sq0x88 & 0x88);
			if (validSqr)
			{
				if(matComb & sqbb)
				{
					sliding = false;				
					m_attacked[set] |= sqbb;
					m_perPieceAttack[set][piece] |= sqbb;
				}			
				else if (matCombedOp & sqbb)
				{
					sliding = false;
					m_attacked[set] |= sqbb;
					m_perPieceAttack[set][piece] |= sqbb;
				}
				else {
					m_attacked[set] |= sqbb;
					m_perPieceAttack[set][piece] |= sqbb;
				}
			}
			else
				sliding = false;

			curSqr = sq8x8;

		} while (sliding);			
	}
}

u64 
Bitboard::AvailableCastling(SET set, byte castling)
{
	u64 retVal = ~universe;
	byte state = 1;
	byte rankCalc = 0;
	if (set == BLACK)
	{
		rankCalc = 7;
		state = 4;
	}

	rankCalc *= 8;

	SET opSet = (SET)!(int)set;
	u64 atked = Attacked(opSet);
	u64 combMat = MaterialCombined(set);
	combMat |= MaterialCombined(opSet);

	// king side
	byte file = 'g' - 'a';
	byte toCheck = file;
	bool available = true;
	u64 castlingSqrs = ~universe;
	castlingSqrs |= (INT64_C(1) << (file + rankCalc));
	castlingSqrs |= (INT64_C(1) << ((file-1) + rankCalc));

	if (castling & state)
	{
		available = !(atked & castlingSqrs);
		while (available)
		{
			u64 sqr = INT64_C(1) << (toCheck + rankCalc);
			if (sqr & combMat)
				available = false;
			toCheck--;

			if (toCheck == ('e' - 'a'))
				break;
		}
	
		if(available)
			retVal |= INT64_C(1) << (file + rankCalc);
	}

	// queen side
	state *= 2;
	file = 'c' - 'a';
	toCheck = 'b' - 'a';
	castlingSqrs = ~universe;
	castlingSqrs |= (INT64_C(1) << (file + rankCalc));
	castlingSqrs |= (INT64_C(1) << ((file+1) + rankCalc));
	if (castling & state)
	{
		available = !(atked & castlingSqrs);
		while (available)
		{
			u64 sqr = INT64_C(1) << (toCheck + rankCalc);
			if (sqr & combMat)
				available = false;
			toCheck++;

			if (toCheck == ('e' - 'a'))
				break;
		}

		if (available)
			retVal |= INT64_C(1) << (file + rankCalc);
	}

	return retVal;
}

void 
Bitboard::MarkDirty(SET set)
{
	m_materialCombined[set] = ~universe;
	m_combMaterialDirty[set] = true;
	m_pinnedDirty[set] = true;
	m_pinned[set] = ~universe;

	m_attacked[WHITE] = ~universe;
	m_attackedDirty[WHITE] = true;
	m_attacked[BLACK] = ~universe;
	m_attackedDirty[BLACK] = true;
}

void Bitboard::Clear()
{
    // TODO: use MarkDirty method or some othger generic method, avoid duplication.
	for (int s = 0; s < NR_OF_SETS; s++)
		for (int p = 0; p < NR_OF_PIECES; p++)
		{
			m_material[s][p] = ~universe;
			m_perPieceAttack[s][p] = ~universe;
		}

	for (int set = 0; set < NR_OF_SETS; set++)
	{
		m_materialCombined[set]  = ~universe;
		m_attacked[set]			 = ~universe;
        m_pinned[set]			 = ~universe;
		m_combMaterialDirty[set] = true;
		m_attackedDirty[set]	 = true;
		m_pinnedDirty[set]       = true;
	}

	for (byte val = 0; val < 128; val++)
	{
		m_board0x88[val] = val;
	}
}