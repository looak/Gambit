#include "GambitEngine.h"
#include "FENParser.h"

namespace GambitEngine
{
	bool UCI::HelloWorld()
	{
		return true;
	}

	bool FEN::InputFen(char* fen, uint8_t length, Board& outputBoard)
	{	
		// if we fail to parse fen, reset board.
		if (!FENParser::Deserialize(fen, length, outputBoard, nullptr))
		{
			outputBoard.ResetBoard();
			return false;
		}
		return true;
	}

	void Chess::Initialize()
	{

	}
}