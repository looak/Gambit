#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "../../engine/src/Board.h"
#include "../../engine/src/GambitEngine.h"
#include "../../engine/src/MoveGenerator.h"
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

    movGen.getMoves(BLACK, &board, count, true);
    EXPECT_EQ(count, 15);
}

TEST_F(MoveGeneratorFixture, Promotion)
{
	EXPECT_EQ(true, board.PlacePiece(WHITE, PAWN, 'd', 7));
	
	auto mvs = movGen.getMoves(WHITE, &board, count, true);
	EXPECT_EQ(count, 4);

	EXPECT_EQ(true, board.PlacePiece(BLACK, BISHOP, 'c', 8));
	count = 0;
	mvs = movGen.getMoves(WHITE, &board, count, true);
	EXPECT_EQ(count, 8);
}
}

