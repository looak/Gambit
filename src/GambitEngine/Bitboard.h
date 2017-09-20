#pragma once
#include "typedef.h"
#include "Pieces.h"

namespace GambitEngine
{

class GAMBIT_API Bitboard
{
public:
	Bitboard();
	~Bitboard();
	
	bool PlacePiece(SET set, PIECE piece, byte file, byte rank);

	uint64 MaterialCombined(SET set);

private:
	uint64 m_material[NR_OF_SETS][NR_OF_PIECES];
};
}

