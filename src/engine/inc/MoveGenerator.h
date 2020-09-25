#pragma once
#include "typedef.h"
#include "Material.h"
#include "MoveNode.h"
#include <vector>
#include <unordered_map>

namespace GambitEngine
{

class Board;
class MoveNode;

class MoveGenerator
{
public:
	struct Counter
	{
		void operator += (const Counter& rhs)
		{
			this->Captures 		+= rhs.Captures;
			this->Promotions 	+= rhs.Promotions;
			this->EnPassants 	+= rhs.EnPassants;
			this->Castles 		+= rhs.Castles;
			this->Checks 		+= rhs.Checks;
			this->CheckMates 	+= rhs.CheckMates;
			this->Moves 		+= rhs.Moves;
		}
		int Captures = 0;
		int Promotions = 0;
		int EnPassants = 0;
		int Castles = 0;
		int Checks = 0;
		int CheckMates = 0;
		int Moves = 0;
	};
	
	MoveGenerator();
	~MoveGenerator();

	void FindBestMove(SET set, Board* board, u32& count, short depth);
	std::vector<Move> getMoves(SET set, Board* board, u32& count, bool ignoreLegality = false);
	
	/* gives a list of possible moves and a possible move count for given depth */
	typedef std::vector<std::pair<Move, int>> DivisionResult;
	DivisionResult getMovesDivision(SET set, Board* board, int depth);


	int countPromotions(const std::vector<Move>& moves) const;
	int countCaptures(const std::vector<Move>& moves) const;
	int countCastles(const std::vector<Move>& moves) const;
	int countChecks(const std::vector<Move>& moves) const;

	void CountMoves(const std::vector<Move>& moves, Counter& out) const;

private:
	int InnerDivision(SET set, Board* board, int depth);

};

}