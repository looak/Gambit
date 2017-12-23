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

    movGen.getMoves(WHITE, &board, count);
    EXPECT_EQ(count, 3);

    board.PlacePiece(WHITE, KNIGHT, 'c', 1);
    movGen.getMoves(WHITE, &board, count);
    EXPECT_EQ(count, 7);
}

TEST_F(MoveGeneratorFixture, Castling)
{
    board.SetCastlingRights(0x04); // all available

    EXPECT_EQ(true, board.PlacePiece(BLACK, KING, 'e', 8));
    EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'h', 8));

    movGen.getMoves(BLACK, &board, count);
    EXPECT_EQ(count, 15);
}
}

