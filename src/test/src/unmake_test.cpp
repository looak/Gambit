#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "../../engine/src/Board.h"
////////////////////////////////////////////////////////////////

namespace GambitTest {

////////////////////////////////////////////////////////////////
class UnmakeFixture : public ::testing::Test
{
public:
	GambitEngine::Board board;
	void MakeMove(char move[4])
	{
		byte promotion = move[4] == 0 ? 0 : move[4];
		board.MakeMove(move[0], move[1]-'0', move[2], move[3]-'0', promotion);
	}
};
////////////////////////////////////////////////////////////////

TEST_F(UnmakeFixture, Pawn_One_Move)
{
	board.PlacePiece(WHITE, PAWN, 'e', 2);
	MakeMove("e2e3");

	EXPECT_EQ(true, board.UnmakeMove());
	EXPECT_EQ(0x01, board.GetValue('e', 2));
}
////////////////////////////////////////////////////////////////

}
