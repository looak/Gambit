#include "gtest/gtest.h"
////////////////////////////////////////////////////////////////
#include "Board.h"
#include "Material.h"
#include "MoveGenerator.h"
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
		board.PlacePiece(WHITE, KING, 'e', 1);
		
		EXPECT_TRUE(MakeMove(&board, "e1e2"));
		EXPECT_FALSE(MakeMove(&board, "e1e2"));

		auto whiteMat = board.GetMaterial((SET)0);
		auto value = whiteMat->GetPiece(byteSqr("e2"));
		bool result = nullptr == value;
		EXPECT_FALSE(result);
		EXPECT_EQ(value->Square8x8, byteSqr("e2"));

		value = whiteMat->GetPiece(byteSqr("e1"));
		result = nullptr == value;
		EXPECT_TRUE(result);
	}

	TEST_F(MaterialFixture, AddPieces_More)
	{
		board.PlacePiece(WHITE, BISHOP, 'c', 4);
		board.PlacePiece(WHITE, ROOK, 'h', 1);

		board.PlacePiece(BLACK, KNIGHT, 'f', 2);
		board.PlacePiece(BLACK, PAWN, 'f', 7);
		board.PlacePiece(BLACK, PAWN, 'g', 7);
		board.PlacePiece(BLACK, PAWN, 'h', 7);

		auto whiteMat = board.GetMaterial(WHITE);
		const Piece* value = whiteMat->GetPiece(byteSqr("c4"));
		bool result = nullptr == value;
		EXPECT_FALSE(result);
		EXPECT_EQ(BISHOP, value->Type);

		value = whiteMat->GetPiece(byteSqr("h1"));
		result = nullptr == value;
		EXPECT_FALSE(result);
		EXPECT_EQ(ROOK, value->Type);

		auto blackMat = board.GetMaterial(BLACK);
		value = blackMat->GetPiece(byteSqr("f2"));
		result = nullptr == value;
		EXPECT_FALSE(result);
		EXPECT_EQ(KNIGHT, value->Type);
		
		value = blackMat->GetPiece(byteSqr("f7"));
		result = nullptr == value;
		EXPECT_FALSE(result);
		EXPECT_EQ(PAWN, value->Type);

		value = blackMat->GetPiece(byteSqr("g7"));
		result = nullptr == value;
		EXPECT_FALSE(result);
		EXPECT_EQ(PAWN, value->Type);

		value = blackMat->GetPiece(byteSqr("h7"));
		result = nullptr == value;
		EXPECT_FALSE(result);
		EXPECT_EQ(PAWN, value->Type);

		value = blackMat->GetPiece(byteSqr("g7"));
		result = nullptr == value;
		EXPECT_FALSE(result);
		EXPECT_EQ(PAWN, value->Type);
	}


	TEST_F(MaterialFixture, Bishop_Capture_Pawn)
	{
		board.PlacePiece(WHITE, BISHOP, 'c', 4);
		board.PlacePiece(WHITE, ROOK, 'h', 1);

		board.PlacePiece(BLACK, KNIGHT, 'f', 2);
		board.PlacePiece(BLACK, PAWN, 'f', 7);
		board.PlacePiece(BLACK, PAWN, 'g', 7);
		board.PlacePiece(BLACK, PAWN, 'h', 7);

		byte f7 = byteSqr("f7");
		board.MakeMove(byteSqr("c4"), f7);
		auto whiteMat = board.GetMaterial(WHITE);

		auto value = whiteMat->GetPiece(f7);
		bool result = nullptr == value;
		EXPECT_FALSE(result);
		EXPECT_EQ(BISHOP, value->Type);

		auto blackMat = board.GetMaterial(BLACK);
		EXPECT_TRUE(blackMat->GetMaterial(PAWN).size() == (size_t)2);
		EXPECT_TRUE(blackMat->GetCaptured().size() == (size_t)1);
	}

	TEST_F(MaterialFixture, Bishop_Capture_Pawn_Unmake_and_Recapture)
	{
		board.PlacePiece(WHITE, BISHOP, 'c', 4);
		board.PlacePiece(WHITE, ROOK, 'h', 1);

		board.PlacePiece(BLACK, KNIGHT, 'f', 2);
		board.PlacePiece(BLACK, PAWN, 'f', 7);
		board.PlacePiece(BLACK, PAWN, 'g', 7);
		board.PlacePiece(BLACK, PAWN, 'h', 7);

		byte f7 = byteSqr("f7");
		board.MakeMove(byteSqr("c4"), f7);

		EXPECT_TRUE(board.UnmakeMove());
		auto blackMat = board.GetMaterial(BLACK);
		EXPECT_EQ(blackMat->GetCaptured().size(), (size_t)0);

		board.MakeMove(byteSqr("c4"), f7);

		auto whiteMat = board.GetMaterial(WHITE);

		auto value = whiteMat->GetPiece(f7);
		bool result = nullptr == value;
		EXPECT_FALSE(result);
		EXPECT_EQ(BISHOP, value->Type);

		EXPECT_TRUE(blackMat->GetMaterial(PAWN).size() == (size_t)2);
	}

	TEST_F(MaterialFixture, Bishop_Capture_BoardPointerTest)
	{
		board.PlacePiece(WHITE, BISHOP, 'c', 4);
		board.PlacePiece(WHITE, ROOK, 'h', 1);

		board.PlacePiece(BLACK, KNIGHT, 'f', 2);
		board.PlacePiece(BLACK, PAWN, 'f', 7);
		board.PlacePiece(BLACK, PAWN, 'g', 7);
		board.PlacePiece(BLACK, PAWN, 'h', 7);

		byte f7 = byteSqr("f7");
		byte g7 = byteSqr("g7");
		byte h7 = byteSqr("h7");

		board.MakeMove(byteSqr("c4"), f7);
		auto blackMat = board.GetMaterial(BLACK);
		EXPECT_TRUE(blackMat->GetPiece(g7)->Square8x8 == g7);
		// EXPECT_TRUE(blackMat->GetPiece(f7) == nullptr);
		EXPECT_TRUE(blackMat->GetPiece(h7)->Square8x8 == h7);

	}

	TEST_F(MaterialFixture, Bishop_Capture_BoardPointerTest_Unmake)
	{
		board.PlacePiece(WHITE, BISHOP, 'c', 4);
		board.PlacePiece(WHITE, ROOK, 'h', 1);

		board.PlacePiece(BLACK, KNIGHT, 'f', 2);
		board.PlacePiece(BLACK, PAWN, 'f', 7);
		board.PlacePiece(BLACK, PAWN, 'g', 7);
		board.PlacePiece(BLACK, PAWN, 'h', 7);

		byte f7 = byteSqr("f7");
		byte g7 = byteSqr("g7");
		byte h7 = byteSqr("h7");

		board.MakeMove(byteSqr("c4"), f7);
		EXPECT_TRUE(board.UnmakeMove());
		auto blackMat = board.GetMaterial(BLACK);
		EXPECT_TRUE(blackMat->GetPiece(g7)->Square8x8 == g7);
		EXPECT_TRUE(blackMat->GetPiece(f7)->Square8x8 == f7);
		EXPECT_TRUE(blackMat->GetPiece(h7)->Square8x8 == h7);

	}

	TEST_F(MaterialFixture, Promotion)
	{
		EXPECT_EQ(true, board.PlacePiece(WHITE, PAWN, 'd', 7));

		board.MakeMove(byteSqr("d7"), byteSqr("d8"), 'q');
		auto whiteMat = board.GetMaterial(WHITE);

		auto mat = whiteMat->GetMaterial(PAWN);
		EXPECT_EQ(0, mat.size());
		mat = whiteMat->GetMaterial(QUEEN);
		EXPECT_EQ(1, mat.size());
	}
	////////////////////////////////////////////////////////////////

}
