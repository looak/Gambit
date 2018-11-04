#include "MoveGenerator.h"
#include "Board.h"
#include "PieceDef.h"
#include <algorithm>
#include <iostream>

using namespace GambitEngine;

MoveGenerator::MoveGenerator()
{
}

MoveGenerator::~MoveGenerator()
{
}

void 
MoveGenerator::FindBestMove(SET set, Board* board, u32& count, short depth)
{
	auto moves = getMoves(set, board, count);

	for (auto mv : moves)
	{
		Board testBoard = *board;
		testBoard.MakeLegalMove(mv.fromSqr, mv.toSqr, mv.promotion);
		
		if (depth > 0)
			FindBestMove(set, &testBoard, count, depth--);
	}
}

std::vector<Move>
MoveGenerator::getMoves(SET set, Board* board, u32& count, bool ignoreLegality)
{
	std::vector<Move> moves;
	//count = 0;
	int debugInd = 0;
	auto pieceArry = board->GetPieces(set);
	for (unsigned int i = 0; i < pieceArry.size(); i++)
	{
		auto piece = pieceArry.at(i);
		byte promotion = 0x00;
		u64 avaMvs = board->AvailableMoves(set, (PIECE)piece->Type, piece->Square8x8, promotion);

		byte sqr = 0;
		while (avaMvs != ~universe)
		{
			u64 mask = UINT64_C(1) << sqr;
			if (mask & avaMvs)
			{
				Move newMove;
				newMove.fromSqr = piece->Square8x8;
				newMove.toSqr = sqr;

				byte prom = 0;
				if (promotion != 0x00)
					prom = PieceDef::converter(QUEEN);
				if(!board->MakeMove(piece->Square8x8, sqr, prom))
					std::cout << "[    OUTPUT] MoveGen::MakeMove failed at index = " << debugInd << std::endl;

				debugInd++;
				if (!ignoreLegality && !board->Legal(set))
				{
					board->UnmakeMove();
					sqr++;
					avaMvs ^= mask;
					continue;
				}

				if (promotion != 0x00)
				{
					for (int i = 2; i < KING; i++)
					{
						newMove.promotion = PieceDef::converter((PIECE)i);

						moves.push_back(newMove);
						count++;
					}
				}
				else
				{
					moves.push_back(newMove);
					count++;
				}

				board->UnmakeMove();
				avaMvs ^= mask;
			}

			sqr++;
		}
	}

	return moves;	
}


