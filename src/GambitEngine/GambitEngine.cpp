// GambitEngine.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "GambitEngine.h"
#include "FENParser.h"
#include "Board.h"


namespace GambitEngine
{
	bool UCI::HelloWorld()
	{
		return true;
	}

	bool FEN::InputFen(char* fen, uint8_t length)
	{
		auto board = FENParser::Deserialize(fen, length);

		/*Board board;

		board.PlacePiece(WHITE, KING, 'e', 1);
		board.PlacePiece(BLACK, KING, 'e', 2);
		board.PlacePiece(WHITE, KING, 'e', 2);
		board.PlacePiece(WHITE, KING, 'i', 9);*/


		return false;
	}
}