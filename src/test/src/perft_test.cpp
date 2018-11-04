#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "../../engine/src/Board.h"
#include "../../engine/src/GambitEngine.h"
#include "../../engine/src/MoveGenerator.h"
////////////////////////////////////////////////////////////////
using namespace GambitEngine;

/*
* Results of these tests can be found here:
* https://www.chessprogramming.org/Perft_Results
*/

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

				GenerateMoves(board, (SET)!(int)set, depth-1, count);
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

/*TEST_F(PerftFixture, PositionBase_DepthFour)
{
    GambitEngine::Board board;
    char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

    u32 count = 0;

    GenerateMoves(board, WHITE, 4, count);

    EXPECT_EQ(206603, count);
}*/

/*
Position Two Expected Results
a.k.a. Kiwipete
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
Depth	Nodes		Captures	E.p.	Castles		Promotions		Checks		Checkmates
1		14			1			0		0			0				2			0
2		191			14			0		0			0				10			0
3		2812		209			2		0			0				267			0
4		43238		3348		123		0			0				1680		17
5		674624		52051		1165	0			0				52950		0
6		11030083	940350		33325	0			7552			452473		2733
7		178633661	14519036	294874	0			140024			12797406	87
*/
TEST_F(PerftFixture, Position_Three)
{
	GambitEngine::Board board;
	char inputFen[] = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
    GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

    u32 count = 0;
    GambitEngine::MoveGenerator movGen;
    auto mvs = movGen.getMoves(WHITE, &board, count);
    EXPECT_EQ(count, 14);
	EXPECT_EQ(movGen.countCaptures(mvs), 1);
	EXPECT_EQ(movGen.countChecks(mvs), 2);
	count = 0;

	MoveGenerator::Counter counter;
    for (unsigned int i = 0; i < mvs.size(); i ++)
    {
        auto move = mvs[i];
        board.MakeMove(move.fromSqr, move.toSqr, move.promotion);
        auto tmpMvs = movGen.getMoves(BLACK, &board, count);
		movGen.CountMoves(tmpMvs, counter);
        board.UnmakeMove();
    }
    EXPECT_EQ(count, 191);
	EXPECT_EQ(counter.Captures, 14);
	EXPECT_EQ(counter.Checks, 10);
}

TEST_F(PerftFixture, Position_Three_Depth_Three)
{
	GambitEngine::Board board;
	char inputFen[] = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

	u32 count = 0;
	GenerateMoves(board, WHITE, 2, count);
	EXPECT_EQ(2812, count);
}

/*
Depth	Nodes		Captures	E.p.	Castles		Promotions	Checks		Checkmates
1		6			0			0		0			0			0			0
2		264			87			0		6			48			10			0
3		9467		1021		4		0			120			38			22
4		422333		131393		0		7795		60032		15492		5
5		15833292	2046173		6512	0			329464		200568		50562
6		706045033	210369132	212		10882006	81102984	26973664	81076
*/
TEST_F(PerftFixture, Position_Four)
{
	GambitEngine::Board board;
	char inputFen[] = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
    GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

    u32 count = 0;
    GambitEngine::MoveGenerator movGen;
    auto mvs = movGen.getMoves(WHITE, &board, count);
    EXPECT_EQ(count, 6);
	count = 0;

	MoveGenerator::Counter counter;
    for (unsigned int i = 0; i < mvs.size(); i ++)
    {
        auto move = mvs[i];
        board.MakeMove(move.fromSqr, move.toSqr, move.promotion);
        auto tmpMvs = movGen.getMoves(BLACK, &board, count);
		movGen.CountMoves(tmpMvs, counter);
        board.UnmakeMove();
    }
    EXPECT_EQ(count, 264);
	EXPECT_EQ(counter.Promotions, 48);
	EXPECT_EQ(counter.Checks, 10);
	//EXPECT_EQ(counter.Castles, 6);
	EXPECT_EQ(counter.Captures, 87);
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
	
	/* 1486 + 44 */
	EXPECT_EQ(1530, count);
}

TEST_F(PerftFixture, PositionFive_DepthThree)
{
	GambitEngine::Board board;
	char inputFen[] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

	u32 count = 0;
	GenerateMoves(board, WHITE, 2, count);
	
	/* 62379 + 1486 + 44 */
	EXPECT_EQ(63809, count);
}

/*
0	1
1	46
2	2,079
3	89,890
4	3,894,594
5	164,075,551
6	6,923,051,137
7	287,188,994,746
8	11,923,589,843,526
9	490,154,852,788,714 */
TEST_F(PerftFixture, Position_Six)
{
	GambitEngine::Board board;
	char inputFen[] = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
    GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

    u32 count = 0;
    GambitEngine::MoveGenerator movGen;
    auto mvs = movGen.getMoves(WHITE, &board, count);
    EXPECT_EQ(count, 46);
	count = 0;

    for (unsigned int i = 0; i < mvs.size(); i ++)
    {
        auto move = mvs[i];
        board.MakeMove(move.fromSqr, move.toSqr, move.promotion);
        movGen.getMoves(BLACK, &board, count);

        board.UnmakeMove();
    }
    EXPECT_EQ(count, 2079);
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
TEST_F(PerftFixture, PositionFifteen)
{
	GambitEngine::Board board;
	char inputFen[] = "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

	u32 count = 0;
	GenerateMoves(board, WHITE, 0, count);
	EXPECT_EQ(24, count);
}

TEST_F(PerftFixture, PositionFifteen_DepthTwo)
{
	GambitEngine::Board board;
	char inputFen[] = "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1";
	GambitEngine::FEN::InputFen(inputFen, sizeof(inputFen), board);

	u32 count = 0;
	GenerateMoves(board, WHITE, 1, count);
	EXPECT_EQ(520, count);
}

////////////////////////////////////////////////////////////////
}
