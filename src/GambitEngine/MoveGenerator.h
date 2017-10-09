#pragma once
#include "typedef.h"
#include "Pieces.h"
#include <vector>

namespace GambitEngine
{

class Board;

struct GAMBIT_API Move
{
	byte fromSqr = 0x00;
	byte toSqr = 0x00;

	byte promotion = 0x00;
};

class GAMBIT_API MoveGenerator
{
public:
	MoveGenerator();
	~MoveGenerator();

	std::vector<Move> getMoves(SET set, Board* board, u32& count);
	
};

}

