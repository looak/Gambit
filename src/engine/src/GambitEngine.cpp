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
		return FENParser::Deserialize(fen, length, outputBoard, nullptr);
	}

	void Chess::Initialize()
	{

	}
}