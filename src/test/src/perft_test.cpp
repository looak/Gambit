#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "../../engine/src/Board.h"
#include "../../engine/src/GambitEngine.h"
#include "../../engine/src/MoveGenerator.h"
////////////////////////////////////////////////////////////////
using namespace GambitEngine;

namespace GambitTest {

////////////////////////////////////////////////////////////////
class PerftFixture : public ::testing::Test
{
public:
};
////////////////////////////////////////////////////////////////
TEST_F(PerftFixture, InitialPosition)
{
    GambitEngine::Board board;
    char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

    u32 count = 0;
    GambitEngine::MoveGenerator movGen;
    movGen.getMoves(WHITE, &board, count);
    EXPECT_EQ(count, 20); // should be twenty different available moves from the initial board as white.
}
/* Position Five Expected Results
 * Depth Nodes
 * 1     44
 * 2     1,486
 * 3     62,379
 * 4     2,103,487
 * 5     89,941,194
 * */
TEST_F(PerftFixture, PositionFive)
{
    GambitEngine::Board board;
    char inputFen[] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";

    GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

    u32 count = 0;
    GambitEngine::MoveGenerator movGen;
    movGen.getMoves(WHITE, &board, count);
    EXPECT_EQ(count, 44); // should be twenty different available moves from the initial board as white.
}
/*
TEST_F(PerftFixture, PositionFive_Depth_Three)
{
    GambitEngine::Board board;
    char inputFen[] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";

    GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

    u32 count = 0;
    GambitEngine::MoveGenerator movGen;
    auto mvs = movGen.getMoves(WHITE, &board, count);
    EXPECT_EQ(count, 44);

    short depth = 0;
    //while(depth < 2)
    {
        for(int i = 0; i < mvs.size(); i ++)
        {
            auto move = mvs[i];
            board.MakeMove(move.fromSqr, move.toSqr, move.promotion);
            movGen.getMoves(BLACK, &board, count);
            board.UnmakeMove();
        }
        EXPECT_EQ(count, 1486);

    }
}*/
////////////////////////////////////////////////////////////////
}
