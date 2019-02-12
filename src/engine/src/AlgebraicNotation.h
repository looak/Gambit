#pragma once

#include <vector>
#include <string>

#include "Board.h"
#include "typedef.h"

namespace GambitEngine
{

struct Move;

// reference: https://en.wikipedia.org/wiki/Algebraic_notation_(chess)
class Notation
{
public:
	static void ConvertMove(Move& move);
	static std::vector<std::string> ConvertMoves(std::vector<Move> moves, const Board& boardRef);
private:
	static void sqrToNotation(byte sqr, char* out);

};
}