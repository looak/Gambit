#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "Board.h"
#include "FENParser.h"
////////////////////////////////////////////////////////////////
using namespace GambitEngine;

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

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 2));
	EXPECT_FALSE(board.PlacePiece(WHITE, KING, 'e', 2));
	EXPECT_FALSE(board.PlacePiece(WHITE, KING, 'i', 9));
}

TEST_F(BoardFixture, Move_King)
{
	GambitEngine::Board board;

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.MakeMove('e', 1, 'e', 2));
	EXPECT_FALSE(board.MakeMove('e', 1, 'e', 2));
	byte var = 0x00 | KING;
	EXPECT_EQ(var, board.GetValue('e', 2));
}


TEST_F(BoardFixture, Move_Pawn)
{
	GambitEngine::Board board;

	EXPECT_TRUE(board.PlacePiece(WHITE, PAWN, 'e', 2));
	EXPECT_TRUE(board.MakeMove('e', 2, 'e', 4));
	EXPECT_TRUE(board.PlacePiece(BLACK, PAWN, 'e', 7));
	EXPECT_TRUE(board.MakeMove('e', 7, 'e', 5));

	EXPECT_TRUE(board.PlacePiece(WHITE, PAWN, 'f', 4));
	EXPECT_TRUE(board.MakeMove('e', 5, 'f', 4));
	
	EXPECT_TRUE(board.PlacePiece(BLACK, PAWN, 'a', 3));
	EXPECT_TRUE(board.PlacePiece(WHITE, PAWN, 'a', 2));
	EXPECT_FALSE(board.MakeMove('a', 2, 'a', 4));
	EXPECT_TRUE(board.PlacePiece(WHITE, PAWN, 'b', 2));
	EXPECT_TRUE(board.MakeMove('b', 2, 'a', 3));

	byte var = 0x00 | PAWN;
	EXPECT_EQ(var, board.GetValue('a', 3));
	var |= 1 << 7;
	EXPECT_EQ(var, board.GetValue('f', 4));
	EXPECT_TRUE(board.MakeMove('e', 4, 'e', 5));
}

TEST_F(BoardFixture, Move_PawnAcrossBoard)
{
	GambitEngine::Board board;

	EXPECT_TRUE(board.PlacePiece(WHITE, PAWN, 'e', 2));
	EXPECT_TRUE(board.PlacePiece(BLACK, PAWN, 'f', 7));
	EXPECT_TRUE(board.MakeMove('e', 2, 'e', 4));
	EXPECT_TRUE(board.MakeMove('f', 7, 'f', 5));

	EXPECT_TRUE(board.MakeMove('e', 4, 'e', 5));
	EXPECT_TRUE(board.MakeMove('f', 5, 'f', 4));

	EXPECT_TRUE(board.MakeMove('e', 5, 'e', 6));
	EXPECT_TRUE(board.MakeMove('f', 4, 'f', 3));
	
	EXPECT_TRUE(board.MakeMove('e', 6, 'e', 7));
	EXPECT_TRUE(board.MakeMove('f', 3, 'f', 2));

	EXPECT_TRUE(board.MakeMove('e', 7, 'e', 8, 'q'));
	EXPECT_TRUE(board.MakeMove('f', 2, 'f', 1, 'q'));
}

TEST_F(BoardFixture, Move_Promotion)
{
	GambitEngine::Board board;

	EXPECT_TRUE(board.PlacePiece(WHITE, PAWN, 'e', 7));
	EXPECT_TRUE(board.PlacePiece(BLACK, PAWN, 'e', 2));

	EXPECT_TRUE(board.MakeMove('e', 7, 'e', 8, 'q'));

	byte comp = 0x00 | QUEEN;
	EXPECT_EQ(comp, board.GetValue('e', 8));
	
	EXPECT_TRUE(board.MakeMove('e', 2, 'e', 1, 'q'));
	comp = 0x00 | QUEEN;
	comp |= 1 << 7; // black
	EXPECT_EQ(comp, board.GetValue('e', 1));
}

TEST_F(BoardFixture, EnPassant)
{
	GambitEngine::Board board;

	EXPECT_TRUE(board.PlacePiece(WHITE, PAWN, 'e', 2));
	EXPECT_TRUE(board.PlacePiece(BLACK, PAWN, 'f', 4));
	EXPECT_TRUE(board.MakeMove('e', 2, 'e', 4));
	EXPECT_TRUE(board.MakeMove('f', 4, 'e', 3));
	
	byte var = 0x00;
	EXPECT_EQ(var, board.GetValue('e', 4));
	EXPECT_EQ(var, board.GetValue('f', 4));
	EXPECT_EQ((byte)0x81, board.GetValue('e', 3));
}

