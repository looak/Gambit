#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////

#include "../../engine/src/Board.h"
#include "../../engine/src/AlgebraicNotation.h"
#include "../../engine/src/MoveGenerator.h"
////////////////////////////////////////////////////////////////
using namespace GambitEngine;

namespace GambitTest
{

	////////////////////////////////////////////////////////////////
	class NotationFixture : public ::testing::Test
	{
	public:
		GambitEngine::Board board;
		GambitEngine::MoveGenerator mvGen;
	};
	////////////////////////////////////////////////////////////////


	TEST_F(NotationFixture, King)
	{
		EXPECT_TRUE(board.PlacePiece(WHITE, KING, 'e', 1));
		EXPECT_TRUE(board.PlacePiece(BLACK, KING, 'e', 7));
		u32 count = 0;
		
		auto mvs = mvGen.getMoves(WHITE, &board, count);
		auto notation = Notation::ConvertMoves(mvs, board);

		bool found = false;
		for (u32 i = 0; i < notation.size(); i++)
		{
			if (notation[i] == "Kd1")
				found = true;
		}
		EXPECT_TRUE(found);
		EXPECT_EQ(notation.size(), 5);
	}

	TEST_F(NotationFixture, Pawn)
	{
		EXPECT_TRUE(board.PlacePiece(WHITE, PAWN, 'e', 2));		
		u32 count = 0;

		auto mvs = mvGen.getMoves(WHITE, &board, count, true);
		auto notation = Notation::ConvertMoves(mvs, board);

		bool found = false;
		for (u32 i = 0; i < notation.size(); i++)
		{
			if (notation[i] == "e3")
				found = true;
		}
		EXPECT_TRUE(found);
		EXPECT_EQ(notation.size(), 2);
	}
}