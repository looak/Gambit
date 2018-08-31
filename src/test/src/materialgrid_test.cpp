#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////
#include "../../engine/src/Board.h"
#include "../../engine/src/Material.h"
#include "../../engine/src/MoveGenerator.h"
////////////////////////////////////////////////////////////////
using namespace GambitEngine;

namespace GambitTest {

	////////////////////////////////////////////////////////////////
	class MaterialFixture : public ::testing::Test
	{
	public:
		GambitEngine::Board board;

		static byte byteSqr(const char sqr[2])
		{
			byte corrFile = sqr[0] - 'a';
			byte corrRank = (byte)(sqr[1] - '1');
			return corrRank * (byte)8 + corrFile;			
		}
		
		static bool MakeMove(Board* board, const char move[4])
		{
			const byte promotion = move[4] == 0 ? 0 : move[4];
			return board->MakeMove(move[0], move[1] - '0', move[2], move[3] - '0', promotion);
		}
	};
	////////////////////////////////////////////////////////////////

	TEST_F(MaterialFixture, AddPiece_Move)
	{
		GambitEngine::Board board;
		board.PlacePiece(WHITE, KING, 'e', 1);
		
		EXPECT_TRUE(MakeMove(&board, "e1e2"));
		EXPECT_FALSE(MakeMove(&board, "e1e2"));

		auto whiteMat = board.GetMaterial()[0];
		Piece* value = whiteMat.GetPiece(byteSqr("e2"));
		EXPECT_NE(nullptr, value);
		EXPECT_EQ(value->Square8x8, byteSqr("e2"));

		value = whiteMat.GetPiece(byteSqr("e1"));
		EXPECT_EQ(nullptr, value); 
	}

	TEST_F(MaterialFixture, CopyMaterial)
	{
		GambitEngine::Board board;
		board.PlacePiece(WHITE, BISHOP, 'c', 4);

		auto cpyOne = board.GetMaterial()[0];
		auto cpyTwo = board.GetMaterial()[0];
		Piece* valueOne = cpyOne.GetPiece(byteSqr("c4"));
		Piece* valueTwo = cpyTwo.GetPiece(byteSqr("c4"));

		EXPECT_NE(valueOne, valueTwo);
		EXPECT_EQ(BISHOP, valueOne->Type);
		EXPECT_EQ(BISHOP, valueTwo->Type);
		EXPECT_EQ(26, valueOne->Square8x8);
		EXPECT_EQ(26, valueTwo->Square8x8);


		auto value = cpyOne.GetPiece(byteSqr("c5"));
		EXPECT_EQ(nullptr, value);

		EXPECT_EQ(nullptr, cpyTwo.GetKing());
	}


	TEST_F(MaterialFixture, AddPieces_Capture)
	{
		GambitEngine::Board board;
		board.PlacePiece(WHITE, BISHOP, 'c', 4);
		board.PlacePiece(WHITE, ROOK, 'h', 1);

		board.PlacePiece(BLACK, KNIGHT, 'f', 2);
		board.PlacePiece(BLACK, PAWN, 'f', 7);
		board.PlacePiece(BLACK, PAWN, 'g', 7);
		board.PlacePiece(BLACK, PAWN, 'h', 7);

		auto whiteMat = board.GetMaterial()[0];
		Piece* value = whiteMat.GetPiece(byteSqr("c4"));
		EXPECT_NE(nullptr, value);
		EXPECT_EQ(BISHOP, value->Type);

		value = whiteMat.GetPiece(byteSqr("h1"));
		EXPECT_NE(nullptr, value);
		EXPECT_EQ(ROOK, value->Type);

		auto blackMat = board.GetMaterial()[1];
		value = blackMat.GetPiece(byteSqr("f2"));
		EXPECT_NE(nullptr, value);
		EXPECT_EQ(KNIGHT, value->Type);
		
		value = blackMat.GetPiece(byteSqr("f7"));
		EXPECT_NE(nullptr, value);
		EXPECT_EQ(PAWN, value->Type);

		value = blackMat.GetPiece(byteSqr("g7"));
		EXPECT_NE(nullptr, value);
		EXPECT_EQ(PAWN, value->Type);

		value = blackMat.GetPiece(byteSqr("h7"));
		EXPECT_NE(nullptr, value);
		EXPECT_EQ(PAWN, value->Type);

		value = blackMat.GetPiece(byteSqr("f7"));
		EXPECT_TRUE(blackMat.CapturePiece(*value));

		value = blackMat.GetPiece(byteSqr("f7"));
		EXPECT_EQ(nullptr, value);

		value = blackMat.GetPiece(byteSqr("g7"));
		EXPECT_NE(nullptr, value);
		EXPECT_EQ(PAWN, value->Type);
	}

	TEST_F(MaterialFixture, Promotion)
	{
		u32 count = 0;
		EXPECT_EQ(true, board.PlacePiece(WHITE, PAWN, 'd', 7));

		board.MakeMove(byteSqr("d7"), byteSqr("d8"), 'q');
		auto whiteMat = board.GetMaterial()[0];

		auto mat = whiteMat.GetMaterial(PAWN);
		EXPECT_EQ(0, mat.size());
		mat = whiteMat.GetMaterial(QUEEN);
		EXPECT_EQ(1, mat.size());
	}
	////////////////////////////////////////////////////////////////

}

