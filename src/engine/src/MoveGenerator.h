#pragma once
#include "typedef.h"
#include "Material.h"
#include "MoveNode.h"
#include <vector>

namespace GambitEngine
{

class Board;


class MoveGenerator
{
public:
	MoveGenerator();
	~MoveGenerator();

	void FindBestMove(SET set, Board* board, u32& count, short depth);

	std::vector<Move> getMoves(SET set, Board* board, u32& count, bool ignoreLegality = false);
	
};

}

