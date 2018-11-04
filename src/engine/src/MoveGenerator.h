#pragma once
#include "typedef.h"
#include "Material.h"
#include "MoveNode.h"
#include <vector>

namespace GambitEngine
{

class Board;
class MoveNode;

class MoveGenerator
{
public:
	struct Counter
	{
		int Captures = 0;
		int Promotions = 0;
		int EnPassants = 0;
		int Castles = 0;
		int Checks = 0;
		int CheckMates = 0;
	};
	
	MoveGenerator();
	~MoveGenerator();

	void FindBestMove(SET set, Board* board, u32& count, short depth);
	std::vector<Move> getMoves(SET set, Board* board, u32& count, bool ignoreLegality = false);

	int countPromotions(const std::vector<Move> moves) const;
	int countCaptures(const std::vector<Move> moves) const;
	int countCastles(const std::vector<Move> moves) const;
	int countChecks(const std::vector<Move> moves) const;

	void CountMoves(const std::vector<Move> moves, Counter& out) const;
};

}