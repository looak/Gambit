#include "stdafx.h"
#include "MoveGenerator.h"
#include "Board.h"

using namespace GambitEngine;

MoveGenerator::MoveGenerator()
{
}

MoveGenerator::~MoveGenerator()
{
}

std::vector<Move>
MoveGenerator::getMoves(SET set, Board* board, u32& count)
{
	std::vector<Move> moves;
	count = 0;
	auto pieceArry = board->GetPieces(set);
	for (int i = 0; i < pieceArry.size(); i++)
	{
		auto piece = pieceArry.at(i);
		u64 avaMvs = board->AvailableMoves(set, (PIECE)piece.Type, piece.Square8x8);

		byte sqr = 0;
		while (avaMvs != ~universe)
		{
			u64 mask = 1i64 << sqr;
			if (mask & avaMvs)
			{
				Move newMove;
				newMove.fromSqr = piece.Square8x8;
				newMove.toSqr	= sqr;
				moves.push_back(newMove);

				avaMvs ^= mask;
			}

			sqr++;
		}		
	}

	return moves;	
}


