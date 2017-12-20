#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "../../engine/src/Board.h"
////////////////////////////////////////////////////////////////

namespace GambitTest
{

////////////////////////////////////////////////////////////////
class BoardFixture : public ::testing::Test
{
public:
};
////////////////////////////////////////////////////////////////


TEST_F(BoardFixture, Place_King)
{
	GambitEngine::Board board;

	EXPECT_EQ(true, board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_EQ(true, board.PlacePiece(BLACK, KING, 'e', 2));
	EXPECT_EQ(false, board.PlacePiece(WHITE, KING, 'e', 2));
	EXPECT_EQ(false, board.PlacePiece(WHITE, KING, 'i', 9));
}

TEST_F(BoardFixture, Move_King)
{
	GambitEngine::Board board;

	EXPECT_EQ(true, board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_EQ(true, board.MakeMove('e', 1, 'e', 2));
	EXPECT_EQ(false, board.MakeMove('e', 1, 'e', 2));
	byte var = 0x00 | KING;
	EXPECT_EQ(var, board.GetValue('e', 2));
}


TEST_F(BoardFixture, Move_Pawn)
{
	GambitEngine::Board board;

	EXPECT_EQ(true, board.PlacePiece(WHITE, PAWN, 'e', 2));
	EXPECT_EQ(true, board.MakeMove('e', 2, 'e', 4));
	EXPECT_EQ(true, board.PlacePiece(BLACK, PAWN, 'e', 7));
	EXPECT_EQ(true, board.MakeMove('e', 7, 'e', 5));

	EXPECT_EQ(true, board.PlacePiece(WHITE, PAWN, 'f', 4));
	EXPECT_EQ(true, board.MakeMove('e', 5, 'f', 4));
	
	EXPECT_EQ(true, board.PlacePiece(BLACK, PAWN, 'a', 3));
	EXPECT_EQ(true, board.PlacePiece(WHITE, PAWN, 'a', 2));
	EXPECT_EQ(false, board.MakeMove('a', 2, 'a', 4));
	EXPECT_EQ(true, board.PlacePiece(WHITE, PAWN, 'b', 2));
	EXPECT_EQ(true, board.MakeMove('b', 2, 'a', 3));

	byte var = 0x00 | PAWN;
	EXPECT_EQ(var, board.GetValue('a', 3));
	var |= 1 << 7;
	EXPECT_EQ(var, board.GetValue('f', 4));
	EXPECT_EQ(true, board.MakeMove('e', 4, 'e', 5));
}

TEST_F(BoardFixture, Move_Promotion)
{
	GambitEngine::Board board;

	EXPECT_EQ(true, board.PlacePiece(WHITE, PAWN, 'e', 7));
	EXPECT_EQ(true, board.PlacePiece(BLACK, PAWN, 'e', 2));

	EXPECT_EQ(true, board.MakeMove('e', 7, 'e', 8, 'q'));

	byte comp = 0x00 | QUEEN;
	EXPECT_EQ(comp, board.GetValue('e', 8));
	
	EXPECT_EQ(true, board.MakeMove('e', 2, 'e', 1, 'q'));
	comp = 0x00 | QUEEN;
	comp |= 1 << 7; // black
	EXPECT_EQ(comp, board.GetValue('e', 1));
}

TEST_F(BoardFixture, EnPassant)
{
	GambitEngine::Board board;

	EXPECT_EQ(true, board.PlacePiece(WHITE, PAWN, 'e', 2));
	EXPECT_EQ(true, board.PlacePiece(BLACK, PAWN, 'f', 4));
	EXPECT_EQ(true, board.MakeMove('e', 2, 'e', 4));
	EXPECT_EQ(true, board.MakeMove('f', 4, 'e', 3));
	
	byte var = 0x00;
	EXPECT_EQ(var, board.GetValue('e', 4));
	EXPECT_EQ((byte)0x81, board.GetValue('e', 3));
}

TEST_F(BoardFixture, Castling_White_KingSide)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_EQ(true, board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'h', 1));
	
	EXPECT_EQ(true, board.MakeMove('e', 1, 'g', 1));
	EXPECT_EQ((byte)0x00, board.GetValue('h', 1));
	EXPECT_EQ((byte)0x04, board.GetValue('f', 1));
}

TEST_F(BoardFixture, Castling_White_QueenSide)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available
	EXPECT_EQ(true, board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'h', 1));

	EXPECT_EQ(true, board.MakeMove('e', 1, 'c', 1));

	EXPECT_EQ((byte)0x00, board.GetValue('a', 1));
	EXPECT_EQ((byte)0x04, board.GetValue('d', 1));
}