TEST_F(BoardFixture, EnPassant_MoveBetween)
{
	GambitEngine::Board board;

	EXPECT_TRUE(board.PlacePiece(WHITE, PAWN, 'e', 2));
	EXPECT_TRUE(board.PlacePiece(WHITE, PAWN, 'f', 2));
	EXPECT_TRUE(board.PlacePiece(BLACK, PAWN, 'f', 4));
	EXPECT_TRUE(board.PlacePiece(BLACK, PAWN, 'e', 7));
	EXPECT_TRUE(board.MakeMove('e', 2, 'e', 4));
	EXPECT_TRUE(board.MakeMove('e', 7, 'e', 6));
	EXPECT_TRUE(board.MakeMove('f', 2, 'f', 3));
	EXPECT_FALSE(board.MakeMove('f', 4, 'e', 3));

	EXPECT_EQ(0x01, board.GetValue('e', 4));
	EXPECT_EQ(0x81, board.GetValue('f', 4));
}

TEST_F(BoardFixture, Castling_White_KingSide)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));
	
	EXPECT_TRUE(board.MakeMove('e', 1, 'g', 1));
	EXPECT_EQ((byte)0x00, board.GetValue('h', 1));
	EXPECT_EQ((byte)0x04, board.GetValue('f', 1));
}

TEST_F(BoardFixture, Castling_White_QueenSide)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available
	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));

	EXPECT_TRUE(board.MakeMove('e', 1, 'c', 1));

	EXPECT_EQ((byte)0x00, board.GetValue('a', 1));
	EXPECT_EQ((byte)0x04, board.GetValue('d', 1));
}

TEST_F(BoardFixture, Castling_Black_KingSide)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'a', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'h', 8));

	EXPECT_TRUE(board.MakeMove('e', 8, 'g', 8));
	EXPECT_EQ((byte)0x00, board.GetValue('h', 8));
	EXPECT_EQ((byte)0x84, board.GetValue('f', 8));
}

TEST_F(BoardFixture, Castling_Black_KingSide_Checked)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'h', 8));

	EXPECT_FALSE(board.MakeMove('e', 8, 'g', 8));
}

TEST_F(BoardFixture, Castling_Black_KingSide_Rook_Attacked)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'h', 8));

	EXPECT_TRUE(board.MakeMove('e', 8, 'g', 8));
}

TEST_F(BoardFixture, Castling_Black_KingSide_Threatened)
{
    GambitEngine::Board board;
    board.SetCastlingRights(15); // all available

    EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 8));
    EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));
    EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'h', 8));

    EXPECT_TRUE(board.MakeMove('e', 8, 'g', 8));
}

TEST_F(BoardFixture, Castling_Black_KingSide_NotThreatened)
{
    GambitEngine::Board board;
    board.SetCastlingRights(15); // all available

    EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 8));
    EXPECT_TRUE(board.PlacePiece(WHITE, PAWN, 'a', 7));
    EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'a', 8));

    EXPECT_TRUE(board.MakeMove('e', 8, 'c', 8));
}

TEST_F(BoardFixture, Castling_Black_QueenSide)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available
	EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'a', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'h', 8));

	EXPECT_TRUE(board.MakeMove('e', 8, 'c', 8));

	EXPECT_EQ((byte)0x00, board.GetValue('a', 8));
	EXPECT_EQ((byte)0x84, board.GetValue('d', 8));
}

TEST_F(BoardFixture, Castling_White_QueenSide_Failed)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, KNIGHT, 'b', 1));

	EXPECT_FALSE(board.MakeMove('e', 1, 'c', 1));

	EXPECT_TRUE(board.MakeMove('b', 1, 'c', 3));

	EXPECT_TRUE(board.MakeMove('e', 1, 'c', 1));
}

TEST_F(BoardFixture, Castling_Black_KingSide_Failed)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'f', 1));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'h', 8));

	EXPECT_FALSE(board.MakeMove('e', 8, 'g', 8));
}


