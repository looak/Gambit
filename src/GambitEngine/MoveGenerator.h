#pragma once
#include "typedef.h"
#include "Pieces.h"
#include <vector>

namespace GambitEngine
{

class Board;

struct GAMBIT_API Move
{
	byte fromSqr;
	byte toSqr;
};

class GAMBIT_API MoveGenerator
{
public:
	MoveGenerator();
	~MoveGenerator();

	std::vector<Move> getMoves(SET set, const Board* board, u32& count);
	
};

}

