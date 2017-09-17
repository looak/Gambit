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
	};
}