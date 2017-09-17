// GambitEngine.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
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
		outputBoard = FENParser::Deserialize(fen, length);		
		return false;
	}
}