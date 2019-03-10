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
{/*
	auto moves = getMoves(set, board, count);

	for (auto mv : moves)
	{
		Board testBoard = *board;
		testBoard.MakeLegalMove(mv.fromSqr, mv.toSqr, mv.promotion);
		
		if (depth > 0)
			FindBestMove(set, &testBoard, count, depth--);
	}*/
}

std::vector<Move>
MoveGenerator::getMoves(SET set, Board* board, u32& count, bool ignoreLegality)
{
	std::vector<Move> moves;
	//count = 0;
	int debugInd = 0;
	auto pieceArry = board->GetPieces(set);

	SET opSet = (SET)((set + 1) % SET::NR_OF_SETS);
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
				if (!board->MakeMove(piece->Square8x8, sqr, prom))
				{
					std::cout << "[    OUTPUT] MoveGen::MakeMove failed at index = " << debugInd << std::endl;
					break;
				}

				// populate Move flags
				auto mvNode = board->GetLastMove();
				if (mvNode->getCapturedPiece() != 0x0)
					newMove.flags |= 1;
				if (board->Check(opSet))
					newMove.flags |= 64;
				if (board->CheckMate(opSet))
					newMove.flags |= 128;					
				if (mvNode->getState() & MoveFlags::CASTLING)
					newMove.flags |= 8;
				//if(mvNode->getParent() != nullptr)
				{
					if (mvNode->getEnPassantState() & 128)
						newMove.flags |= 4;
				}

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
						newMove.flags |= 2;

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
void 
MoveGenerator::CountMoves(const std::vector<Move> moves, MoveGenerator::Counter& out) const
{
	for (size_t i = 0; i < moves.size(); i++)
	{			
		if (moves[i].flags & 1)
			out.Captures++;				
		if (moves[i].flags & 2)
			out.Promotions++;		
		if (moves[i].flags & 4)
			out.EnPassants++;
		if (moves[i].flags & 8)
			out.Castles++;		
		if (moves[i].flags & 64)
			out.Checks++;
		if (moves[i].flags & 128)
			out.CheckMates++;
	}
	
	out.Moves = moves.size();
}

int 
MoveGenerator::countPromotions(const std::vector<Move> moves) const
{
	Counter count;
	CountMoves(moves, count);
	return count.Promotions;
}
int 
MoveGenerator::countCaptures(const std::vector<Move> moves) const
{
	Counter count;
	CountMoves(moves, count);
	return count.Captures;
}
int 
MoveGenerator::countCastles(const std::vector<Move> moves) const
{
	Counter count;
	CountMoves(moves, count);
	return count.Castles;
}

int 
MoveGenerator::countChecks(const std::vector<Move> moves) const
{
	Counter count;
	CountMoves(moves, count);
	return count.Checks;
}