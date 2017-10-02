#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\GambitEngine\GambitEngine.h"

namespace GambitTest
{		
	//
	TEST_CLASS(FenTests)
	{
	public:
		
		TEST_METHOD(FenInput_StartingBoardTest)
		{
			// starting position FEN
			// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

			char inputFen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

			Assert::AreEqual(true, false, L"Failed to setup board.", LINE_INFO());
		}

		TEST_METHOD(FenInput_WrongFen_TooFewPieces)
		{
			bool expectedValue = false;

			// white king on white king starting position.
			char inputFen[] = "8/8/8/8/8/8/8/4K2 w - - 0 1";
			uint8_t length = sizeof(inputFen);
			GambitEngine::Board board;

			bool result = GambitEngine::FEN::InputFen(inputFen, length, board);

			// handle too short FEN
			Assert::AreEqual(expectedValue, result, L"Failed to input FEN", LINE_INFO());
		}


		TEST_METHOD(FenInput_WhiteKingOnly)
		{
			bool expectedValue = true;

			// white king on white king starting position.
			char inputFen[] = "8/8/8/8/8/8/8/4K3 w - - 0 1";
			uint8_t length = sizeof(inputFen);
			GambitEngine::Board board;

			bool result = GambitEngine::FEN::InputFen(inputFen, length, board);
			
			Assert::AreEqual(expectedValue, result, L"Failed to input FEN", LINE_INFO());
		}

		TEST_METHOD(FenInput_Castling)
		{
			bool expectedValue = true;

			// white king on white king starting position.
			char inputFen[] = "8/8/8/8/8/8/8/R3K2R w KQ - 0 1";
			uint8_t length = sizeof(inputFen);
			GambitEngine::Board board;

			bool result = GambitEngine::FEN::InputFen(inputFen, length, board);

			Assert::AreEqual(expectedValue, result, L"Failed to input FEN", LINE_INFO());

			// castling
			result = board.MakeMove('e', 1, 'a', 1);
			Assert::AreEqual(expectedValue, result, L"Failed to castle", LINE_INFO());
		}


		TEST_METHOD(FenInput_EnPassant_White)
		{
			bool expectedValue = true;

			// white king on white king starting position.
			char inputFen[] = "8/8/8/8/4Pp2/8/8/R3K2R w KQ e3 0 1";
			uint8_t length = sizeof(inputFen);
			GambitEngine::Board board;

			bool result = GambitEngine::FEN::InputFen(inputFen, length, board);

			Assert::AreEqual(expectedValue, result, L"Failed to input FEN", LINE_INFO());

			// take pawn
			result = board.MakeMove('f', 4, 'e', 3);
			Assert::AreEqual(expectedValue, result, L"Failed to take en passant pawn", LINE_INFO());
			byte var = 0x00;
			Assert::AreEqual(var, board.GetValue('e', 4), L"There should not be a pawn on e4.", LINE_INFO());
		}

		TEST_METHOD(FenInput_EnPassant_Black)
		{
			bool expectedValue = true;

			// white king on white king starting position.
			char inputFen[] = "8/8/8/4Pp2/8/8/8/R3K2R w KQ f6 0 1";
			uint8_t length = sizeof(inputFen);
			GambitEngine::Board board;

			bool result = GambitEngine::FEN::InputFen(inputFen, length, board);

			Assert::AreEqual(expectedValue, result, L"Failed to input FEN", LINE_INFO());

			// take pawn
			result = board.MakeMove('e', 5, 'f', 6);
			Assert::AreEqual(expectedValue, result, L"Failed to take en passant pawn", LINE_INFO());
			byte var = 0x00;
			Assert::AreEqual(var, board.GetValue('f', 5), L"There should not be a pawn on e4.", LINE_INFO());

		}

	};
}