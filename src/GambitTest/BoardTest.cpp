#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\GambitEngine\GambitEngine.h"
#include "..\GambitEngine\Board.h"

namespace GambitTest
{
//
TEST_CLASS(BoardTest)
{
public:
	TEST_METHOD(BoardTest_PlaceKing)
	{
		GambitEngine::Board board;

		Assert::AreEqual(true, board.PlacePiece(WHITE, KING, 'e', 1), L"Failed to place King", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(BLACK, KING, 'e', 2), L"Failed to place King", LINE_INFO());
		Assert::AreEqual(false, board.PlacePiece(WHITE, KING, 'e', 2), L"Cant place piece here", LINE_INFO());
		Assert::AreEqual(false, board.PlacePiece(WHITE, KING, 'i', 9), L"Cant place piece here", LINE_INFO());
	}

	TEST_METHOD(BoardTest_MoveKing)
	{
		GambitEngine::Board board;

		Assert::AreEqual(true, board.PlacePiece(WHITE, KING, 'e', 1), L"Failed to input FEN", LINE_INFO());
		Assert::AreEqual(true, board.MakeMove('e', 1, 'e', 2), L"Failed to move King", LINE_INFO());
		Assert::AreEqual(false, board.MakeMove('e', 1, 'e', 2), L"There shouldnt be a piece on e1 which can do this move.", LINE_INFO());
		byte var = 0x00 | KING;
		Assert::AreEqual(var, board.GetValue('e', 2), L"There should be a king on e2.", LINE_INFO());
	}


	TEST_METHOD(BoardTest_MovePawn)
	{
		GambitEngine::Board board;

		Assert::AreEqual(true, board.PlacePiece(WHITE, PAWN, 'e', 2), L"Failed to place Pawn", LINE_INFO());
		Assert::AreEqual(true, board.MakeMove('e', 2, 'e', 4), L"Pawn should be able to jump two steps from starting position.", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(BLACK, PAWN, 'e', 7), L"Failed to place Pawn", LINE_INFO());
		Assert::AreEqual(true, board.MakeMove('e', 7, 'e', 5), L"Pawn should be able to jump two steps from starting position.", LINE_INFO());

		Assert::AreEqual(true, board.PlacePiece(WHITE, PAWN, 'f', 4), L"Failed to place Pawn", LINE_INFO());
		Assert::AreEqual(true, board.MakeMove('e', 5, 'f', 4), L"Pawn should be able to take diagonally.", LINE_INFO());
		
		Assert::AreEqual(true, board.PlacePiece(BLACK, PAWN, 'a', 3), L"Failed to place Pawn", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(WHITE, PAWN, 'a', 2), L"Failed to place Pawn", LINE_INFO());
		Assert::AreEqual(false, board.MakeMove('a', 2, 'a', 4), L"Can't move through other pieces.", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(WHITE, PAWN, 'b', 2), L"Failed to place Pawn", LINE_INFO());
		Assert::AreEqual(true, board.MakeMove('b', 2, 'a', 3), L"Should be able to take piece diagonally.", LINE_INFO());

		byte var = 0x00 | PAWN;
		Assert::AreEqual(var, board.GetValue('a', 3), L"There should be a white pawn on a3.", LINE_INFO());
		var |= 1 << 7;
		Assert::AreEqual(var, board.GetValue('f', 4), L"There should be a black pawn on f4.", LINE_INFO());
	}

	TEST_METHOD(BoardTest_EnPassant)
	{
		GambitEngine::Board board;

		Assert::AreEqual(true, board.PlacePiece(WHITE, PAWN, 'e', 2), L"Failed to place Pawn", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(BLACK, PAWN, 'f', 4), L"Failed to place Pawn", LINE_INFO());
		Assert::AreEqual(true, board.MakeMove('e', 2, 'e', 4), L"Pawn should be able to jump two steps from starting position.", LINE_INFO());		
		Assert::AreEqual(true, board.MakeMove('f', 4, 'e', 3), L"Pawn should be able to take en passant pawn.", LINE_INFO());
		
		byte var = 0x00;
		Assert::AreEqual(var, board.GetValue('e', 4), L"There should not be a pawn on e4.", LINE_INFO());		
	}

	TEST_METHOD(BoardTest_Castling_White_KingSide)
	{
		GambitEngine::Board board;

		Assert::AreEqual(true, board.PlacePiece(WHITE, KING, 'e', 1), L"Failed to place King", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(WHITE, ROOK, 'a', 1), L"Failed to place Rook", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(WHITE, ROOK, 'h', 1), L"Failed to place Rook", LINE_INFO());
		
		Assert::AreEqual(true, board.MakeMove('e', 1, 'g', 1), L"Failed to castle king side", LINE_INFO());
		Assert::AreEqual((byte)0x00, board.GetValue('h', 1), L"There should not be a rook on h1.", LINE_INFO());
		Assert::AreEqual((byte)0x04, board.GetValue('f', 1), L"There should be a rook on f1.", LINE_INFO());
	}

	TEST_METHOD(BoardTest_Castling_White_QueenSide)
	{
		GambitEngine::Board board;
		Assert::AreEqual(true, board.PlacePiece(WHITE, KING, 'e', 1), L"Failed to place King", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(WHITE, ROOK, 'a', 1), L"Failed to place Rook", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(WHITE, ROOK, 'h', 1), L"Failed to place Rook", LINE_INFO());

		Assert::AreEqual(true, board.MakeMove('e', 1, 'c', 1), L"Failed to castle queen side", LINE_INFO());

		Assert::AreEqual((byte)0x00, board.GetValue('a', 1), L"There should not be a rook on a1.", LINE_INFO());
		Assert::AreEqual((byte)0x04, board.GetValue('d', 1), L"There should be a rook on d1.", LINE_INFO());
	}

	TEST_METHOD(BoardTest_Castling_Black_KingSide)
	{
		GambitEngine::Board board;

		Assert::AreEqual(true, board.PlacePiece(BLACK, KING, 'e', 8), L"Failed to place King", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(BLACK, ROOK, 'a', 8), L"Failed to place Rook", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(BLACK, ROOK, 'h', 8), L"Failed to place Rook", LINE_INFO());

		Assert::AreEqual(true, board.MakeMove('e', 8, 'g', 8), L"Failed to castle king side", LINE_INFO());
		Assert::AreEqual((byte)0x00, board.GetValue('h', 8), L"There should not be a rook on h8.", LINE_INFO());
		Assert::AreEqual((byte)0x84, board.GetValue('f', 8), L"There should be a rook on f8.", LINE_INFO());
	}

	TEST_METHOD(BoardTest_Castling_Black_QueenSide)
	{
		GambitEngine::Board board;
		Assert::AreEqual(true, board.PlacePiece(BLACK, KING, 'e', 8), L"Failed to place King", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(BLACK, ROOK, 'a', 8), L"Failed to place Rook", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(BLACK, ROOK, 'h', 8), L"Failed to place Rook", LINE_INFO());

		Assert::AreEqual(true, board.MakeMove('e', 8, 'c', 8), L"Failed to castle queen side", LINE_INFO());

		Assert::AreEqual((byte)0x00, board.GetValue('a', 8), L"There should not be a rook on a8.", LINE_INFO());
		Assert::AreEqual((byte)0x84, board.GetValue('d', 8), L"There should be a rook on d8.", LINE_INFO());
	}

	TEST_METHOD(BoardTest_Castling_White_QueenSide_Failed)
	{
		GambitEngine::Board board;

		Assert::AreEqual(true, board.PlacePiece(WHITE, KING, 'e', 1), L"Failed to place King", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(WHITE, ROOK, 'a', 1), L"Failed to place Rook", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(WHITE, KNIGHT, 'b', 1), L"Failed to place Rook", LINE_INFO());

		Assert::AreEqual(false, board.MakeMove('e', 1, 'c', 1), L"Can't castle with pieces in between.", LINE_INFO());

		Assert::AreEqual(true, board.MakeMove('b', 1, 'c', 3), L"Failed to move Knight", LINE_INFO());

		Assert::AreEqual(true, board.MakeMove('e', 1, 'c', 1), L"Failed to castle queen side.", LINE_INFO());
	}

};

}