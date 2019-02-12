#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "Board.h"
#include "GambitEngine.h"
////////////////////////////////////////////////////////////////

namespace GambitTest
{

////////////////////////////////////////////////////////////////
class FenFixture : public ::testing::Test
{
public:
};
////////////////////////////////////////////////////////////////

TEST_F(FenFixture, WrongFen_TooFewPieces)
{
	bool expectedValue = false;

	// white king on white king starting position.
	char inputFen[] = "8/8/8/8/8/8/8/4K2 w - - 0 1";
	uint8_t length = sizeof(inputFen);
	GambitEngine::Board board;

	bool result = GambitEngine::FEN::InputFen(inputFen, length, board);

	// handle too short FEN
	EXPECT_EQ(expectedValue, result);
}


TEST_F(FenFixture, WhiteKingOnly)
{
	bool expectedValue = true;

	// white king on white king starting position.
	char inputFen[] = "8/8/8/8/8/8/8/4K3 w - - 0 1";
	uint8_t length = sizeof(inputFen);
	GambitEngine::Board board;

	bool result = GambitEngine::FEN::InputFen(inputFen, length, board);
	
	EXPECT_EQ(expectedValue, result);
}

TEST_F(FenFixture, Castling)
{
	bool expectedValue = true;

	// white king on white king starting position.
	char inputFen[] = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQ - 0 1";
	uint8_t length = sizeof(inputFen);
	GambitEngine::Board board;

	bool result = GambitEngine::FEN::InputFen(inputFen, length, board);

	EXPECT_EQ(expectedValue, result);

	// castling
	result = board.MakeMove('e', 1, 'c', 1);
	EXPECT_EQ(expectedValue, result);

	result = board.MakeMove('e', 8, 'c', 8);
	EXPECT_EQ(false, result);
}


TEST_F(FenFixture, EnPassant_White)
{
	bool expectedValue = true;

	// white king on white king starting position.
	char inputFen[] = "8/8/8/8/4Pp2/8/8/R3K2R w KQ e3 0 1";
	uint8_t length = sizeof(inputFen);
	GambitEngine::Board board;

	bool result = GambitEngine::FEN::InputFen(inputFen, length, board);

	EXPECT_EQ(expectedValue, result);

	// take pawn
	result = board.MakeMove('f', 4, 'e', 3);
	EXPECT_EQ(expectedValue, result);
	byte var = 0x00;
	EXPECT_EQ(var, board.GetValue('e', 4));
	EXPECT_EQ((byte)0x81, board.GetValue('e', 3));
}

TEST_F(FenFixture, EnPassant_Black)
{
	bool expectedValue = true;

	// white king on white king starting position.
	char inputFen[] = "8/8/8/4Pp2/8/8/8/R3K2R w KQ f6 0 1";
	uint8_t length = sizeof(inputFen);
	GambitEngine::Board board;

	bool result = GambitEngine::FEN::InputFen(inputFen, length, board);

	EXPECT_EQ(expectedValue, result);

	// take pawn
	result = board.MakeMove('e', 5, 'f', 6);
	EXPECT_EQ(expectedValue, result);
	byte var = 0x00;
	EXPECT_EQ(var, board.GetValue('f', 5));

}
////////////////////////////////////////////////////////////////

}