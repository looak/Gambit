#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "Board.h"
#include "GameState.h"
#include "GambitEngine.h"
#include <optional>
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

TEST_F(FenFixture, StartingPosition)
{
	bool expectedValue = true;

	char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	uint8_t length = sizeof(inputFen);
	GambitEngine::Board board;

	bool result = GambitEngine::FEN::InputFen(inputFen, length, board);

	EXPECT_EQ(expectedValue, result);
}

TEST_F(FenFixture, StartingPosition_NoWhiteSpace_fail)
{
	bool expectedValue = false;

	char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNRwKQkq-01";
	uint8_t length = sizeof(inputFen);
	GambitEngine::Board board;

	bool result = GambitEngine::FEN::InputFen(inputFen, length, board);

	EXPECT_EQ(expectedValue, result);
}

TEST_F(FenFixture, StartingPosition_ShortFen_succeed)
{
	/* should write default castling state, no en passant, ply 1 and white to start */
	bool expectedValue = true;

	char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	uint8_t length = sizeof(inputFen);
	GambitEngine::Board board;

	bool result = GambitEngine::FEN::InputFen(inputFen, length, board);
	
	EXPECT_EQ(expectedValue, result);
}

TEST_F(FenFixture, Castling)
{
	bool expectedValue = true;

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

TEST_F(FenFixture, serialize_board_successful)
{
	std::string expectedFEN{ "8/8/8/4Pp2/8/8/8/R3K2R" };

	char inputFen[] = "8/8/8/4Pp2/8/8/8/R3K2R w KQ f6 0 1";
	uint8_t length = sizeof(inputFen);

	// setup board
	GambitEngine::Board board;
	GambitEngine::GameState state(board);
	GambitEngine::FEN::InputFen(inputFen, length, board);

	// serialize back.
	auto result = GambitEngine::FEN::OutputFEN(state);

	EXPECT_TRUE(result.size() > 0);
	for (u32 i = 0; i < expectedFEN.size(); ++i) {
		EXPECT_EQ(result[i], expectedFEN[i]);
	}
}

TEST_F(FenFixture, serialize_board_start_position_successful)
{
	std::string expectedFEN{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

	char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	uint8_t length = sizeof(inputFen);

	// setup board
	GambitEngine::Board board;
	GambitEngine::GameState state(board);
	GambitEngine::FEN::InputFen(inputFen, length, state);

	// serialize back.
	auto result = GambitEngine::FEN::OutputFEN(state);

	EXPECT_TRUE(result.size() > 0);
	for (u32 i = 0; i < length; ++i) {
		EXPECT_EQ(result[i], expectedFEN[i]);
	}
}
////////////////////////////////////////////////////////////////

}