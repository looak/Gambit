#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "../../engine/src/Board.h"
////////////////////////////////////////////////////////////////
using namespace GambitEngine;

namespace GambitTest {

////////////////////////////////////////////////////////////////
class UnmakeFixture : public ::testing::Test
{
public:
	GambitEngine::Board board;
	bool MakeMove(const char move[4])
	{
		const byte promotion = move[4] == 0 ? 0 : move[4];
		return board.MakeMove(move[0], move[1]-'0', move[2], move[3]-'0', promotion);
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

TEST_F(UnmakeFixture, Pawn_Three_Moves)
{
	board.PlacePiece(WHITE, PAWN, 'e', 1);
	MakeMove("e1e2");
	MakeMove("e2e3");
	MakeMove("e3e4");
	EXPECT_TRUE(board.UnmakeMove());
	EXPECT_EQ(0x01, board.GetValue('e', 3));

	EXPECT_TRUE(board.GetBitboard().IsSquareAttacked(WHITE, 29));
	auto mat = board.GetPieces(WHITE);
	EXPECT_EQ(1, mat.size());
	EXPECT_EQ(20, mat[0].Square8x8);
	EXPECT_EQ(PAWN, mat[0].Type);

	EXPECT_TRUE(board.UnmakeMove());
	EXPECT_EQ(0x01, board.GetValue('e', 2));

	EXPECT_TRUE(board.GetBitboard().IsSquareAttacked(WHITE, 21));
	mat = board.GetPieces(WHITE);
	EXPECT_EQ(1, mat.size());
	EXPECT_EQ(12, mat[0].Square8x8);
	EXPECT_EQ(PAWN, mat[0].Type);

	EXPECT_TRUE(MakeMove("e2e3"));
	EXPECT_EQ(0x01, board.GetValue('e', 3));
}

TEST_F(UnmakeFixture, Castling)
{
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));

	MakeMove("e1g1");
	EXPECT_TRUE(board.UnmakeMove());
	EXPECT_EQ(ROOK, board.GetValue('h', 1));
	EXPECT_EQ(KING, board.GetValue('e', 1));
	EXPECT_EQ(0x0, board.GetValue('g', 1));
}

TEST_F(UnmakeFixture, Castling_ReCastle)
{
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));

	MakeMove("e1g1");
	EXPECT_TRUE(board.UnmakeMove());
	EXPECT_EQ(ROOK, board.GetValue('h', 1));
	EXPECT_EQ(KING, board.GetValue('e', 1));
	EXPECT_EQ(0x0, board.GetValue('g', 1));
// second time, we can't believe it's tru it worked the first so we do it again :o
	EXPECT_TRUE(MakeMove("e1g1"));
	EXPECT_TRUE(board.UnmakeMove());
	EXPECT_EQ(ROOK, board.GetValue('h', 1));
	EXPECT_EQ(KING, board.GetValue('e', 1));
	EXPECT_EQ(0x0, board.GetValue('g', 1));

	EXPECT_TRUE(MakeMove("e1c1"));
	EXPECT_EQ(ROOK, board.GetValue('d', 1));
	EXPECT_EQ(KING, board.GetValue('c', 1));
	EXPECT_EQ(0x0, board.GetValue('a', 1));
}

TEST_F(UnmakeFixture, Promotion)
{
	std::string mvStr = "e7e8q";
	board.PlacePiece(WHITE, PAWN, 'e', 7);
	MakeMove(mvStr.c_str());

	EXPECT_EQ(0x05, board.GetValue('e', 8));

	EXPECT_TRUE(board.UnmakeMove());
	EXPECT_EQ(0x01, board.GetValue('e', 7));
	EXPECT_EQ(0x00, board.GetValue('e', 8));

	u64 matComb = board.GetBitboard().MaterialCombined(WHITE);
	u64 mask = UINT64_C(1) << 52;
	EXPECT_EQ(mask, matComb);
}

TEST_F(UnmakeFixture, Capture)
{
	board.PlacePiece(WHITE, KNIGHT, 'e', 2);
	board.PlacePiece(BLACK, KNIGHT, 'f', 4);

	EXPECT_TRUE(MakeMove("f4e2"));
	EXPECT_EQ(0x82, board.GetValue('e', 2));

	EXPECT_TRUE(board.UnmakeMove());

	EXPECT_EQ(0x02, board.GetValue('e', 2));
	EXPECT_EQ(0x82, board.GetValue('f', 4));

}

TEST_F(UnmakeFixture, CapturePromote)
{
	board.PlacePiece(BLACK, BISHOP, 'f', 8);
	board.PlacePiece(WHITE, PAWN, 'e', 7);
	EXPECT_TRUE(MakeMove("e7f8q"));
	EXPECT_EQ(0x0, board.GetValue('e', 7));
	EXPECT_EQ(0x05, board.GetValue('f', 8));

	EXPECT_TRUE(board.UnmakeMove());

	EXPECT_EQ(0x01, board.GetValue('e', 7));
	EXPECT_EQ(0x83, board.GetValue('f', 8));
}

TEST_F(UnmakeFixture, EnPassant)
{
	board.PlacePiece(WHITE, PAWN, 'e', 2);
	MakeMove("e2e4"); // triggers enPassant;
	board.UnmakeMove();

	EXPECT_EQ(0x01, board.GetValue('e', 2));
	board.PlacePiece(BLACK, PAWN, 'f', 4);
	EXPECT_FALSE(MakeMove("f4e3"));

	MakeMove("e2e4");
	EXPECT_TRUE(MakeMove("f4e3"));
}

TEST_F(UnmakeFixture, EnPassantTake)
{
	board.PlacePiece(WHITE, PAWN, 'e', 2);
	board.PlacePiece(BLACK, PAWN, 'f', 4);
	MakeMove("e2e4"); // triggers enPassant;

	EXPECT_TRUE(MakeMove("f4e3"));

	board.UnmakeMove();
	EXPECT_TRUE(MakeMove("f4e3"));

	EXPECT_EQ(0x0, board.GetValue('e', 4));
	EXPECT_EQ(0x81, board.GetValue('e', 3));
	EXPECT_EQ(0x0, board.GetValue('f', 4));
}
////////////////////////////////////////////////////////////////

}
