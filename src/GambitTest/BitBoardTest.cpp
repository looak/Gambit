#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\GambitEngine\GambitEngine.h"
#include "..\GambitEngine\Board.h"

namespace GambitTest
{
	//
	TEST_CLASS(BitBoardTest)
	{
	public:
		TEST_METHOD(BitBoardTest_KnightAttack)
		{
			GambitEngine::Board board;
			Assert::AreEqual(true, board.PlacePiece(WHITE, KNIGHT, 'a', 3), L"Failed to place Knight", LINE_INFO());
			uint64 attked = ~universe;
			// b1 should be attacked
			attked |= 1i64 << 1i64;
			// c2 should be attacked
			attked |= 1i64 << 10i64;
			// c4 should be attacked
			attked |= 1i64 << 26i64;
			// b5 should be attacked
			attked |= 1i64 << 33i64;

			auto result = board.bitboard.Attacked(WHITE);
			Assert::AreEqual(~universe, result ^ attked, L"Something is wrong with the result of attacked squares", LINE_INFO());

			GambitEngine::Board board2;
			Assert::AreEqual(true, board2.PlacePiece(WHITE, KNIGHT, 'a', 1), L"Failed to place Knight", LINE_INFO());

			attked = ~universe;
			// c2 should be attacked
			attked |= 1i64 << 10i64;
			// a3 should be attacked
			attked |= 1i64 << 17i64;			

			result = board2.bitboard.Attacked(WHITE);
			Assert::AreEqual(~universe, result ^ attked, L"Something is wrong with the result of attacked squares", LINE_INFO());

		}
	};
};