TEST_F(BoardFixture, Castling_Black_RookMoved)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'h', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'a', 8));

	board.MakeMove('h', 8, 'h', 7);
	board.MakeMove('h', 7, 'h', 8);

	EXPECT_FALSE(board.MakeMove('e', 8, 'g', 8));
	EXPECT_TRUE(board.MakeMove('e', 8, 'c', 8));
}

TEST_F(BoardFixture, Castling_White_KingMoved)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));

	board.MakeMove('e', 1, 'e', 2);
	board.MakeMove('e', 2, 'e', 1);

	EXPECT_FALSE(board.MakeMove('e', 1, 'c', 1));
	EXPECT_FALSE(board.MakeMove('e', 1, 'g', 1));
}

TEST_F(BoardFixture, Castling_White_KingMoved_Diagonal)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));

	board.MakeMove('e', 1, 'd', 2);
		
	EXPECT_FALSE(board.MakeMove('d', 2, 'g', 1));
}

TEST_F(BoardFixture, Castling_White_AlreadyCastled)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));

	board.MakeMove('e', 1, 'c', 1); // castle

	EXPECT_FALSE(board.MakeMove('c', 1, 'g', 1));  // expect not to be able to castle again
}

TEST_F(BoardFixture, Castling_Black_AlreadyCastled)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'a', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'h', 8));

	EXPECT_TRUE(board.MakeMove('e', 8, 'g', 8)); // castle

	EXPECT_FALSE(board.MakeMove('e', 8, 'c', 8));  // expect not to be able to castle again
}

TEST_F(BoardFixture, Castling_White_RookCaptured)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));
	EXPECT_TRUE(board.PlacePiece(BLACK, KNIGHT, 'f', 2));
		
	board.MakeMove('f', 2, 'h', 1);

	EXPECT_FALSE(board.MakeMove('e', 1, 'g', 1));
}

TEST_F(BoardFixture, Castling_White_KingMovedRookCaptured)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));
	EXPECT_TRUE(board.PlacePiece(BLACK, KNIGHT, 'f', 2));

	board.MakeMove('e', 1, 'd', 2);
	board.MakeMove('f', 2, 'h', 1);

	EXPECT_FALSE(board.MakeMove('d', 2, 'g', 1));
}

TEST_F(BoardFixture, Castling_White_RookCaptured_QueenSide)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'a', 1));
	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'h', 1));
	EXPECT_TRUE(board.PlacePiece(BLACK, KNIGHT, 'c', 2));

	board.MakeMove('c', 2, 'a', 1);

	EXPECT_FALSE(board.MakeMove('e', 1, 'c', 1));
}


TEST_F(BoardFixture, Castling_Black_RookCaptured)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'a', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'h', 8));
	EXPECT_TRUE(board.PlacePiece(WHITE, KNIGHT, 'f', 7));

	board.MakeMove('f', 7, 'h', 8);

	EXPECT_FALSE(board.MakeMove('e', 8, 'g', 8));
}

TEST_F(BoardFixture, Castling_Black_RookCaptured_QueenSide)
{
	GambitEngine::Board board;
	board.SetCastlingRights(15); // all available

	EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'a', 8));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'h', 8));
	EXPECT_TRUE(board.PlacePiece(WHITE, KNIGHT, 'c', 7));

	board.MakeMove('c', 7, 'a', 8);

	EXPECT_FALSE(board.MakeMove('e', 8, 'c', 8));
}

TEST_F(BoardFixture, Checked_White)
{
	GambitEngine::Board board;

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'a', 1));

	EXPECT_TRUE(board.Check(WHITE));
	EXPECT_FALSE(board.CheckMate (WHITE));
}

TEST_F(BoardFixture, CheckMate_White)
{
	GambitEngine::Board board;
	board.SetCastlingRights(0);

	EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'b', 1));
	EXPECT_TRUE(board.PlacePiece(BLACK, ROOK, 'a', 2));

	EXPECT_TRUE(board.CheckMate(WHITE));

	EXPECT_TRUE(board.PlacePiece(WHITE, ROOK, 'c', 5));
	EXPECT_FALSE(board.CheckMate(WHITE));

	board.MakeLegalMove('c', 5, 'b', 5);
	EXPECT_FALSE(board.CheckMate(WHITE));
	EXPECT_TRUE(board.Check(WHITE));
}

