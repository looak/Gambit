#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "../../engine/src/Bitboard.h"
////////////////////////////////////////////////////////////////

namespace GambitTest
{
////////////////////////////////////////////////////////////////
    class BitboardFixture : public ::testing::Test
    {
    public:
     /*   virtual void SetUp()
        {

        };
        virtual void TearDown() {};
*/
    };
////////////////////////////////////////////////////////////////
    TEST_F(BitboardFixture, Empty)
    {
        GambitEngine::Bitboard bitboard;
        auto combined = bitboard.MaterialCombined(WHITE) & bitboard.MaterialCombined(BLACK);
        EXPECT_EQ(combined, 0x0);
    }

    TEST_F(BitboardFixture, Knight_Attack)
    {
        GambitEngine::Bitboard bitboard;
        EXPECT_EQ(true, bitboard.PlacePiece(WHITE, KNIGHT, 'a', 3));

        u64 attked = ~universe;
        // b1 should be attacked
        attked |= INT64_C(1) << INT64_C(1);
        // c2 should be attacked
        attked |= INT64_C(1) << INT64_C(10);
        // c4 should be attacked
        attked |= INT64_C(1) << INT64_C(26);
        // b5 should be attacked
        attked |= INT64_C(1) << INT64_C(33);

        auto result = bitboard.Attacked(WHITE);
        //, L"Something is wrong with the result of attacked squares", LINE_INFO());
        EXPECT_EQ(~universe, result ^ attked);

        bitboard.Clear();
        auto combined = bitboard.MaterialCombined(WHITE) & bitboard.MaterialCombined(BLACK);
        EXPECT_EQ(combined, 0x0);

        bitboard.PlacePiece(WHITE, KNIGHT, 'a', 1);

        attked = ~universe;
        // c2 should be attacked
        attked |= INT64_C(1) << INT64_C(10);
        // a3 should be attacked
        attked |= INT64_C(1) << INT64_C(17);

        result = bitboard.Attacked(WHITE);
        EXPECT_EQ(~universe, result ^ attked);
    }

    TEST_F(BitboardFixture, King_Attack)
    {
        GambitEngine::Bitboard board;
        EXPECT_EQ(true, board.PlacePiece(WHITE, KING, 'e', 1));
        u64 attked = ~universe;
        // d1 should be attacked
        attked |= INT64_C( 1) << INT64_C( 3);
        // f1 should be attacked
        attked |= INT64_C( 1) << INT64_C( 5);
        // d2 should be attacked
        attked |= INT64_C( 1) << INT64_C(11);
        // e2 should be attacked
        attked |= INT64_C( 1) << INT64_C(12);
        // f2 should be attacked
        attked |= INT64_C( 1) << INT64_C(13);

        auto result = board.Attacked(WHITE);
        EXPECT_EQ(~universe, result ^ attked);
    }

    TEST_F(BitboardFixture, Rook_Attack)
    {
        GambitEngine::Bitboard board;
        EXPECT_EQ(true, board.PlacePiece(WHITE, ROOK, 'h', 8));
        u64 attked = ~universe;

        // h file
        u64 h7 = 55;
        for (i64 hfile = h7; hfile >= 0; hfile-=8)
        {
            attked |= INT64_C(1) << hfile;
        }
        // 8 rank
        u64 g8 = 62;
        for (u64 rank = g8; rank > h7; rank -- )
        {
            attked |= INT64_C(1) << rank;
        }

        auto result = board.Attacked(WHITE);
        EXPECT_EQ(~universe, result ^ attked);
    }


    TEST_F(BitboardFixture, Pawn_Attack) {
        GambitEngine::Bitboard board;
        EXPECT_EQ(true, board.PlacePiece(WHITE, PAWN, 'e', 2));
        board.PlacePiece(WHITE, PAWN, 'f', 3);
        board.PlacePiece(BLACK, PAWN, 'd', 3);

        u64 attked = ~universe;
        // d3 should be attacked
        attked |= INT64_C(1) << INT64_C(19);
        // e4 should be attacked
        attked |= INT64_C(1) << INT64_C(28);
        // g4 should be attacked
        attked |= INT64_C(1) << INT64_C(30);

        auto result = board.Attacked(WHITE);
        EXPECT_EQ(~universe, result ^ attked);

        attked = ~universe;
        // e4 should be attacked
        attked |= INT64_C(1) << INT64_C(10);
        // g4 should be attacked
        attked |= INT64_C(1) << INT64_C(12);

        result = board.Attacked(BLACK);
        EXPECT_FALSE(result ^ attked);
    }

    TEST_F(BitboardFixture, King_Available_Moves)
    {
        GambitEngine::Bitboard board;
        board.PlacePiece(WHITE, KING, 'e', 1);

        u64 expectedAvaMv = ~universe;
        expectedAvaMv |= INT64_C(1) << INT64_C(3);
        expectedAvaMv |= INT64_C(1) << INT64_C(5);
        expectedAvaMv |= INT64_C(1) << INT64_C(11);
        expectedAvaMv |= INT64_C(1) << INT64_C(12);
        expectedAvaMv |= INT64_C(1) << INT64_C(13);

        u64 avaMoves = board.AvailableMovesSimple(WHITE, KING, 4);

        EXPECT_EQ(expectedAvaMv, avaMoves);
    }

    TEST_F(BitboardFixture, King_Black_Available_Moves)
    {
        GambitEngine::Bitboard board;
        board.PlacePiece(BLACK, KING, 'e', 8);

        u64 expectedAvaMv = ~universe;
        expectedAvaMv |= INT64_C(1) << INT64_C(59);
        expectedAvaMv |= INT64_C(1) << INT64_C(61);
        expectedAvaMv |= INT64_C(1) << INT64_C(53);
        expectedAvaMv |= INT64_C(1) << INT64_C(52);
        expectedAvaMv |= INT64_C(1) << INT64_C(51);

		byte prom = 0;
        u64 avaMoves = board.AvailableMoves(BLACK, KING, 60, 0, 0, prom);

        EXPECT_EQ(expectedAvaMv, avaMoves);
    }


	TEST_F(BitboardFixture, King_Black_Available_Moves_With_Rooks)
	{
		GambitEngine::Bitboard board;
		board.PlacePiece(BLACK, KING, 'e', 8);
		board.PlacePiece(BLACK, ROOK, 'h', 8);
		board.PlacePiece(BLACK, ROOK, 'a', 8);

		u64 expectedAvaMv = ~universe;
		expectedAvaMv |= INT64_C(1) << INT64_C(58);
		expectedAvaMv |= INT64_C(1) << INT64_C(59);
		expectedAvaMv |= INT64_C(1) << INT64_C(61);
		expectedAvaMv |= INT64_C(1) << INT64_C(62);
		expectedAvaMv |= INT64_C(1) << INT64_C(53);
		expectedAvaMv |= INT64_C(1) << INT64_C(52);
		expectedAvaMv |= INT64_C(1) << INT64_C(51);

		byte prom = 0;
		u64 avaMoves = board.AvailableMoves(BLACK, KING, 60, 0, 0xc, prom);

		EXPECT_EQ(expectedAvaMv, avaMoves);
	}
////////////////////////////////////////////////////////////////

}