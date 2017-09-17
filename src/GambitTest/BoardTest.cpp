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

		Assert::AreEqual(true, board.PlacePiece(WHITE, KING, 'e', 1), L"Failed to input FEN", LINE_INFO());
		Assert::AreEqual(true, board.PlacePiece(BLACK, KING, 'e', 2), L"Failed to input FEN", LINE_INFO());
		Assert::AreEqual(false, board.PlacePiece(WHITE, KING, 'e', 2), L"Failed to input FEN", LINE_INFO());
		Assert::AreEqual(false, board.PlacePiece(WHITE, KING, 'i', 9), L"Failed to input FEN", LINE_INFO());
	}
};

}