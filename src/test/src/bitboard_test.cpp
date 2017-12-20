#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "../../engine/src/Board.h"
////////////////////////////////////////////////////////////////

namespace GambitTest
{
////////////////////////////////////////////////////////////////
    class BitboardFixture : public ::testing::Test
    {
        //virtual void SetUp() { };
        //virtual void TearDown() {};
    };
////////////////////////////////////////////////////////////////
    TEST_F(BitboardFixture, EmptyBitboard)
    {
        EXPECT_EQ(true, true);
    }

    TEST_F(BitboardFixture, Bitboard_Knight_Attack)
    {
        GambitEngine::Board board;
        EXPECT_EQ(true, board.PlacePiece(WHITE, KNIGHT, 'a', 3));

        u64 attked = ~universe;
        // b1 should be attacked
        attked |= INT64_C(1) << INT64_C(1);
        // c2 should be attacked
        attked |= INT64_C(1) << INT64_C(10);
        // c4 should be attacked
        attked |= INT64_C(1) << INT64_C(26);
        // b5 should be attacked
        attked |= INT64_C(1) << INT64_C(33);

        auto result = board.GetBitboard().Attacked(WHITE);
        EXPECT_EQ(~universe, result ^ attked); //, L"Something is wrong with the result of attacked squares", LINE_INFO());
/*
        GambitEngine::Board board2;
        Assert::AreEqual(true, board2.PlacePiece(WHITE, KNIGHT, 'a', 1), L"Failed to place Knight", LINE_INFO());

        attked = ~universe;
        // c2 should be attacked
        attked |= 1i64 << 10i64;
        // a3 should be attacked
        attked |= 1i64 << 17i64;

        result = board2.GetBitboard().Attacked(WHITE);
        Assert::AreEqual(~universe, result ^ attked, L"Something is wrong with the result of attacked squares", LINE_INFO()); */
    }
////////////////////////////////////////////////////////////////

}

	/*
	TEST_CLASS(BitBoardTest)
	{
	public:
		TEST_METHOD(BitBoardTest_Knight_Attack)
			GambitEngine::Board board;
			Assert::AreEqual(true, board.PlacePiece(WHITE, KNIGHT, 'a', 3), L"Failed to place Knight", LINE_INFO());
			u64 attked = ~universe;
			// b1 should be attacked
			attked |= 1i64 << 1i64;
			// c2 should be attacked
			attked |= 1i64 << 10i64;
			// c4 should be attacked
			attked |= 1i64 << 26i64;
			// b5 should be attacked
			attked |= 1i64 << 33i64;

			auto result = board.GetBitboard().Attacked(WHITE);
			Assert::AreEqual(~universe, result ^ attked, L"Something is wrong with the result of attacked squares", LINE_INFO());

			GambitEngine::Board board2;
			Assert::AreEqual(true, board2.PlacePiece(WHITE, KNIGHT, 'a', 1), L"Failed to place Knight", LINE_INFO());

			attked = ~universe;
			// c2 should be attacked
			attked |= 1i64 << 10i64;
			// a3 should be attacked
			attked |= 1i64 << 17i64;

			result = board2.GetBitboard().Attacked(WHITE);
			Assert::AreEqual(~universe, result ^ attked, L"Something is wrong with the result of attacked squares", LINE_INFO());

		}
		{

		TEST_METHOD(BitBoardTest_King_Attack)
		{
			GambitEngine::Board board;
			Assert::AreEqual(true, board.PlacePiece(WHITE, KING, 'e', 1), L"Failed to place King", LINE_INFO());
			u64 attked = ~universe;
			// d1 should be attacked
			attked |= 1i64 << 3i64;
			// f1 should be attacked
			attked |= 1i64 << 5i64;
			// d2 should be attacked
			attked |= 1i64 << 11i64;
			// e2 should be attacked
			attked |= 1i64 << 12i64;
			// f2 should be attacked
			attked |= 1i64 << 13i64;			
			
			auto result = board.GetBitboard().Attacked(WHITE);
			Assert::AreEqual(~universe, result ^ attked, L"Something is wrong with the result of attacked squares", LINE_INFO());
		}

		TEST_METHOD(BitBoardTest_Rook_Attack)
		{
			GambitEngine::Board board;
			Assert::AreEqual(true, board.PlacePiece(WHITE, ROOK, 'h', 8), L"Failed to place Rook", LINE_INFO());
			u64 attked = ~universe;
			
			// h file
			u64 h7 = 55;
			for (__int64 hfile = h7; hfile >= 0; hfile-=8)
			{
				attked |= 1i64 << hfile;
			}			
			// 8 rank
			u64 g8 = 62;
			for (u64 rank = g8; rank > h7; rank -- )
			{
				attked |= 1i64 << rank;
			}

			auto result = board.GetBitboard().Attacked(WHITE);
			Assert::AreEqual(~universe, result ^ attked, L"Something is wrong with the result of attacked squares", LINE_INFO());
		}

		TEST_METHOD(BitBoardTest_Pawn_Attack)
		{
			GambitEngine::Board board;
			Assert::AreEqual(true, board.PlacePiece(WHITE, PAWN, 'e', 2), L"Failed to place Pawn", LINE_INFO());
			Assert::AreEqual(true, board.PlacePiece(WHITE, PAWN, 'f', 3), L"Failed to place Pawn", LINE_INFO());
			Assert::AreEqual(true, board.PlacePiece(BLACK, PAWN, 'd', 3), L"Failed to place Pawn", LINE_INFO());
			
			u64 attked = ~universe;
			// d3 should be attacked
			attked |= 1i64 << 19i64;
			// e4 should be attacked
			attked |= 1i64 << 28i64;
			// g4 should be attacked
			attked |= 1i64 << 30i64;
		
			auto result = board.GetBitboard().Attacked(WHITE);
			Assert::AreEqual(~universe, result ^ attked, L"Something is wrong with the result of white attacked squares", LINE_INFO());

			attked = ~universe;
			// e4 should be attacked
			attked |= 1i64 << 10i64;
			// g4 should be attacked
			attked |= 1i64 << 12i64;

			result = board.GetBitboard().Attacked(BLACK);
			Assert::AreEqual(~universe, result ^ attked, L"Something is wrong with the result of black attacked squares", LINE_INFO());
		}
	};
	 */