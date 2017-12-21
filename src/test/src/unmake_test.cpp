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

	EXPECT_TRUE(board.UnmakeMove());
	EXPECT_EQ(0x01, board.GetValue('e', 2));

	EXPECT_TRUE(board.GetBitboard().IsSquareAttacked(WHITE, 21));
	auto mat = board.GetPieces(WHITE);
	EXPECT_EQ(1, mat.size());
	EXPECT_EQ(12, mat[0].Square8x8);
	EXPECT_EQ(PAWN, mat[0].Type);
}
////////////////////////////////////////////////////////////////

}
