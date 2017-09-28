#pragma once
#include "typedef.h"
#include "Pieces.h"

namespace GambitEngine
{

class GAMBIT_API Bitboard
{
private:
	void CalculateAttacked(SET set = NR_OF_SETS);
	PIECE getPieceOnSquare(SET set, int square);
	void AddAttackedFrom(SET set, PIECE piece, int square);

	bool m_dirty[NR_OF_SETS];

	u64 m_material[NR_OF_SETS][NR_OF_PIECES];

	u64 m_materialCombined[NR_OF_SETS];
	u64 m_attacked[NR_OF_SETS];

	byte m_board0x88[128];

public:
	Bitboard();
	~Bitboard();
		
	bool PlacePiece(SET set, PIECE piece, byte file, byte rank);	
	bool MakeMove(SET set, PIECE piece, byte file, byte rank);

	u64 AvailableMoves(SET set, PIECE piece, u32 square);

	u64 MaterialCombined(SET set);

	u64 Attacked(SET set);

};
}

