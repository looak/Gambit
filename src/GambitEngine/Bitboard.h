#pragma once
#include "typedef.h"
#include "Pieces.h"

namespace GambitEngine
{

class GAMBIT_API Bitboard
{
private:
	void CalculateAttacked(SET set = NR_OF_SETS);
	PIECE GetPieceOnSquare(SET set, int square);
	void AddAttackedFrom(SET set, PIECE piece, int square);

	void MarkDirty(SET set);

	bool m_combMaterialDirty[NR_OF_SETS];
	bool m_attackedDirty[NR_OF_SETS];

	u64 m_material[NR_OF_SETS][NR_OF_PIECES];

	u64 m_materialCombined[NR_OF_SETS];
	u64 m_attacked[NR_OF_SETS];

	byte m_board0x88[128];

public:
	Bitboard();
	Bitboard(const Bitboard& _src);

	~Bitboard();
		
	bool PlacePiece(SET set, PIECE piece, byte file, byte rank);	
	bool CapturePiece(SET set, PIECE piece, byte tSqr);
	bool MakeMove(byte sSqr, SET set, PIECE piece, byte tSqr);

	u64 AvailableMoves(SET set, PIECE piece, u32 square, byte enPassant);

	u64 MaterialCombined(SET set);

	u64 Attacked(SET set);

};
}

