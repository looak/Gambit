#pragma once

#include <vector>
#include <string>

#include "Board.h"
#include "typedef.h"

namespace GambitEngine
{

// reference: https://en.wikipedia.org/wiki/Algebraic_notation_(chess)
class Notation
{
public:
	static std::vector<std::string> ConvertMoves(std::vector<Move> moves, const Board& boardRef);
};
}