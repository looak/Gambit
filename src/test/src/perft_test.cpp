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
    void GenerateMoves(Board& board, SET set, int depth, u32& count)
    {
        auto mvs = mv.getMoves(set, &board, count);

		if (depth > 0)
		{
			for (unsigned int i = 0; i < mvs.size(); i++)
			{
				auto move = mvs[i];
				board.MakeMove(move.fromSqr, move.toSqr, move.promotion);

				GenerateMoves(board, (SET)!(int)set, --depth, count);
				board.UnmakeMove();
			}
		}
	}

    MoveGenerator mv;
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
	GenerateMoves(board, WHITE, 0, count);
	EXPECT_EQ(44, count);
}

TEST_F(PerftFixture, PositionFive_DepthTwo)
{
	GambitEngine::Board board;
	char inputFen[] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

	u32 count = 0;
	GenerateMoves(board, WHITE, 1, count);
	EXPECT_EQ(1486, count);
}
/*
Position Two Expected Results
depth	nodes			totalnodes
1		48				48
2		2,039			2,087
3		97,862			99,949
4		4,085,603		4,185,552
5		193,690,690		19,78,76,242
6		8,031,647,685	8,229,523,927
*/
TEST_F(PerftFixture, PositionTwo)
{
	GambitEngine::Board board;
	char inputFen[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

	u32 count = 0;
	GenerateMoves(board, WHITE, 0, count);
	EXPECT_EQ(48, count);
}

TEST_F(PerftFixture, PositionTwo_DepthTwo)
{
	GambitEngine::Board board;
	char inputFen[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

	u32 count = 0;
	GenerateMoves(board, WHITE, 1, count);
	EXPECT_EQ(2087, count);
}

/*
depth	nodes	totalnodes
1	24	24
2	496	520
3	9483	10003
4	182838	192841
5	3605103	3797944
6	71179139	74977083

*/
TEST_F(PerftFixture, PositionThree)
{
	GambitEngine::Board board;
	char inputFen[] = "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

	u32 count = 0;
	GenerateMoves(board, WHITE, 0, count);
	EXPECT_EQ(24, count);
}

TEST_F(PerftFixture, PositionThree_DepthTwo)
{
	GambitEngine::Board board;
	char inputFen[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

	u32 count = 0;
	GenerateMoves(board, WHITE, 1, count);
	EXPECT_EQ(496, count);
}

/*
depth	nodes	totalnodes
1	20	20
2	400	420
3	8902	9322
4	197281	206603
5	4865609	5072212
6	119060324	124132536
7	3195901860	3320034396
*/

TEST_F(PerftFixture, PositionBase)
{
    GambitEngine::Board board;
    char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

    u32 count = 0;
    GambitEngine::MoveGenerator movGen;
    auto mvs = movGen.getMoves(WHITE, &board, count);
    EXPECT_EQ(count, 20);
	count = 0;

    for (unsigned int i = 0; i < mvs.size(); i ++)
    {
        auto move = mvs[i];
        board.MakeMove(move.fromSqr, move.toSqr, move.promotion);
        movGen.getMoves(BLACK, &board, count);

        board.UnmakeMove();
    }
    EXPECT_EQ(count, 400);
}

TEST_F(PerftFixture, PositionBase_DepthTwo)
{
    GambitEngine::Board board;
    char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

    u32 count = 0;

    GenerateMoves(board, WHITE, 1, count);

    EXPECT_EQ(420, count);
}

TEST_F(PerftFixture, PositionBase_DepthThree)
{
    GambitEngine::Board board;
    char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

    u32 count = 0;

    GenerateMoves(board, WHITE, 2, count);

    EXPECT_EQ(9322, count);
}

////////////////////////////////////////////////////////////////
}