TEST_F(BoardFixture, Castling_Black_KingSide)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_EQ(true, board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'a', 8));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'h', 8));

	EXPECT_EQ(true, board.MakeMove('e', 8, 'g', 8));
	EXPECT_EQ((byte)0x00, board.GetValue('h', 8));
	EXPECT_EQ((byte)0x84, board.GetValue('f', 8));
}

TEST_F(BoardFixture, Castling_Black_KingSide_Checked)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_EQ(true, board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'a', 8));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'h', 8));

	EXPECT_EQ(false, board.MakeMove('e', 8, 'g', 8));
}

TEST_F(BoardFixture, Castling_Black_KingSide_Rook_Attacked)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_EQ(true, board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'h', 1));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'h', 8));

	EXPECT_EQ(true, board.MakeMove('e', 8, 'g', 8));
}

TEST_F(BoardFixture, Castling_Black_QueenSide)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available
	EXPECT_EQ(true, board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'a', 8));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'h', 8));

	EXPECT_EQ(true, board.MakeMove('e', 8, 'c', 8));

	EXPECT_EQ((byte)0x00, board.GetValue('a', 8));
	EXPECT_EQ((byte)0x84, board.GetValue('d', 8));
}

TEST_F(BoardFixture, Castling_White_QueenSide_Failed)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_EQ(true, board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_EQ(true, board.PlacePiece(WHITE, KNIGHT, 'b', 1));

	EXPECT_EQ(false, board.MakeMove('e', 1, 'c', 1));

	EXPECT_EQ(true, board.MakeMove('b', 1, 'c', 3));

	EXPECT_EQ(true, board.MakeMove('e', 1, 'c', 1));
}

TEST_F(BoardFixture, Castling_Black_KingSide_Failed)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_EQ(true, board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'f', 1));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'h', 8));

	EXPECT_EQ(false, board.MakeMove('e', 8, 'g', 8));
}


TEST_F(BoardFixture, Castling_Black_RookMoved)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_EQ(true, board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'h', 8));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'a', 8));

	board.MakeMove('h', 8, 'h', 7);
	board.MakeMove('h', 7, 'h', 8);

	EXPECT_EQ(false, board.MakeMove('e', 8, 'g', 8));
	EXPECT_EQ(true, board.MakeMove('e', 8, 'c', 8));
}

TEST_F(BoardFixture, Castling_White_KingMoved)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_EQ(true, board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'h', 1));

	board.MakeMove('e', 1, 'e', 2);
	board.MakeMove('e', 2, 'e', 1);

	EXPECT_EQ(false, board.MakeMove('e', 1, 'c', 1));
	EXPECT_EQ(false, board.MakeMove('e', 1, 'g', 1));
}

TEST_F(BoardFixture, Checked_White)
{
	GambitEngine::Board board;

	EXPECT_EQ(true, board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'a', 1));

	EXPECT_EQ(true, board.Check(WHITE));
	EXPECT_EQ(false, board.CheckMate (WHITE));
}

TEST_F(BoardFixture, CheckMate_White)
{
	GambitEngine::Board board;
	board.SetCastlingRights(0);

	EXPECT_EQ(true, board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'b', 1));
	EXPECT_EQ(true, board.PlacePiece(BLACK, ROOK, 'a', 2));

	EXPECT_EQ(true, board.CheckMate(WHITE));

	EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'c', 5));
	EXPECT_EQ(false, board.CheckMate(WHITE));

	board.MakeLegalMove('c', 5, 'b', 5);
	EXPECT_EQ(false, board.CheckMate(WHITE));
	EXPECT_EQ(true, board.Check(WHITE));
}

////////////////////////////////////////////////////////////////
}