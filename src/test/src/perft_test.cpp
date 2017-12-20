#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "../../engine/src/Board.h"
#include "../../engine/src/GambitEngine.h"
#include "../../engine/src/MoveGenerator.h"
////////////////////////////////////////////////////////////////

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
////////////////////////////////////////////////////////////////
}