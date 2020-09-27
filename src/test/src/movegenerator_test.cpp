#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "Board.h"
#include "GambitEngine.h"
#include "MoveGenerator.h"
////////////////////////////////////////////////////////////////
using namespace GambitEngine;

namespace GambitTest {

////////////////////////////////////////////////////////////////
class MoveGeneratorFixture : public ::testing::Test
{
protected:
    GambitEngine::Board board;
    GambitEngine::MoveGenerator movGen;

    u32 count = 0;
};
////////////////////////////////////////////////////////////////
TEST_F(MoveGeneratorFixture, Pawns_and_Knight_Available_Moves)
{
    board.PlacePiece(WHITE, PAWN, 'c', 3);
    board.PlacePiece(WHITE, PAWN, 'd', 2);

    movGen.getMoves(WHITE, &board, count, true);
    EXPECT_EQ(count, 3);

    count = 0;
    board.PlacePiece(WHITE, KNIGHT, 'c', 1);
    movGen.getMoves(WHITE, &board, count, true);
    EXPECT_EQ(count, 7);
}

TEST_F(MoveGeneratorFixture, Castling)
{
    board.SetCastlingRights(0x04); // all available

    EXPECT_EQ(true, board.PlacePiece(BLACK, KING, 'e', 8));
    EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'h', 8));

    auto mvs = movGen.getMoves(BLACK, &board, count, true); 
    EXPECT_EQ(count, 15);
    EXPECT_EQ(movGen.countCastles(mvs), 1);
}

TEST_F(MoveGeneratorFixture, Promotion)
{
	EXPECT_EQ(true, board.PlacePiece(WHITE, PAWN, 'd', 7));
	
	auto mvs = movGen.getMoves(WHITE, &board, count, true);
	EXPECT_EQ(count, 4);
    EXPECT_EQ(movGen.countPromotions(mvs), 4);

	EXPECT_EQ(true, board.PlacePiece(BLACK, BISHOP, 'c', 8));
	count = 0;
	mvs = movGen.getMoves(WHITE, &board, count, true);
	EXPECT_EQ(count, 8);
    EXPECT_EQ(movGen.countPromotions(mvs), 8);    
    EXPECT_EQ(movGen.countCaptures(mvs), 4);
}

TEST_F(MoveGeneratorFixture, MovePawnIntoChess)
{
    char inputFen[] = "8/8/8/8/K4p1k/8/6P1/8 w - -";
    
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);
    
    auto mvs = movGen.getMoves(WHITE, &board, count, false);
    EXPECT_EQ(count, 7);
    EXPECT_EQ(movGen.countChecks(mvs), 1);
}

TEST_F(MoveGeneratorFixture, EnPassant)
{
    char inputFen[] = "4k3/8/8/8/1p6/8/P7/4K3 w - - 0 1";    
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);
    board.MakeMove('a',2,'a',4);
    
    auto mvs = movGen.getMoves(BLACK, &board, count, false);
    EXPECT_EQ(count, 7);
    MoveGenerator::Counter counter;
    movGen.CountMoves(mvs, counter);
    EXPECT_EQ(counter.Captures, 1);
    EXPECT_EQ(counter.EnPassants, 1);
}

TEST_F(MoveGeneratorFixture, EnPassant_Horse_GetMoves)
{
	char inputFen[] = "4k3/8/8/8/1p6/8/PN6/4K3 w - -";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);
	auto mvs = movGen.getMoves(WHITE, &board, count, false);
	EXPECT_EQ(count, 11);

	board.MakeMove('a', 2, 'a', 4);

	count = 0;
	mvs = movGen.getMoves(BLACK, &board, count, false);
	EXPECT_EQ(count, 7);
	MoveGenerator::Counter counter;
	movGen.CountMoves(mvs, counter);
	EXPECT_EQ(counter.Captures, 1);
	EXPECT_EQ(counter.EnPassants, 1);
}

TEST_F(MoveGeneratorFixture, EnPassant_GetMoves)
{
	char inputFen[] = "4k3/8/8/8/1p6/8/PN6/4K3 w - -";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);
	auto mvs = movGen.getMoves(WHITE, &board, count, false);
	EXPECT_EQ(count, 11);

	for (unsigned int i = 0; i < mvs.size(); i++)
	{
		auto move = mvs[i];
		board.MakeMove(move.fromSqr, move.toSqr, move.promotion);

		u32 scndCount = 0;
		movGen.getMoves(BLACK, &board, scndCount);
		board.UnmakeMove();
	}
}

TEST_F(MoveGeneratorFixture, Castling_Moves)
{
	char inputFen[] = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq -";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);
	auto mvs = movGen.getMoves(WHITE, &board, count, false);
	
	MoveGenerator::Counter counter;
	movGen.CountMoves(mvs, counter);

	EXPECT_EQ(count, 26);
	EXPECT_EQ(counter.Checks, 2);
	EXPECT_EQ(counter.Castles, 2);
	EXPECT_EQ(counter.Captures, 2);

	for (unsigned int i = 0; i < mvs.size(); i++)
	{
		auto move = mvs[i];
		board.MakeMove(move.fromSqr, move.toSqr, move.promotion);

		u32 scndCount = 0;
		movGen.getMoves(BLACK, &board, scndCount);
		board.UnmakeMove();
	}
}

TEST_F(MoveGeneratorFixture, BishopKnightEndGame)
{
    char inputFen[] = "8/3k4/8/8/8/8/5p2/3K1Nb1 w - - 0 1";
    
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);
    
    auto mvs = movGen.getMoves(WHITE, &board, count, false);
    EXPECT_EQ(count, 8);
}

}