TEST_F(BoardFixture, PinnedPiece_CheckMate)
{
	GambitEngine::Board board;
	board.SetCastlingRights(0);

	board.PlacePiece(WHITE, KING, 'e', 1);
	board.PlacePiece(BLACK, KING, 'e', 8);

	board.PlacePiece(BLACK, BISHOP, 'e', 6);

	board.PlacePiece(WHITE, ROOK, 'e', 2);
	board.PlacePiece(WHITE, ROOK, 'f', 2);
	board.PlacePiece(WHITE, ROOK, 'a', 7);
	board.PlacePiece(WHITE, QUEEN, 'c', 8);
	
	EXPECT_TRUE(board.CheckMate(BLACK));
}

TEST_F(BoardFixture, GuardedPiece_CheckMate)
{
	GambitEngine::Board board;
	board.SetCastlingRights(0);
	
	std::string fen = "k7/5b2/8/8/8/8/r6p/1K6 b - -";
	FENParser::Deserialize(fen.c_str(), (u32)fen.length(), board, nullptr);

	EXPECT_FALSE(board.CheckMate(WHITE));

	board.MakeLegalMove('h', 2, 'h', 1, 'q');
	EXPECT_TRUE(board.CheckMate(WHITE));
}

TEST_F(BoardFixture, GuardedPiece_CheckMate)
{
	GambitEngine::Board board;
	board.SetCastlingRights(0);

	std::string fen = "k7/2K5/1NN5/8/8/8/8/8 b - -0 1";
	FENParser::Deserialize(fen.c_str(), (u32)fen.length(), board, nullptr);

	EXPECT_TRUE(board.CheckMate(WHITE));
}

TEST_F(BoardFixture, CapturePromote)
{
	GambitEngine::Board board;

	board.PlacePiece(BLACK, BISHOP, 'f', 8);
	board.PlacePiece(WHITE, PAWN, 'e', 7);
	EXPECT_TRUE(board.MakeMove('e',7,'f',8,'q'));
	EXPECT_EQ(0x0, board.GetValue('e', 7));
	EXPECT_EQ(0x05, board.GetValue('f', 8));
}

TEST_F(BoardFixture, CapturePromote_Black)
{
	GambitEngine::Board board;
		
	board.PlacePiece(BLACK, PAWN, 'b', 2);
	board.PlacePiece(WHITE, ROOK, 'c', 1);

	EXPECT_TRUE(board.MakeMove('b', 2, 'c', 1, 'q'));
	EXPECT_EQ(0x0, board.GetValue('b', 2));
	EXPECT_EQ(0x85, board.GetValue('c', 1));
}

TEST_F(BoardFixture, CapturePromote_Black_Fail)
{
	GambitEngine::Board board;

	board.PlacePiece(BLACK, PAWN, 'b', 2);
	board.PlacePiece(WHITE, ROOK, 'c', 1);

	EXPECT_FALSE(board.MakeMove('b', 2, 'a', 1, 'q'));
	EXPECT_EQ(0x81, board.GetValue('b', 2));
	EXPECT_EQ(0x0, board.GetValue('a', 1));
	EXPECT_EQ(0x04, board.GetValue('c', 1));
}

TEST_F(BoardFixture, LegalBoard)
{
	GambitEngine::Board board;
	board.PlacePiece(BLACK, KING, 'e', 8);
	board.PlacePiece(BLACK, ROOK, 'e', 7);
	board.PlacePiece(WHITE, KING, 'e', 1);

	EXPECT_FALSE(board.Legal());
}

TEST_F(BoardFixture, LegalBoard_PosFive)
{
	GambitEngine::Board board;
	char inputFen[] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
	FENParser::Deserialize(inputFen, sizeof(inputFen), board, nullptr);

	EXPECT_TRUE(board.Legal());

	board.MakeMove(48, 32, 0);
	EXPECT_TRUE(board.Legal());

	board.UnmakeMove();
	EXPECT_TRUE(board.Legal());
}

TEST_F(BoardFixture, IllegalMove)
{
	GambitEngine::Board board;

	board.PlacePiece(BLACK, KING, 'e', 8);
	board.PlacePiece(BLACK, ROOK, 'e', 7);

	board.PlacePiece(WHITE, KNIGHT, 'e', 3);
	board.PlacePiece(WHITE, KING, 'e', 1);

	EXPECT_FALSE(board.MakeMove('e', 3, 'c', 2));

	EXPECT_TRUE(board.Legal());
}
////////////////////////////////////////////////////////////////
}