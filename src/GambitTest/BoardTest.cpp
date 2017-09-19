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

		Assert::AreEqual((byte)0x06, board.GetValue('e', 2), L"There should be a king on e2.", LINE_INFO());
	}
};

}